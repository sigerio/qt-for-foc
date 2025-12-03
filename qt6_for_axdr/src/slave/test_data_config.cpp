/**
 * @file test_data_config.cpp
 * @brief 测试数据配置管理类实现
 */

#include "test_data_config.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>

test_data_config_t::test_data_config_t(QObject *parent)
    : QObject(parent)
{
}

test_data_config_t::~test_data_config_t()
{
}

bool test_data_config_t::load_config(const QString &file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        emit signal_error_occurred(QString("无法打开配置文件: %1").arg(file_path));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parse_error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parse_error);
    if (parse_error.error != QJsonParseError::NoError) {
        emit signal_error_occurred(QString("JSON解析错误: %1").arg(parse_error.errorString()));
        return false;
    }

    if (!doc.isObject()) {
        emit signal_error_occurred("配置文件格式错误: 根元素必须是对象");
        return false;
    }

    QJsonObject root = doc.object();
    QJsonObject registers_obj = root["registers"].toObject();

    m_registers.clear();
    for (auto it = registers_obj.begin(); it != registers_obj.end(); ++it) {
        QString addr_str = it.key();
        bool ok;
        quint16 address = addr_str.toUInt(&ok, 0);
        if (!ok) {
            continue;
        }

        QJsonObject reg_obj = it.value().toObject();
        register_info_t info;
        info.name = reg_obj["name"].toString();
        info.value = static_cast<quint16>(reg_obj["value"].toInt());
        m_registers[address] = info;
    }

    emit signal_config_loaded(file_path);
    return true;
}

bool test_data_config_t::save_config(const QString &file_path)
{
    QJsonObject root;
    QJsonObject registers_obj;

    for (auto it = m_registers.begin(); it != m_registers.end(); ++it) {
        QString addr_str = QString("0x%1").arg(it.key(), 4, 16, QChar('0')).toUpper();
        QJsonObject reg_obj;
        reg_obj["name"] = it.value().name;
        reg_obj["value"] = it.value().value;
        registers_obj[addr_str] = reg_obj;
    }

    root["registers"] = registers_obj;

    QJsonDocument doc(root);
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly)) {
        emit signal_error_occurred(QString("无法保存配置文件: %1").arg(file_path));
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    emit signal_config_saved(file_path);
    return true;
}

void test_data_config_t::load_default_config()
{
    m_registers.clear();

    /* 
     * 寄存器映射与主机一致，每个float参数占用2个连续寄存器
     * 小端序: reg[n]=低16位, reg[n+1]=高16位
     */

    /* PID参数 - 电流环 (地址与param_manager.h一致) */
    m_registers[0x0000] = {"Kp_current_L", 0};      /* 0x0000-0x0001 */
    m_registers[0x0001] = {"Kp_current_H", 0};
    m_registers[0x0002] = {"Ki_current_L", 0};      /* 0x0002-0x0003 */
    m_registers[0x0003] = {"Ki_current_H", 0};
    m_registers[0x0004] = {"Kd_current_L", 0};      /* 0x0004-0x0005 */
    m_registers[0x0005] = {"Kd_current_H", 0};

    /* PID参数 - 速度环 */
    m_registers[0x0006] = {"Kp_velocity_L", 0};     /* 0x0006-0x0007 */
    m_registers[0x0007] = {"Kp_velocity_H", 0};
    m_registers[0x0008] = {"Ki_velocity_L", 0};     /* 0x0008-0x0009 */
    m_registers[0x0009] = {"Ki_velocity_H", 0};
    m_registers[0x000A] = {"Kd_velocity_L", 0};     /* 0x000A-0x000B */
    m_registers[0x000B] = {"Kd_velocity_H", 0};

    /* PID参数 - 位置环 */
    m_registers[0x000C] = {"Kp_position_L", 0};     /* 0x000C-0x000D */
    m_registers[0x000D] = {"Kp_position_H", 0};
    m_registers[0x000E] = {"Ki_position_L", 0};     /* 0x000E-0x000F */
    m_registers[0x000F] = {"Ki_position_H", 0};
    m_registers[0x0010] = {"Kd_position_L", 0};     /* 0x0010-0x0011 */
    m_registers[0x0011] = {"Kd_position_H", 0};

    /* 电机参数 */
    m_registers[0x0020] = {"pole_pairs", 7};        /* 整型，单寄存器 */
    m_registers[0x0021] = {"reserved_H", 0};        /* 预留，跳过显示 */
    m_registers[0x0022] = {"phase_resistance_L", 0};/* 0x0022-0x0023 */
    m_registers[0x0023] = {"phase_resistance_H", 0};
    m_registers[0x0024] = {"phase_inductance_L", 0};/* 0x0024-0x0025 */
    m_registers[0x0025] = {"phase_inductance_H", 0};
    m_registers[0x0026] = {"torque_constant_L", 0}; /* 0x0026-0x0027 */
    m_registers[0x0027] = {"torque_constant_H", 0};

    /* 限制参数 */
    m_registers[0x0030] = {"current_limit_L", 0};   /* 0x0030-0x0031 */
    m_registers[0x0031] = {"current_limit_H", 0};
    m_registers[0x0032] = {"velocity_limit_L", 0};  /* 0x0032-0x0033 */
    m_registers[0x0033] = {"velocity_limit_H", 0};
    m_registers[0x0034] = {"voltage_limit_L", 0};   /* 0x0034-0x0035 */
    m_registers[0x0035] = {"voltage_limit_H", 0};

    /* 编码器参数 */
    m_registers[0x0040] = {"encoder_cpr_L", 0};     /* 0x0040-0x0041 */
    m_registers[0x0041] = {"encoder_cpr_H", 0};
    m_registers[0x0042] = {"encoder_offset_L", 0};  /* 0x0042-0x0043 */
    m_registers[0x0043] = {"encoder_offset_H", 0};

    /* 保护参数 */
    m_registers[0x0050] = {"over_voltage_L", 0};    /* 0x0050-0x0051 */
    m_registers[0x0051] = {"over_voltage_H", 0};
    m_registers[0x0052] = {"under_voltage_L", 0};   /* 0x0052-0x0053 */
    m_registers[0x0053] = {"under_voltage_H", 0};
    m_registers[0x0054] = {"over_temp_L", 0};       /* 0x0054-0x0055 */
    m_registers[0x0055] = {"over_temp_H", 0};

    /* 控制模式 (单寄存器) */
    m_registers[0x0060] = {"control_mode", 1};

    /* 实时数据 */
    m_registers[0x0100] = {"velocity_L", 0};        /* 0x0100-0x0101 */
    m_registers[0x0101] = {"velocity_H", 0};
    m_registers[0x0102] = {"position_L", 0};        /* 0x0102-0x0103 */
    m_registers[0x0103] = {"position_H", 0};
    m_registers[0x0104] = {"current_q_L", 0};       /* 0x0104-0x0105 */
    m_registers[0x0105] = {"current_q_H", 0};
    m_registers[0x0106] = {"current_d_L", 0};       /* 0x0106-0x0107 */
    m_registers[0x0107] = {"current_d_H", 0};
    m_registers[0x0108] = {"voltage_bus_L", 0};     /* 0x0108-0x0109 */
    m_registers[0x0109] = {"voltage_bus_H", 0};
    m_registers[0x010A] = {"temperature_L", 0};     /* 0x010A-0x010B */
    m_registers[0x010B] = {"temperature_H", 0};
}

QMap<quint16, register_info_t> test_data_config_t::get_registers() const
{
    return m_registers;
}

void test_data_config_t::apply_to_slave(modbus_slave_t *slave)
{
    if (!slave) {
        return;
    }

    slave->clear_registers();
    for (auto it = m_registers.begin(); it != m_registers.end(); ++it) {
        slave->set_register_with_name(it.key(), it.value().name, it.value().value);
    }
}

void test_data_config_t::load_from_slave(modbus_slave_t *slave)
{
    if (!slave) {
        return;
    }

    m_registers = slave->get_all_registers();
}

QString test_data_config_t::get_default_config_path()
{
    QString config_dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir(config_dir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return config_dir + "/slave_config.json";
}
