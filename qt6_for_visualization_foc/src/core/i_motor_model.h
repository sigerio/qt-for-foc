#ifndef CORE_I_MOTOR_MODEL_H
#define CORE_I_MOTOR_MODEL_H

#include "types.h"

// 电机模型接口
class i_motor_model {
public:
    virtual ~i_motor_model() = default;

    // 设置电机参数
    virtual void set_params(const motor_params_t& params) = 0;

    // 设置dq轴电压输入
    virtual void set_voltage(double ud, double uq) = 0;

    // 设置负载转矩
    virtual void set_load_torque(double TL) = 0;

    // 执行一步仿真
    virtual void step(double dt) = 0;

    // 获取电机状态
    virtual motor_state_t get_state() const = 0;

    // 获取电机参数
    virtual motor_params_t get_params() const = 0;

    // 复位状态
    virtual void reset() = 0;
};

#endif // CORE_I_MOTOR_MODEL_H
