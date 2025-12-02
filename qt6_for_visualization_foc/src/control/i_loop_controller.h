#ifndef CONTROL_I_LOOP_CONTROLLER_H
#define CONTROL_I_LOOP_CONTROLLER_H

#include "core/types.h"

// 三环控制器接口
class i_loop_controller {
public:
    virtual ~i_loop_controller() = default;

    // 环路启用/禁用
    virtual void set_current_loop_enabled(bool en) = 0;
    virtual void set_velocity_loop_enabled(bool en) = 0;
    virtual void set_position_loop_enabled(bool en) = 0;

    // 设置PID参数
    virtual void set_current_pid(const pid_params_t& id_pid, const pid_params_t& iq_pid) = 0;
    virtual void set_velocity_pid(const pid_params_t& pid) = 0;
    virtual void set_position_pid(const pid_params_t& pid) = 0;

    // 设置目标值
    virtual void set_target(const control_target_t& target) = 0;

    // 计算控制输出
    virtual control_target_t calc(const motor_state_t& state, double dt) = 0;

    // 复位
    virtual void reset() = 0;
};

#endif // CONTROL_I_LOOP_CONTROLLER_H
