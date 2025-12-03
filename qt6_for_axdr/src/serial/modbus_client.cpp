/**
 * @file modbus_client.cpp
 * @brief Modbus RTU客户端类实现
 * @note 使用QSerialPort直接实现Modbus RTU协议
 */

#include "modbus_client.h"
#include "log/comm_logger.h"
#include <QDebug>

modbus_client_t::modbus_client_t(QObject *parent)
    : QObject(parent)
    , m_serial_port(nullptr)
    , m_state(e_disconnected)
    , m_is_busy(false)
    , m_timeout_timer(nullptr)
    , m_reconnect_timer(nullptr)
    , m_reconnect_attempts(0)
    , m_expected_response_len(0)
    , m_current_start_addr(0)
    , m_current_function_code(0)
{
    m_serial_port = new QSerialPort(this);
    
    connect(m_serial_port, &QSerialPort::readyRead,
            this, &modbus_client_t::slot_on_ready_read);
    connect(m_serial_port, &QSerialPort::errorOccurred,
            this, &modbus_client_t::slot_on_error_occurred);
    
    /* 初始化定时器 */
    m_timeout_timer = new QTimer(this);
    m_timeout_timer->setSingleShot(true);
    connect(m_timeout_timer, &QTimer::timeout,
            this, &modbus_client_t::slot_on_timeout);
    
    m_reconnect_timer = new QTimer(this);
    connect(m_reconnect_timer, &QTimer::timeout,
            this, &modbus_client_t::slot_reconnect_timer);
}

modbus_client_t::~modbus_client_t()
{
    disconnect_device();
}

/**
 * @brief 连接Modbus设备
 * @param config 串口配置参数
 * @return 连接是否成功
 */
bool modbus_client_t::connect_device(const serial_config_t &config)
{
    QMutexLocker locker(&m_mutex);
    
    qDebug() << "[modbus_client] connect_device() 被调用, 端口:" << config.port_name;
    
    if (m_serial_port->isOpen()) {
        m_serial_port->close();
    }

    m_config = config;
    m_state = e_connecting;
    emit signal_connection_changed(m_state);

    /* 配置串口参数 */
    m_serial_port->setPortName(config.port_name);
    m_serial_port->setBaudRate(config.baud_rate);
    m_serial_port->setDataBits(config.data_bits);
    m_serial_port->setParity(config.parity);
    m_serial_port->setStopBits(config.stop_bits);
    m_serial_port->setFlowControl(QSerialPort::NoFlowControl);

    qDebug() << "[modbus_client] 尝试打开串口...";
    
    if (!m_serial_port->open(QIODevice::ReadWrite)) {
        m_state = e_error;
        emit signal_connection_changed(m_state);
        QString error_msg = QString("打开串口失败: %1").arg(m_serial_port->errorString());
        qDebug() << "[modbus_client]" << error_msg;
        emit signal_error_occurred(error_msg);
        return false;
    }

    qDebug() << "[modbus_client] 串口打开成功";
    m_state = e_connected;
    m_reconnect_attempts = 0;
    emit signal_connection_changed(m_state);
    return true;
}

/**
 * @brief 断开设备连接
 */
void modbus_client_t::disconnect_device()
{
    QMutexLocker locker(&m_mutex);
    
    m_timeout_timer->stop();
    m_reconnect_timer->stop();
    m_reconnect_attempts = 0;
    m_recv_buffer.clear();
    m_is_busy = false;
    
    if (m_serial_port->isOpen()) {
        m_serial_port->close();
    }
    
    m_state = e_disconnected;
    emit signal_connection_changed(m_state);
}

connection_state_E modbus_client_t::get_connection_state() const
{
    return m_state;
}

serial_config_t modbus_client_t::get_current_config() const
{
    return m_config;
}

/**
 * @brief 读取保持寄存器
 * @param start_addr 起始地址
 * @param count 读取数量
 */
void modbus_client_t::read_holding_registers(int start_addr, quint16 count)
{
    QMutexLocker locker(&m_mutex);
    
    if (m_state != e_connected) {
        emit signal_error_occurred("未连接设备，无法读取");
        return;
    }
    
    /* 检查是否正在等待响应 */
    if (m_is_busy) {
        qDebug() << "[modbus_client] 请求被跳过(繁忙中), 地址:" << start_addr;
        return;
    }

    QByteArray request = build_read_request(m_config.server_address, start_addr, count);
    
    m_recv_buffer.clear();
    m_current_start_addr = start_addr;
    m_current_function_code = MODBUS_FC_READ_HOLDING_REGISTERS;
    m_expected_response_len = 5 + count * 2;  /* 地址+功能码+字节数+数据+CRC */
    m_is_busy = true;
    
    comm_logger_t::instance()->log_send(request, 
        QString("读取寄存器 地址:0x%1 数量:%2").arg(start_addr, 4, 16, QChar('0')).arg(count));
    
    m_serial_port->write(request);
    m_timeout_timer->start(m_config.response_timeout);
}

/**
 * @brief 写入单个保持寄存器
 * @param addr 寄存器地址
 * @param value 写入值
 */
void modbus_client_t::write_holding_register(int addr, quint16 value)
{
    QMutexLocker locker(&m_mutex);
    
    if (m_state != e_connected) {
        emit signal_error_occurred("未连接设备，无法写入");
        return;
    }
    
    if (m_is_busy) {
        qDebug() << "[modbus_client] 写入请求被跳过(繁忙中), 地址:" << addr;
        return;
    }

    QByteArray request = build_write_single_request(m_config.server_address, addr, value);
    
    m_recv_buffer.clear();
    m_current_start_addr = addr;
    m_current_function_code = MODBUS_FC_WRITE_SINGLE_REGISTER;
    m_expected_response_len = 8;  /* 固定8字节响应 */
    m_is_busy = true;
    
    comm_logger_t::instance()->log_send(request, 
        QString("写单个寄存器 地址:0x%1 值:%2").arg(addr, 4, 16, QChar('0')).arg(value));
    
    m_serial_port->write(request);
    m_timeout_timer->start(m_config.response_timeout);
}

/**
 * @brief 写入多个保持寄存器
 * @param start_addr 起始地址
 * @param values 写入值列表
 */
void modbus_client_t::write_holding_registers(int start_addr, const QVector<quint16> &values)
{
    QMutexLocker locker(&m_mutex);
    
    if (m_state != e_connected) {
        emit signal_error_occurred("未连接设备，无法写入");
        return;
    }
    
    if (m_is_busy) {
        qDebug() << "[modbus_client] 批量写入请求被跳过(繁忙中), 地址:" << start_addr;
        return;
    }

    QByteArray request = build_write_multiple_request(m_config.server_address, start_addr, values);
    
    m_recv_buffer.clear();
    m_current_start_addr = start_addr;
    m_current_function_code = MODBUS_FC_WRITE_MULTIPLE_REGISTERS;
    m_expected_response_len = 8;  /* 固定8字节响应 */
    m_is_busy = true;
    
    QString values_str;
    for (int i = 0; i < values.size(); ++i) {
        values_str += QString("%1 ").arg(values[i], 4, 16, QChar('0')).toUpper();
    }
    comm_logger_t::instance()->log_send(request, 
        QString("写多个寄存器 地址:0x%1 数量:%2 值:[%3]")
            .arg(start_addr, 4, 16, QChar('0')).arg(values.size()).arg(values_str.trimmed()));
    
    m_serial_port->write(request);
    m_timeout_timer->start(m_config.response_timeout);
}

/* ============== 协议构建函数 ============== */

/**
 * @brief 构建读取寄存器请求帧
 */
QByteArray modbus_client_t::build_read_request(quint8 slave_addr, quint16 start_addr, quint16 count)
{
    QByteArray frame;
    frame.append(slave_addr);
    frame.append(MODBUS_FC_READ_HOLDING_REGISTERS);
    frame.append((start_addr >> 8) & 0xFF);
    frame.append(start_addr & 0xFF);
    frame.append((count >> 8) & 0xFF);
    frame.append(count & 0xFF);
    
    quint16 crc = calc_crc16(frame);
    frame.append(crc & 0xFF);
    frame.append((crc >> 8) & 0xFF);
    
    return frame;
}

/**
 * @brief 构建写入单个寄存器请求帧
 */
QByteArray modbus_client_t::build_write_single_request(quint8 slave_addr, quint16 addr, quint16 value)
{
    QByteArray frame;
    frame.append(slave_addr);
    frame.append(MODBUS_FC_WRITE_SINGLE_REGISTER);
    frame.append((addr >> 8) & 0xFF);
    frame.append(addr & 0xFF);
    frame.append((value >> 8) & 0xFF);
    frame.append(value & 0xFF);
    
    quint16 crc = calc_crc16(frame);
    frame.append(crc & 0xFF);
    frame.append((crc >> 8) & 0xFF);
    
    return frame;
}

/**
 * @brief 构建写入多个寄存器请求帧
 */
QByteArray modbus_client_t::build_write_multiple_request(quint8 slave_addr, quint16 start_addr,
                                                          const QVector<quint16> &values)
{
    QByteArray frame;
    frame.append(slave_addr);
    frame.append(MODBUS_FC_WRITE_MULTIPLE_REGISTERS);
    frame.append((start_addr >> 8) & 0xFF);
    frame.append(start_addr & 0xFF);
    
    quint16 count = values.size();
    frame.append((count >> 8) & 0xFF);
    frame.append(count & 0xFF);
    
    quint8 byte_count = count * 2;
    frame.append(byte_count);
    
    for (quint16 value : values) {
        frame.append((value >> 8) & 0xFF);
        frame.append(value & 0xFF);
    }
    
    quint16 crc = calc_crc16(frame);
    frame.append(crc & 0xFF);
    frame.append((crc >> 8) & 0xFF);
    
    return frame;
}

/**
 * @brief 计算CRC16校验值
 * @note Modbus RTU使用CRC-16/MODBUS多项式
 */
quint16 modbus_client_t::calc_crc16(const QByteArray &data)
{
    quint16 crc = 0xFFFF;
    
    for (int i = 0; i < data.size(); ++i) {
        crc ^= (quint8)data[i];
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    
    return crc;
}

/* ============== 响应解析函数 ============== */

/**
 * @brief 解析读取响应
 */
bool modbus_client_t::parse_read_response(const QByteArray &response, QVector<quint16> &values)
{
    if (response.size() < 5) {
        return false;
    }
    
    /* 验证CRC */
    QByteArray data_part = response.left(response.size() - 2);
    quint16 recv_crc = ((quint8)response[response.size()-1] << 8) | (quint8)response[response.size()-2];
    if (calc_crc16(data_part) != recv_crc) {
        return false;
    }
    
    /* 检查功能码 */
    if ((quint8)response[1] != MODBUS_FC_READ_HOLDING_REGISTERS) {
        return false;
    }
    
    quint8 byte_count = (quint8)response[2];
    int reg_count = byte_count / 2;
    
    values.clear();
    for (int i = 0; i < reg_count; ++i) {
        quint16 value = ((quint8)response[3 + i*2] << 8) | (quint8)response[4 + i*2];
        values.append(value);
    }
    
    return true;
}

/**
 * @brief 解析写入响应
 */
bool modbus_client_t::parse_write_response(const QByteArray &response)
{
    if (response.size() < 8) {
        return false;
    }
    
    /* 验证CRC */
    QByteArray data_part = response.left(response.size() - 2);
    quint16 recv_crc = ((quint8)response[response.size()-1] << 8) | (quint8)response[response.size()-2];
    if (calc_crc16(data_part) != recv_crc) {
        return false;
    }
    
    /* 检查是否为异常响应 */
    if ((quint8)response[1] & 0x80) {
        return false;
    }
    
    return true;
}

/* ============== 槽函数 ============== */

/**
 * @brief 串口数据接收槽
 */
void modbus_client_t::slot_on_ready_read()
{
    m_recv_buffer.append(m_serial_port->readAll());
    
    /* 检查是否接收完整 */
    if (m_recv_buffer.size() >= m_expected_response_len) {
        m_timeout_timer->stop();
        m_is_busy = false;  /* 清除繁忙标志 */
        
        comm_logger_t::instance()->log_recv(m_recv_buffer, 
            QString("响应 功能码:0x%1").arg(m_current_function_code, 2, 16, QChar('0')));
        
        if (m_current_function_code == MODBUS_FC_READ_HOLDING_REGISTERS) {
            QVector<quint16> values;
            if (parse_read_response(m_recv_buffer, values)) {
                QString values_str;
                for (int i = 0; i < values.size(); ++i) {
                    values_str += QString("%1 ").arg(values[i], 4, 16, QChar('0')).toUpper();
                }
                comm_logger_t::instance()->log_info(
                    QString("解析成功 地址:0x%1 值:[%2]")
                        .arg(m_current_start_addr, 4, 16, QChar('0')).arg(values_str.trimmed()));
                emit signal_read_completed(m_current_start_addr, values);
            } else {
                comm_logger_t::instance()->log_error("读取响应解析失败");
                emit signal_error_occurred("读取响应解析失败");
            }
        } else {
            if (parse_write_response(m_recv_buffer)) {
                comm_logger_t::instance()->log_info("写入成功");
                emit signal_write_completed(m_current_start_addr, true);
            } else {
                comm_logger_t::instance()->log_error("写入响应解析失败");
                emit signal_error_occurred("写入响应解析失败");
                emit signal_write_completed(m_current_start_addr, false);
            }
        }
        
        m_recv_buffer.clear();
    }
}

/**
 * @brief 串口错误槽
 */
void modbus_client_t::slot_on_error_occurred(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError) {
        return;
    }
    
    m_timeout_timer->stop();
    m_is_busy = false;  /* 清除繁忙标志 */
    m_recv_buffer.clear();
    m_state = e_error;
    emit signal_connection_changed(m_state);
    emit signal_error_occurred(m_serial_port->errorString());
    
    handle_reconnect();
}

/**
 * @brief 超时处理槽
 */
void modbus_client_t::slot_on_timeout()
{
    m_is_busy = false;  /* 清除繁忙标志 */
    emit signal_error_occurred("通信超时");
    m_recv_buffer.clear();
    
    if (m_current_function_code == MODBUS_FC_READ_HOLDING_REGISTERS) {
        /* 读取超时不发送完成信号 */
    } else {
        emit signal_write_completed(m_current_start_addr, false);
    }
}

/**
 * @brief 重连定时器槽
 */
void modbus_client_t::slot_reconnect_timer()
{
    if (m_reconnect_attempts >= MAX_RECONNECT_ATTEMPTS) {
        m_reconnect_timer->stop();
        emit signal_error_occurred("重连失败，已达最大尝试次数");
        return;
    }

    m_reconnect_attempts++;
    connect_device(m_config);
}

/**
 * @brief 处理重连
 */
void modbus_client_t::handle_reconnect()
{
    if (!m_reconnect_timer->isActive() && 
        m_reconnect_attempts < MAX_RECONNECT_ATTEMPTS) {
        m_reconnect_timer->start(RECONNECT_INTERVAL_MS);
    }
}
