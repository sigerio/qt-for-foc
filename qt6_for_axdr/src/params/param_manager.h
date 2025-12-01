/**
 * @file param_manager.h
 * @brief 参数管理器类声明
 * @note 负责参数与Modbus寄存器映射、读写、保存/加载
 */

#ifndef PARAM_MANAGER_H
#define PARAM_MANAGER_H

#include <QObject>
#include <QMap>
#include <QJsonObject>
#include "motor_params.h"
#include "serial/modbus_client.h"

/* ============== 寄存器地址定义 ============== */
/* PID参数寄存器地址 */
#define REG_ADDR_PID_CURRENT_KP     0x0000
#define REG_ADDR_PID_CURRENT_KI     0x0002
#define REG_ADDR_PID_CURRENT_KD     0x0004
#define REG_ADDR_PID_VELOCITY_KP    0x0006
#define REG_ADDR_PID_VELOCITY_KI    0x0008
#define REG_ADDR_PID_VELOCITY_KD    0x000A
#define REG_ADDR_PID_POSITION_KP    0x000C
#define REG_ADDR_PID_POSITION_KI    0x000E
#define REG_ADDR_PID_POSITION_KD    0x0010

/* 电机参数寄存器地址 */
#define REG_ADDR_POLE_PAIRS         0x0020
#define REG_ADDR_PHASE_RESISTANCE   0x0022
#define REG_ADDR_PHASE_INDUCTANCE   0x0024
#define REG_ADDR_TORQUE_CONSTANT    0x0026

/* 限制参数寄存器地址 */
#define REG_ADDR_CURRENT_LIMIT      0x0030
#define REG_ADDR_VELOCITY_LIMIT     0x0032
#define REG_ADDR_VOLTAGE_LIMIT      0x0034

/* 编码器参数寄存器地址 */
#define REG_ADDR_ENCODER_CPR        0x0040
#define REG_ADDR_ENCODER_OFFSET     0x0042

/* 保护参数寄存器地址 */
#define REG_ADDR_OVER_VOLTAGE       0x0050
#define REG_ADDR_UNDER_VOLTAGE      0x0052
#define REG_ADDR_OVER_TEMP          0x0054

/* 控制模式寄存器地址 */
#define REG_ADDR_CONTROL_MODE       0x0060

/* 实时数据寄存器地址（只读） */
#define REG_ADDR_RT_VELOCITY        0x0100
#define REG_ADDR_RT_POSITION        0x0102
#define REG_ADDR_RT_CURRENT_Q       0x0104
#define REG_ADDR_RT_CURRENT_D       0x0106
#define REG_ADDR_RT_VOLTAGE_BUS     0x0108
#define REG_ADDR_RT_TEMPERATURE     0x010A

/**
 * @brief 参数管理器类
 */
class param_manager_t : public QObject
{
    Q_OBJECT

public:
    explicit param_manager_t(modbus_client_t *client, QObject *parent = nullptr);
    ~param_manager_t();

    /* 参数读取 */
    void read_all_params();
    void read_pid_params();
    void read_motor_params();
    void read_limit_params();
    void read_realtime_data();

    /* 参数写入 */
    void write_pid_current(const pid_param_t &pid);
    void write_pid_velocity(const pid_param_t &pid);
    void write_pid_position(const pid_param_t &pid);
    void write_motor_params(const motor_physical_t &motor);
    void write_limit_params(const limit_param_t &limit);
    void write_control_mode(control_mode_E mode);

    /* 本地保存/加载 */
    bool save_to_file(const QString &file_path);
    bool load_from_file(const QString &file_path);

    /* 获取当前配置 */
    motor_config_t get_config() const;
    realtime_data_t get_realtime_data() const;

signals:
    void signal_pid_updated(const pid_config_t &pid);
    void signal_motor_updated(const motor_physical_t &motor);
    void signal_limit_updated(const limit_param_t &limit);
    void signal_realtime_updated(const realtime_data_t &data);
    void signal_config_loaded(const motor_config_t &config);
    void signal_error(const QString &msg);

private slots:
    void slot_on_read_completed(int start_addr, const QVector<quint16> &values);
    void slot_on_write_completed(int addr, bool success);

private:
    float registers_to_float(quint16 high, quint16 low);
    void float_to_registers(float value, quint16 &high, quint16 &low);
    QJsonObject config_to_json(const motor_config_t &config);
    motor_config_t json_to_config(const QJsonObject &json);

private:
    modbus_client_t *m_client;        /* Modbus客户端指针 */
    motor_config_t m_config;          /* 当前配置 */
    realtime_data_t m_realtime_data;  /* 实时数据 */
};

#endif /* PARAM_MANAGER_H */
