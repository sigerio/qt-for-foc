#ifndef CORE_I_PID_CONTROLLER_H
#define CORE_I_PID_CONTROLLER_H

#include "types.h"

// PID控制器接口
class i_pid_controller {
public:
    virtual ~i_pid_controller() = default;

    // 设置PID参数
    virtual void set_params(const pid_params_t& params) = 0;

    // 计算PID输出
    virtual double calc(double target, double feedback, double dt) = 0;

    // 复位控制器状态
    virtual void reset() = 0;

    // 获取当前参数
    virtual pid_params_t get_params() const = 0;
};

#endif // CORE_I_PID_CONTROLLER_H
