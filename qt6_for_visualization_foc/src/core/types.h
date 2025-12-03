#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <cstdint>
#include <cmath>

// 电机类型枚举
enum class e_motor_type {
    PMSM,       // 永磁同步电机
    BLDC,       // 无刷直流电机
    ACIM,       // 异步感应电机（预留）
    SRM         // 开关磁阻电机（预留）
};

// 控制模式枚举
enum class e_control_mode {
    FOC,        // 矢量控制
    SIX_STEP    // 六步换向
};

// 霍尔传感器状态结构
struct hall_state_t {
    bool ha = false;      // 霍尔A信号
    bool hb = false;      // 霍尔B信号
    bool hc = false;      // 霍尔C信号
    int sector = 1;       // 当前换向扇区(1-6)
    int hall_code = 0;    // 霍尔编码（3位二进制）
};

// FOC算法执行步骤枚举（用于步骤高亮）
enum class e_foc_step {
    IDLE,           // 空闲
    SAMPLING,       // 采样（读取电流、位置）
    CLARK,          // Clark变换（abc→αβ）
    PARK,           // Park变换（αβ→dq）
    CURRENT_LOOP,   // 电流环PI计算
    VELOCITY_LOOP,  // 速度环PI计算
    POSITION_LOOP,  // 位置环PID计算
    INV_PARK,       // 逆Park变换（dq→αβ）
    SVPWM,          // SVPWM计算
    OUTPUT,         // 输出PWM
    MOTOR_MODEL     // 电机模型更新
};

// 电机参数结构（snake_case命名规范）
struct motor_params_t {
    double rs         = 0.5;       // 定子电阻 (Ω)
    double ld         = 0.001;     // d轴电感 (H)
    double lq         = 0.001;     // q轴电感 (H)
    double psi_f      = 0.1;       // 永磁体磁链 (Wb)
    double j          = 0.0001;    // 转动惯量 (kg·m²)
    double b          = 0.0001;    // 阻尼系数 (N·m·s/rad)
    uint32_t pole_pairs = 4;       // 极对数
};

// 电机状态结构（snake_case命名规范）
struct motor_state_t {
    double theta_e   = 0.0;     // 电角度 (rad)
    double theta_m   = 0.0;     // 机械角度 (rad)
    double omega_e   = 0.0;     // 电角速度 (rad/s)
    double omega_m   = 0.0;     // 机械角速度 (rad/s)
    double id        = 0.0;     // d轴电流 (A)
    double iq        = 0.0;     // q轴电流 (A)
    double ud        = 0.0;     // d轴电压 (V)
    double uq        = 0.0;     // q轴电压 (V)
    double ia        = 0.0;     // a相电流 (A)
    double ib        = 0.0;     // b相电流 (A)
    double ic        = 0.0;     // c相电流 (A)
    double i_alpha   = 0.0;     // α轴电流 (A)
    double i_beta    = 0.0;     // β轴电流 (A)
    double u_alpha   = 0.0;     // α轴电压 (V)
    double u_beta    = 0.0;     // β轴电压 (V)
    double te        = 0.0;     // 电磁转矩 (N·m)
    double tl        = 0.0;     // 负载转矩 (N·m)
};

// 控制目标结构
struct control_target_t {
    double id_ref    = 0.0;     // d轴电流目标 (A)
    double iq_ref    = 0.0;     // q轴电流目标 (A)
    double vel_ref   = 0.0;     // 速度目标 (rad/s)
    double pos_ref   = 0.0;     // 位置目标 (rad)
};

// PID参数结构
struct pid_params_t {
    double kp           = 1.0;
    double ki           = 0.0;
    double kd           = 0.0;
    double out_max      = 100.0;
    double out_min      = -100.0;
    double integral_max = 50.0;
};

// 仿真配置
struct sim_config_t {
    double dt           = 100e-6;   // 仿真步长 (s): 默认100μs
    double speed_ratio  = 1.0;      // 仿真速度倍率
    bool running        = false;
    bool single_step    = false;
};

// SVPWM输出结构（snake_case命名规范）
struct svpwm_output_t {
    double ta       = 0.0;      // a相占空比
    double tb       = 0.0;      // b相占空比
    double tc       = 0.0;      // c相占空比
    int sector      = 1;        // 当前扇区(1-6)
    double mag      = 0.0;      // 矢量幅值
    double angle    = 0.0;      // 矢量角度 (rad)
};

// 常量定义
constexpr double PI = 3.14159265358979323846;
constexpr double TWO_PI = 2.0 * PI;
constexpr double SQRT3 = 1.7320508075688772;
constexpr double SQRT3_DIV_2 = SQRT3 / 2.0;
constexpr double ONE_DIV_SQRT3 = 1.0 / SQRT3;
constexpr double TWO_DIV_3 = 2.0 / 3.0;

#endif // CORE_TYPES_H
