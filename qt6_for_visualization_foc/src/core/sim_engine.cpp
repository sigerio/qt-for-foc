#include "sim_engine.h"
#include "control/loop_controller.h"
#include <cmath>

sim_engine::sim_engine(QObject* parent) : QObject(parent) {
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &sim_engine::run_loop);
}

sim_engine::~sim_engine() {
    stop();
}

void sim_engine::set_config(const sim_config_t& cfg) {
    m_config = cfg;
    // 根据仿真步长和速度倍率计算定时器间隔
    // UI刷新频率约60Hz，每次定时器触发执行多步仿真
    int interval_ms = 16;  // ~60Hz
    m_timer->setInterval(interval_ms);
}

void sim_engine::set_motor_model(std::unique_ptr<i_motor_model> model) {
    m_motor = std::move(model);
}

void sim_engine::start() {
    if (!m_motor) return;
    m_config.running = true;
    m_timer->start();
}

void sim_engine::stop() {
    m_config.running = false;
    m_timer->stop();
}

void sim_engine::pause() {
    m_config.running = false;
    m_timer->stop();
}

void sim_engine::step() {
    if (!m_motor) return;
    m_config.single_step = true;
    execute_one_step();
    m_config.single_step = false;
}

void sim_engine::reset() {
    stop();
    if (m_motor) {
        m_motor->reset();
    }
    if (m_six_step_ctrl) {
        m_six_step_ctrl->reset();
    }
    m_step_index = 0;
    m_svpwm_out = svpwm_output_t{};
    m_hall_state = hall_state_t{};
    emit state_updated(m_motor ? m_motor->get_state() : motor_state_t{});
}

void sim_engine::run_loop() {
    if (!m_config.running || !m_motor) return;

    // 每帧执行多步仿真以达到实时效果
    // 16ms内执行 16ms / dt * speed_ratio 步
    int steps_per_frame = static_cast<int>(0.016 / m_config.dt * m_config.speed_ratio);
    steps_per_frame = std::max(1, std::min(steps_per_frame, 10000));
    
    // 每帧发射多次状态更新以避免波形欠采样
    // 目标: 每帧至少8个采样点，确保波形平滑
    int emit_interval = std::max(1, steps_per_frame / 8);

    for (int i = 0; i < steps_per_frame && m_config.running; ++i) {
        execute_one_step();
        // 每隔emit_interval步发射一次波形数据
        if ((i + 1) % emit_interval == 0) {
            emit state_updated(m_motor->get_state());
        }
    }
}

void sim_engine::execute_one_step() {
    if (!m_motor) return;
    
    bool detailed = m_config.single_step;  // 单步模式发送详细步骤

    // 设置负载转矩
    m_motor->set_load_torque(m_load_torque);
    
    // 获取当前状态
    motor_state_t state = m_motor->get_state();

    // 根据控制模式执行不同的控制算法
    if (m_control_mode == e_control_mode::SIX_STEP) {
        execute_six_step(state, detailed);
    } else {
        execute_foc_step(state, detailed);
    }

    // 步骤7: 电机模型更新
    m_motor->step(m_config.dt);

    m_step_index++;

    // 单步模式：显示SVPWM步骤（矢量调制是FOC的核心可视化步骤）
    if (m_config.single_step) {
        emit foc_step_changed(e_foc_step::SVPWM);
        emit step_completed(m_step_index, QString("Step %1").arg(m_step_index));
        emit state_updated(m_motor->get_state());
    }
}

// FOC矢量控制执行
void sim_engine::execute_foc_step(motor_state_t& state, bool /*detailed*/) {
    // 控制环计算（包含Clark、Park变换）
    double ud = 0.0, uq = 0.0;
    if (m_loop_ctrl) {
        control_target_t target = m_loop_ctrl->calc(state, m_config.dt);
        ud = target.id_ref;
        uq = target.iq_ref;
    }

    // 步骤5: 逆Park变换
    m_motor->set_voltage(ud, uq);

    // 步骤6: SVPWM计算
    state = m_motor->get_state();
    double u_alpha, u_beta;
    m_transform.inv_park(state.ud, state.uq, state.theta_e, u_alpha, u_beta);
    m_svpwm.calc(u_alpha, u_beta, m_udc,
                 m_svpwm_out.ta, m_svpwm_out.tb, m_svpwm_out.tc);
    m_svpwm_out.sector = m_svpwm.get_sector();
    m_svpwm.get_vector(m_svpwm_out.mag, m_svpwm_out.angle);
}

// 六步换向控制执行
void sim_engine::execute_six_step(motor_state_t& state, bool /*detailed*/) {
    if (!m_six_step_ctrl) return;

    // 计算霍尔传感器状态
    calc_hall_state(state.theta_e);
    emit hall_state_changed(m_hall_state);

    // 获取速度目标（从loop_controller获取，如果有的话）
    double vel_ref = 100.0;  // 默认速度目标
    if (m_loop_ctrl) {
        vel_ref = m_loop_ctrl->get_target().vel_ref;
    }

    // 六步换向控制计算
    double duty_a, duty_b, duty_c;
    m_six_step_ctrl->calc(state.theta_e, vel_ref, state.omega_m, m_config.dt,
                          duty_a, duty_b, duty_c);

    // 将三相占空比转换为dq轴电压（简化处理）
    // 实际上六步换向直接控制相电压，这里做近似转换用于电机模型
    double ua = (duty_a - 0.5) * m_udc;
    double ub = (duty_b - 0.5) * m_udc;
    double uc = (duty_c - 0.5) * m_udc;

    // Clark变换: abc -> αβ
    double u_alpha, u_beta;
    m_transform.clark(ua, ub, uc, u_alpha, u_beta);

    // Park变换: αβ -> dq
    double ud, uq;
    m_transform.park(u_alpha, u_beta, state.theta_e, ud, uq);

    m_motor->set_voltage(ud, uq);

    // 更新SVPWM输出（用于显示）
    m_svpwm_out.ta = duty_a;
    m_svpwm_out.tb = duty_b;
    m_svpwm_out.tc = duty_c;
    m_svpwm_out.sector = m_hall_state.sector;
    m_svpwm_out.mag = std::sqrt(u_alpha * u_alpha + u_beta * u_beta);
    m_svpwm_out.angle = std::atan2(u_beta, u_alpha);
}

// 霍尔传感器状态查表（扇区1-6对应的Ha,Hb,Hc状态和编码）
static constexpr struct { bool ha, hb, hc; int code; } HALL_TABLE[6] = {
    {true,  false, true,  5},  // 扇区1 (0-60°)
    {true,  false, false, 4},  // 扇区2 (60-120°)
    {true,  true,  false, 6},  // 扇区3 (120-180°)
    {false, true,  false, 2},  // 扇区4 (180-240°)
    {false, true,  true,  3},  // 扇区5 (240-300°)
    {false, false, true,  1},  // 扇区6 (300-360°)
};

void sim_engine::calc_hall_state(double theta_e) {
    // 归一化角度到[0, 2π)并计算扇区索引(0-5)
    theta_e = std::fmod(theta_e, TWO_PI);
    if (theta_e < 0) theta_e += TWO_PI;
    int idx = std::clamp(static_cast<int>(theta_e * 3.0 / PI), 0, 5);
    
    // 查表获取霍尔状态
    const auto& h = HALL_TABLE[idx];
    m_hall_state.sector = idx + 1;
    m_hall_state.ha = h.ha;
    m_hall_state.hb = h.hb;
    m_hall_state.hc = h.hc;
    m_hall_state.hall_code = h.code;
}
