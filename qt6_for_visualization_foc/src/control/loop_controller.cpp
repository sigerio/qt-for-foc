/**
 * @file loop_controller.cpp
 * @brief FOC三环级联控制器
 * 
 * 控制结构：位置环 -> 速度环 -> 电流环
 * PID参数设计基于IMC(内模控制)方法：
 * - 电流环: 带宽2kHz，Kp=Ld*ωc, Ki=Rs*ωc
 * - 速度环: 带宽100Hz（电流环1/20），保证稳定性
 * - 位置环: 带宽20Hz（速度环1/5），最外层控制
 */
#include "loop_controller.h"

loop_controller::loop_controller() {
    // 电流环PID（针对中型电机参数：Rs=0.3Ω, Ld=Lq=1mH）
    // IMC内模控制法：带宽fc=2kHz, ωc=12566 rad/s
    // Kp = Ld * ωc = 0.001 * 12566 = 12.57
    // Ki = Rs * ωc = 0.3 * 12566 = 3770
    pid_params_t current_pid{12.57, 3770.0, 0.0, 24.0, -24.0, 20.0};
    
    // 速度环PI（带宽fc=100Hz，电流环的1/20，稳定性优先）
    // Kt = 1.5 * psi_f * p = 1.5 * 0.15 * 4 = 0.9 N·m/A
    // ωc = 628 rad/s
    // Kp = J * ωc / Kt = 0.001 * 628 / 0.9 = 0.70
    // Ki = Kp * ωc / 5 = 0.70 * 628 / 5 = 88 (抑制稳态误差)
    pid_params_t vel_pid{0.7, 88.0, 0.0, 5.0, -5.0, 3.0};
    
    // 位置环PID（带宽fc=20Hz，速度环的1/5）
    // ωc = 125.7 rad/s
    // Kp = ωc = 125.7，Ki用于消除静差，Kd改善阻尼
    pid_params_t pos_pid{10.0, 0.5, 0.5, 100.0, -100.0, 30.0};

    m_id_pid.set_params(current_pid);
    m_iq_pid.set_params(current_pid);
    m_vel_pid.set_params(vel_pid);
    m_pos_pid.set_params(pos_pid);
}

void loop_controller::set_current_pid(const pid_params_t& id_pid, const pid_params_t& iq_pid) {
    m_id_pid.set_params(id_pid);
    m_iq_pid.set_params(iq_pid);
}

void loop_controller::set_velocity_pid(const pid_params_t& pid) {
    m_vel_pid.set_params(pid);
}

void loop_controller::set_position_pid(const pid_params_t& pid) {
    m_pos_pid.set_params(pid);
}

void loop_controller::reset() {
    m_id_pid.reset();
    m_iq_pid.reset();
    m_vel_pid.reset();
    m_pos_pid.reset();
}

void loop_controller::reset_to_default() {
    // 恢复默认PID参数（与构造函数一致）
    pid_params_t current_pid{12.57, 3770.0, 0.0, 24.0, -24.0, 20.0};
    pid_params_t vel_pid{0.7, 88.0, 0.0, 5.0, -5.0, 3.0};
    pid_params_t pos_pid{10.0, 0.5, 0.5, 100.0, -100.0, 30.0};

    m_id_pid.set_params(current_pid);
    m_iq_pid.set_params(current_pid);
    m_vel_pid.set_params(vel_pid);
    m_pos_pid.set_params(pos_pid);
    
    // 复位内部状态
    reset();
    
    // 恢复默认目标
    m_target = control_target_t{};
    m_target.vel_ref = 100.0;
    m_iq_ref_internal = 0.0;
    m_vel_ref_internal = 0.0;
}

// 三环级联控制计算
// 位置环 -> 速度环 -> 电流环
// 每个环的输出作为下一个环的输入
control_target_t loop_controller::calc(const motor_state_t& state, double dt) {
    control_target_t out;

    // 当前速度参考值（来自位置环或直接设定）
    double vel_ref = m_target.vel_ref;

    // 位置环：位置误差 -> 速度参考
    if (m_position_enabled) {
        vel_ref = m_pos_pid.calc(m_target.pos_ref, state.theta_e, dt);
    }

    // 当前iq参考值（来自速度环或直接设定）
    double iq_ref = m_target.iq_ref;

    // 速度环：速度误差 -> iq参考
    if (m_velocity_enabled) {
        iq_ref = m_vel_pid.calc(vel_ref, state.omega_m, dt);
    }
    
    // 保存内部参考值用于UI显示
    m_vel_ref_internal = vel_ref;
    m_iq_ref_internal = iq_ref;

    // 电流环：id/iq误差 -> ud/uq电压
    double ud = 0.0, uq = 0.0;
    if (m_current_enabled) {
        // id通常控制为0（表贴式PMSM）
        ud = m_id_pid.calc(m_target.id_ref, state.id, dt);
        uq = m_iq_pid.calc(iq_ref, state.iq, dt);
    }

    // 输出dq轴电压
    out.id_ref = ud;
    out.iq_ref = uq;
    out.vel_ref = vel_ref;
    out.pos_ref = m_target.pos_ref;

    return out;
}
