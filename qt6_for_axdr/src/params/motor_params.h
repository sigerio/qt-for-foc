/**
 * @file motor_params.h
 * @brief 电机参数数据结构定义
 */

#ifndef MOTOR_PARAMS_H
#define MOTOR_PARAMS_H

#include <QtGlobal>
#include <QString>

/* ============== 控制模式枚举 ============== */
typedef enum {
    e_mode_torque = 0,    /* 力矩控制模式 */
    e_mode_velocity,      /* 速度控制模式 */
    e_mode_position       /* 位置控制模式 */
} control_mode_E;

/* ============== PID参数结构体 ============== */
typedef struct {
    float kp;             /* 比例系数 */
    float ki;             /* 积分系数 */
    float kd;             /* 微分系数 */
} pid_param_t;

/* ============== 三环PID参数结构体 ============== */
typedef struct {
    pid_param_t current;  /* 电流环PID */
    pid_param_t velocity; /* 速度环PID */
    pid_param_t position; /* 位置环PID */
} pid_config_t;

/* ============== 电机物理参数结构体 ============== */
typedef struct {
    quint16 pole_pairs;       /* 电机极对数 */
    float phase_resistance;   /* 相电阻 (Ω) */
    float phase_inductance;   /* 相电感 (H) */
    float torque_constant;    /* 转矩常数 (Nm/A) */
} motor_physical_t;

/* ============== 限制参数结构体 ============== */
typedef struct {
    float current_limit;      /* 电流限制 (A) */
    float velocity_limit;     /* 速度限制 (rad/s) */
    float voltage_limit;      /* 电压限制 (V) */
} limit_param_t;

/* ============== 编码器参数结构体 ============== */
typedef struct {
    quint32 cpr;              /* 每转脉冲数 */
    float offset;             /* 零点偏移 */
} encoder_param_t;

/* ============== 保护参数结构体 ============== */
typedef struct {
    float over_voltage;       /* 过压保护阈值 (V) */
    float under_voltage;      /* 欠压保护阈值 (V) */
    float over_temp;          /* 过温保护阈值 (℃) */
} protection_param_t;

/* ============== 实时数据结构体 ============== */
typedef struct {
    float velocity;           /* 当前速度 (rad/s) */
    float position;           /* 当前位置 (rad) */
    float current_q;          /* Q轴电流 (A) */
    float current_d;          /* D轴电流 (A) */
    float voltage_bus;        /* 母线电压 (V) */
    float temperature;        /* 温度 (℃) */
} realtime_data_t;

/* ============== 完整电机配置结构体 ============== */
typedef struct {
    pid_config_t pid;              /* PID参数 */
    motor_physical_t motor;        /* 电机物理参数 */
    limit_param_t limit;           /* 限制参数 */
    encoder_param_t encoder;       /* 编码器参数 */
    protection_param_t protection; /* 保护参数 */
    control_mode_E control_mode;   /* 控制模式 */
} motor_config_t;

#endif /* MOTOR_PARAMS_H */
