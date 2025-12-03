/**
 * @file modbus_slave.cpp
 * @brief Modbus RTU从机模拟器类实现
 */

#include "modbus_slave.h"
#include <QDebug>

modbus_slave_t::modbus_slave_t(QObject *parent)
    : QObject(parent)
    , m_serial_port(nullptr)
    , m_slave_address(1)
    , m_state(e_slave_stopped)
    , m_frame_timer(nullptr)
{
    m_serial_port = new QSerialPort(this);
    m_frame_timer = new QTimer(this);
    m_frame_timer->setSingleShot(true);

    connect(m_serial_port, &QSerialPort::readyRead,
            this, &modbus_slave_t::slot_on_ready_read);
    connect(m_serial_port, &QSerialPort::errorOccurred,
            this, &modbus_slave_t::slot_on_serial_error);
    connect(m_frame_timer, &QTimer::timeout,
            this, &modbus_slave_t::slot_process_frame);
}

modbus_slave_t::~modbus_slave_t()
{
    stop_listening();
}

bool modbus_slave_t::start_listening(const QString &port_name, qint32 baud_rate,
                                      QSerialPort::DataBits data_bits,
                                      QSerialPort::Parity parity,
                                      QSerialPort::StopBits stop_bits)
{
    if (m_state == e_slave_running) {
        stop_listening();
    }

    m_serial_port->setPortName(port_name);
    m_serial_port->setBaudRate(baud_rate);
    m_serial_port->setDataBits(data_bits);
    m_serial_port->setParity(parity);
    m_serial_port->setStopBits(stop_bits);

    if (!m_serial_port->open(QIODevice::ReadWrite)) {
        QString error_msg = QString("无法打开串口 %1: %2")
                            .arg(port_name)
                            .arg(m_serial_port->errorString());
        emit signal_error_occurred(error_msg);
        m_state = e_slave_error;
        emit signal_state_changed(m_state);
        return false;
    }

    m_recv_buffer.clear();
    m_state = e_slave_running;
    emit signal_state_changed(m_state);
    return true;
}

void modbus_slave_t::stop_listening()
{
    if (m_serial_port->isOpen()) {
        m_serial_port->close();
    }
    m_frame_timer->stop();
    m_recv_buffer.clear();
    m_state = e_slave_stopped;
    emit signal_state_changed(m_state);
}

slave_state_E modbus_slave_t::get_state() const
{
    return m_state;
}

void modbus_slave_t::set_slave_address(quint8 address)
{
    m_slave_address = address;
}

quint8 modbus_slave_t::get_slave_address() const
{
    return m_slave_address;
}

void modbus_slave_t::set_register(quint16 address, quint16 value)
{
    if (m_registers.contains(address)) {
        m_registers[address].value = value;
    } else {
        register_info_t info;
        info.name = QString("寄存器_%1").arg(address, 4, 16, QChar('0')).toUpper();
        info.value = value;
        m_registers[address] = info;
    }
    emit signal_register_changed(address, value);
}

void modbus_slave_t::set_register_with_name(quint16 address, const QString &name, quint16 value)
{
    register_info_t info;
    info.name = name;
    info.value = value;
    m_registers[address] = info;
    emit signal_register_changed(address, value);
}

quint16 modbus_slave_t::get_register(quint16 address) const
{
    if (m_registers.contains(address)) {
        return m_registers[address].value;
    }
    return 0;
}

QString modbus_slave_t::get_register_name(quint16 address) const
{
    if (m_registers.contains(address)) {
        return m_registers[address].name;
    }
    return QString();
}

QMap<quint16, register_info_t> modbus_slave_t::get_all_registers() const
{
    return m_registers;
}

void modbus_slave_t::clear_registers()
{
    m_registers.clear();
}

void modbus_slave_t::slot_on_ready_read()
{
    m_recv_buffer.append(m_serial_port->readAll());
    m_frame_timer->start(FRAME_TIMEOUT_MS);
}

void modbus_slave_t::slot_on_serial_error(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError) {
        QString error_msg = QString("串口错误: %1").arg(m_serial_port->errorString());
        emit signal_error_occurred(error_msg);
        if (error != QSerialPort::TimeoutError) {
            m_state = e_slave_error;
            emit signal_state_changed(m_state);
        }
    }
}

void modbus_slave_t::slot_process_frame()
{
    if (m_recv_buffer.isEmpty()) {
        return;
    }

    emit signal_request_received(m_recv_buffer);
    process_request(m_recv_buffer);
    m_recv_buffer.clear();
}

void modbus_slave_t::process_request(const QByteArray &request)
{
    /* 最小帧长度检查: 地址(1) + 功能码(1) + 数据(至少2) + CRC(2) = 6 */
    if (request.size() < 6) {
        return;
    }

    /* CRC校验 */
    if (!verify_crc(request)) {
        return;
    }

    quint8 slave_addr = static_cast<quint8>(request[0]);
    quint8 function_code = static_cast<quint8>(request[1]);

    /* 地址匹配检查 */
    if (slave_addr != m_slave_address && slave_addr != 0) {
        return;
    }

    QByteArray response;

    switch (function_code) {
    case MODBUS_FC_READ_HOLDING_REGISTERS: {
        if (request.size() < 8) {
            return;
        }
        quint16 start_addr = (static_cast<quint8>(request[2]) << 8) | static_cast<quint8>(request[3]);
        quint16 count = (static_cast<quint8>(request[4]) << 8) | static_cast<quint8>(request[5]);
        response = handle_read_holding_registers(start_addr, count);
        break;
    }
    case MODBUS_FC_WRITE_SINGLE_REGISTER: {
        if (request.size() < 8) {
            return;
        }
        quint16 addr = (static_cast<quint8>(request[2]) << 8) | static_cast<quint8>(request[3]);
        quint16 value = (static_cast<quint8>(request[4]) << 8) | static_cast<quint8>(request[5]);
        response = handle_write_single_register(addr, value);
        break;
    }
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS: {
        if (request.size() < 9) {
            return;
        }
        quint16 start_addr = (static_cast<quint8>(request[2]) << 8) | static_cast<quint8>(request[3]);
        quint8 byte_count = static_cast<quint8>(request[6]);
        if (request.size() < 9 + byte_count) {
            return;
        }
        QByteArray data = request.mid(7, byte_count);
        response = handle_write_multiple_registers(start_addr, data);
        break;
    }
    default:
        response = build_exception_response(function_code, MODBUS_EX_ILLEGAL_FUNCTION);
        break;
    }

    if (!response.isEmpty() && slave_addr != 0) {
        m_serial_port->write(response);
        emit signal_response_sent(response);
    }
}

QByteArray modbus_slave_t::handle_read_holding_registers(quint16 start_addr, quint16 count)
{
    /* 检查寄存器范围 */
    for (quint16 i = 0; i < count; ++i) {
        if (!m_registers.contains(start_addr + i)) {
            return build_exception_response(MODBUS_FC_READ_HOLDING_REGISTERS, MODBUS_EX_ILLEGAL_DATA_ADDRESS);
        }
    }

    QByteArray response;
    response.append(static_cast<char>(m_slave_address));
    response.append(static_cast<char>(MODBUS_FC_READ_HOLDING_REGISTERS));
    response.append(static_cast<char>(count * 2));

    for (quint16 i = 0; i < count; ++i) {
        quint16 value = m_registers[start_addr + i].value;
        response.append(static_cast<char>((value >> 8) & 0xFF));
        response.append(static_cast<char>(value & 0xFF));
    }

    quint16 crc = calc_crc16(response);
    response.append(static_cast<char>(crc & 0xFF));
    response.append(static_cast<char>((crc >> 8) & 0xFF));

    return response;
}

QByteArray modbus_slave_t::handle_write_single_register(quint16 addr, quint16 value)
{
    /* 设置寄存器值 */
    set_register(addr, value);

    /* 响应与请求相同 */
    QByteArray response;
    response.append(static_cast<char>(m_slave_address));
    response.append(static_cast<char>(MODBUS_FC_WRITE_SINGLE_REGISTER));
    response.append(static_cast<char>((addr >> 8) & 0xFF));
    response.append(static_cast<char>(addr & 0xFF));
    response.append(static_cast<char>((value >> 8) & 0xFF));
    response.append(static_cast<char>(value & 0xFF));

    quint16 crc = calc_crc16(response);
    response.append(static_cast<char>(crc & 0xFF));
    response.append(static_cast<char>((crc >> 8) & 0xFF));

    return response;
}

QByteArray modbus_slave_t::handle_write_multiple_registers(quint16 start_addr, const QByteArray &data)
{
    quint16 count = data.size() / 2;

    /* 写入寄存器 */
    for (quint16 i = 0; i < count; ++i) {
        quint16 value = (static_cast<quint8>(data[i * 2]) << 8) | static_cast<quint8>(data[i * 2 + 1]);
        set_register(start_addr + i, value);
    }

    /* 构建响应 */
    QByteArray response;
    response.append(static_cast<char>(m_slave_address));
    response.append(static_cast<char>(MODBUS_FC_WRITE_MULTIPLE_REGISTERS));
    response.append(static_cast<char>((start_addr >> 8) & 0xFF));
    response.append(static_cast<char>(start_addr & 0xFF));
    response.append(static_cast<char>((count >> 8) & 0xFF));
    response.append(static_cast<char>(count & 0xFF));

    quint16 crc = calc_crc16(response);
    response.append(static_cast<char>(crc & 0xFF));
    response.append(static_cast<char>((crc >> 8) & 0xFF));

    return response;
}

QByteArray modbus_slave_t::build_exception_response(quint8 function_code, quint8 exception_code)
{
    QByteArray response;
    response.append(static_cast<char>(m_slave_address));
    response.append(static_cast<char>(function_code | 0x80));
    response.append(static_cast<char>(exception_code));

    quint16 crc = calc_crc16(response);
    response.append(static_cast<char>(crc & 0xFF));
    response.append(static_cast<char>((crc >> 8) & 0xFF));

    return response;
}

quint16 modbus_slave_t::calc_crc16(const QByteArray &data)
{
    quint16 crc = 0xFFFF;

    for (int i = 0; i < data.size(); ++i) {
        crc ^= static_cast<quint8>(data[i]);
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

bool modbus_slave_t::verify_crc(const QByteArray &data)
{
    if (data.size() < 4) {
        return false;
    }

    QByteArray data_without_crc = data.left(data.size() - 2);
    quint16 calc_crc = calc_crc16(data_without_crc);
    quint16 recv_crc = static_cast<quint8>(data[data.size() - 2]) |
                       (static_cast<quint8>(data[data.size() - 1]) << 8);

    return calc_crc == recv_crc;
}
