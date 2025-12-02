#ifndef CONTROL_LOOP_CONTROLLER_H
#define CONTROL_LOOP_CONTROLLER_H

#include "core/types.h"
#include "core/pid_controller.h"
#include "i_loop_controller.h"

// 三环控制器实现
// 支持电流环、速度环、位置环级联控制
class loop_controller : public i_loop_controller {
public:
    loop_controller();

    // 环路启用/禁用
    void set_current_loop_enabled(bool en) override { m_current_enabled = en; }
    void set_velocity_loop_enabled(bool en) override { m_velocity_enabled = en; }
    void set_position_loop_enabled(bool en) override { m_position_enabled = en; }

    bool is_current_loop_enabled() const { return m_current_enabled; }
    bool is_velocity_loop_enabled() const { return m_velocity_enabled; }
    bool is_position_loop_enabled() const { return m_position_enabled; }

    // 设置PID参数
    void set_current_pid(const pid_params_t& id_pid, const pid_params_t& iq_pid) override;
    void set_velocity_pid(const pid_params_t& pid) override;
    void set_position_pid(const pid_params_t& pid) override;

    // 获取PID参数
    pid_params_t get_id_pid() const { return m_id_pid.get_params(); }
    pid_params_t get_iq_pid() const { return m_iq_pid.get_params(); }
    pid_params_t get_vel_pid() const { return m_vel_pid.get_params(); }
    pid_params_t get_pos_pid() const { return m_pos_pid.get_params(); }

    // 设置目标值
    void set_target(const control_target_t& target) override { m_target = target; }
    control_target_t get_target() const { return m_target; }

    // 计算控制输出
    control_target_t calc(const motor_state_t& state, double dt) override;

    // 复位所有控制器
    void reset() override;

private:
    bool m_current_enabled = true;
    bool m_velocity_enabled = true;
    bool m_position_enabled = false;

    pid_controller m_id_pid;
    pid_controller m_iq_pid;
    pid_controller m_vel_pid;
    pid_controller m_pos_pid;

    control_target_t m_target;
};

#endif // CONTROL_LOOP_CONTROLLER_H
