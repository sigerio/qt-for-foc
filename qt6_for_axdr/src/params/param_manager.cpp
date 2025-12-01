/**
 * @file param_manager.cpp
 * @brief 参数管理器类实现
 */

#include "param_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

param_manager_t::param_manager_t(modbus_client_t *client, QObject *parent)
    : QObject(parent)
    , m_client(client)
{
    /* 初始化配置为默认值 */
    memset(&m_config, 0, sizeof(motor_config_t));
    memset(&m_realtime_data, 0, sizeof(realtime_data_t));

    /* 连接信号 */
    connect(m_client, &modbus_client_t::signal_read_completed,
            this, &param_manager_t::slot_on_read_completed);
    connect(m_client, &modbus_client_t::signal_write_completed,
            this, &param_manager_t::slot_on_write_completed);
}

param_manager_t::~param_manager_t()
{
}

/* ============== 参数读取函数 ============== */

/**
 * @brief 读取所有参数
 * @note 依次读取PID、电机、限制参数
 */
void param_manager_t::read_all_params()
{
    read_pid_params();
    read_motor_params();
    read_limit_params();
}

void param_manager_t::read_pid_params()
{
    /* 读取所有PID参数 (18个寄存器: 3环 * 3参数 * 2寄存器) */
    m_client->read_holding_registers(REG_ADDR_PID_CURRENT_KP, 18);
}

void param_manager_t::read_motor_params()
{
    /* 读取电机物理参数 (8个寄存器) */
    m_client->read_holding_registers(REG_ADDR_POLE_PAIRS, 8);
}

void param_manager_t::read_limit_params()
{
    /* 读取限制参数 (6个寄存器) */
    m_client->read_holding_registers(REG_ADDR_CURRENT_LIMIT, 6);
}

void param_manager_t::read_realtime_data()
{
    /* 读取实时数据 (12个寄存器) */
    m_client->read_holding_registers(REG_ADDR_RT_VELOCITY, 12);
}

/* ============== 参数写入函数 ============== */

void param_manager_t::write_pid_current(const pid_param_t &pid)
{
    QVector<quint16> values;
    quint16 high, low;
    
    float_to_registers(pid.kp, high, low);
    values.append(high); values.append(low);
    float_to_registers(pid.ki, high, low);
    values.append(high); values.append(low);
    float_to_registers(pid.kd, high, low);
    values.append(high); values.append(low);
    
    m_client->write_holding_registers(REG_ADDR_PID_CURRENT_KP, values);
}

void param_manager_t::write_pid_velocity(const pid_param_t &pid)
{
    QVector<quint16> values;
    quint16 high, low;
    
    float_to_registers(pid.kp, high, low);
    values.append(high); values.append(low);
    float_to_registers(pid.ki, high, low);
    values.append(high); values.append(low);
    float_to_registers(pid.kd, high, low);
    values.append(high); values.append(low);
    
    m_client->write_holding_registers(REG_ADDR_PID_VELOCITY_KP, values);
}

void param_manager_t::write_pid_position(const pid_param_t &pid)
{
    QVector<quint16> values;
    quint16 high, low;
    
    float_to_registers(pid.kp, high, low);
    values.append(high); values.append(low);
    float_to_registers(pid.ki, high, low);
    values.append(high); values.append(low);
    float_to_registers(pid.kd, high, low);
    values.append(high); values.append(low);
    
    m_client->write_holding_registers(REG_ADDR_PID_POSITION_KP, values);
}

void param_manager_t::write_motor_params(const motor_physical_t &motor)
{
    QVector<quint16> values;
    quint16 high, low;
    
    values.append(motor.pole_pairs);
    values.append(0); /* 预留 */
    float_to_registers(motor.phase_resistance, high, low);
    values.append(high); values.append(low);
    float_to_registers(motor.phase_inductance, high, low);
    values.append(high); values.append(low);
    float_to_registers(motor.torque_constant, high, low);
    values.append(high); values.append(low);
    
    m_client->write_holding_registers(REG_ADDR_POLE_PAIRS, values);
}

void param_manager_t::write_limit_params(const limit_param_t &limit)
{
    QVector<quint16> values;
    quint16 high, low;
    
    float_to_registers(limit.current_limit, high, low);
    values.append(high); values.append(low);
    float_to_registers(limit.velocity_limit, high, low);
    values.append(high); values.append(low);
    float_to_registers(limit.voltage_limit, high, low);
    values.append(high); values.append(low);
    
    m_client->write_holding_registers(REG_ADDR_CURRENT_LIMIT, values);
}

void param_manager_t::write_control_mode(control_mode_E mode)
{
    m_client->write_holding_register(REG_ADDR_CONTROL_MODE, (quint16)mode);
}

/* ============== 获取函数 ============== */

motor_config_t param_manager_t::get_config() const
{
    return m_config;
}

realtime_data_t param_manager_t::get_realtime_data() const
{
    return m_realtime_data;
}

/* ============== 本地保存/加载 ============== */

bool param_manager_t::save_to_file(const QString &file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly)) {
        emit signal_error(QString("无法打开文件: %1").arg(file_path));
        return false;
    }
    
    QJsonObject json = config_to_json(m_config);
    QJsonDocument doc(json);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool param_manager_t::load_from_file(const QString &file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        emit signal_error(QString("无法打开文件: %1").arg(file_path));
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        emit signal_error("JSON解析失败");
        return false;
    }
    
    m_config = json_to_config(doc.object());
    emit signal_config_loaded(m_config);
    return true;
}

/* ============== 槽函数 ============== */

void param_manager_t::slot_on_read_completed(int start_addr, const QVector<quint16> &values)
{
    /* 根据起始地址解析数据 */
    if (start_addr == REG_ADDR_PID_CURRENT_KP && values.size() >= 18) {
        /* PID参数 */
        int idx = 0;
        m_config.pid.current.kp = registers_to_float(values[idx], values[idx+1]); idx += 2;
        m_config.pid.current.ki = registers_to_float(values[idx], values[idx+1]); idx += 2;
        m_config.pid.current.kd = registers_to_float(values[idx], values[idx+1]); idx += 2;
        m_config.pid.velocity.kp = registers_to_float(values[idx], values[idx+1]); idx += 2;
        m_config.pid.velocity.ki = registers_to_float(values[idx], values[idx+1]); idx += 2;
        m_config.pid.velocity.kd = registers_to_float(values[idx], values[idx+1]); idx += 2;
        m_config.pid.position.kp = registers_to_float(values[idx], values[idx+1]); idx += 2;
        m_config.pid.position.ki = registers_to_float(values[idx], values[idx+1]); idx += 2;
        m_config.pid.position.kd = registers_to_float(values[idx], values[idx+1]);
        emit signal_pid_updated(m_config.pid);
    }
    else if (start_addr == REG_ADDR_POLE_PAIRS && values.size() >= 8) {
        /* 电机参数 */
        m_config.motor.pole_pairs = values[0];
        m_config.motor.phase_resistance = registers_to_float(values[2], values[3]);
        m_config.motor.phase_inductance = registers_to_float(values[4], values[5]);
        m_config.motor.torque_constant = registers_to_float(values[6], values[7]);
        emit signal_motor_updated(m_config.motor);
    }
    else if (start_addr == REG_ADDR_CURRENT_LIMIT && values.size() >= 6) {
        /* 限制参数 */
        m_config.limit.current_limit = registers_to_float(values[0], values[1]);
        m_config.limit.velocity_limit = registers_to_float(values[2], values[3]);
        m_config.limit.voltage_limit = registers_to_float(values[4], values[5]);
        emit signal_limit_updated(m_config.limit);
    }
    else if (start_addr == REG_ADDR_RT_VELOCITY && values.size() >= 12) {
        /* 实时数据 */
        m_realtime_data.velocity = registers_to_float(values[0], values[1]);
        m_realtime_data.position = registers_to_float(values[2], values[3]);
        m_realtime_data.current_q = registers_to_float(values[4], values[5]);
        m_realtime_data.current_d = registers_to_float(values[6], values[7]);
        m_realtime_data.voltage_bus = registers_to_float(values[8], values[9]);
        m_realtime_data.temperature = registers_to_float(values[10], values[11]);
        emit signal_realtime_updated(m_realtime_data);
    }
}

void param_manager_t::slot_on_write_completed(int addr, bool success)
{
    if (!success) {
        emit signal_error(QString("写入地址 0x%1 失败").arg(addr, 4, 16, QChar('0')));
    }
}

/* ============== 辅助函数 ============== */

/**
 * @brief 将两个16位寄存器转换为float
 * @param high 高16位寄存器值
 * @param low 低16位寄存器值
 * @return 转换后的float值
 * @note 使用大端序（高位在前）
 */
float param_manager_t::registers_to_float(quint16 high, quint16 low)
{
    /* 将两个16位寄存器合并为32位float (大端序) */
    quint32 combined = ((quint32)high << 16) | low;
    float result;
    memcpy(&result, &combined, sizeof(float));
    return result;
}

/**
 * @brief 将float转换为两个16位寄存器值
 * @param value 要转换的float值
 * @param high 输出高16位寄存器值
 * @param low 输出低16位寄存器值
 * @note 使用大端序（高位在前）
 */
void param_manager_t::float_to_registers(float value, quint16 &high, quint16 &low)
{
    /* 将float拆分为两个16位寄存器 (大端序) */
    quint32 combined;
    memcpy(&combined, &value, sizeof(float));
    high = (quint16)(combined >> 16);
    low = (quint16)(combined & 0xFFFF);
}

QJsonObject param_manager_t::config_to_json(const motor_config_t &config)
{
    QJsonObject json;
    
    /* PID参数 */
    QJsonObject pid_json;
    QJsonObject current_pid;
    current_pid["kp"] = config.pid.current.kp;
    current_pid["ki"] = config.pid.current.ki;
    current_pid["kd"] = config.pid.current.kd;
    pid_json["current"] = current_pid;
    
    QJsonObject velocity_pid;
    velocity_pid["kp"] = config.pid.velocity.kp;
    velocity_pid["ki"] = config.pid.velocity.ki;
    velocity_pid["kd"] = config.pid.velocity.kd;
    pid_json["velocity"] = velocity_pid;
    
    QJsonObject position_pid;
    position_pid["kp"] = config.pid.position.kp;
    position_pid["ki"] = config.pid.position.ki;
    position_pid["kd"] = config.pid.position.kd;
    pid_json["position"] = position_pid;
    json["pid"] = pid_json;
    
    /* 电机参数 */
    QJsonObject motor_json;
    motor_json["pole_pairs"] = config.motor.pole_pairs;
    motor_json["phase_resistance"] = config.motor.phase_resistance;
    motor_json["phase_inductance"] = config.motor.phase_inductance;
    motor_json["torque_constant"] = config.motor.torque_constant;
    json["motor"] = motor_json;
    
    /* 限制参数 */
    QJsonObject limit_json;
    limit_json["current_limit"] = config.limit.current_limit;
    limit_json["velocity_limit"] = config.limit.velocity_limit;
    limit_json["voltage_limit"] = config.limit.voltage_limit;
    json["limit"] = limit_json;
    
    /* 控制模式 */
    json["control_mode"] = (int)config.control_mode;
    
    return json;
}

motor_config_t param_manager_t::json_to_config(const QJsonObject &json)
{
    motor_config_t config;
    memset(&config, 0, sizeof(motor_config_t));
    
    /* PID参数 */
    if (json.contains("pid")) {
        QJsonObject pid_json = json["pid"].toObject();
        if (pid_json.contains("current")) {
            QJsonObject c = pid_json["current"].toObject();
            config.pid.current.kp = c["kp"].toDouble();
            config.pid.current.ki = c["ki"].toDouble();
            config.pid.current.kd = c["kd"].toDouble();
        }
        if (pid_json.contains("velocity")) {
            QJsonObject v = pid_json["velocity"].toObject();
            config.pid.velocity.kp = v["kp"].toDouble();
            config.pid.velocity.ki = v["ki"].toDouble();
            config.pid.velocity.kd = v["kd"].toDouble();
        }
        if (pid_json.contains("position")) {
            QJsonObject p = pid_json["position"].toObject();
            config.pid.position.kp = p["kp"].toDouble();
            config.pid.position.ki = p["ki"].toDouble();
            config.pid.position.kd = p["kd"].toDouble();
        }
    }
    
    /* 电机参数 */
    if (json.contains("motor")) {
        QJsonObject m = json["motor"].toObject();
        config.motor.pole_pairs = m["pole_pairs"].toInt();
        config.motor.phase_resistance = m["phase_resistance"].toDouble();
        config.motor.phase_inductance = m["phase_inductance"].toDouble();
        config.motor.torque_constant = m["torque_constant"].toDouble();
    }
    
    /* 限制参数 */
    if (json.contains("limit")) {
        QJsonObject l = json["limit"].toObject();
        config.limit.current_limit = l["current_limit"].toDouble();
        config.limit.velocity_limit = l["velocity_limit"].toDouble();
        config.limit.voltage_limit = l["voltage_limit"].toDouble();
    }
    
    /* 控制模式 */
    config.control_mode = (control_mode_E)json["control_mode"].toInt();
    
    return config;
}
