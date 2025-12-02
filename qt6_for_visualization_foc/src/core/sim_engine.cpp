#include "sim_engine.h"
#include "control/loop_controller.h"

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
    m_step_index = 0;
    m_svpwm_out = svpwm_output_t{};
    emit state_updated(m_motor ? m_motor->get_state() : motor_state_t{});
}

void sim_engine::run_loop() {
    if (!m_config.running || !m_motor) return;

    // 每帧执行多步仿真以达到实时效果
    // 16ms内执行 16ms / dt * speed_ratio 步
    int steps_per_frame = static_cast<int>(0.016 / m_config.dt * m_config.speed_ratio);
    steps_per_frame = std::max(1, std::min(steps_per_frame, 10000));

    for (int i = 0; i < steps_per_frame && m_config.running; ++i) {
        execute_one_step();
    }

    // 发送状态更新
    emit state_updated(m_motor->get_state());
}

void sim_engine::execute_one_step() {
    if (!m_motor) return;
    
    bool detailed = m_config.single_step;  // 单步模式发送详细步骤

    // 设置默认负载转矩（中型电机负载）
    m_motor->set_load_torque(0.2);
    
    // 步骤1: 采样
    if (detailed) emit foc_step_changed(e_foc_step::SAMPLING);
    motor_state_t state = m_motor->get_state();

    // 步骤2-4: 控制环计算（包含Clark、Park变换）
    double ud = 0.0, uq = 0.0;
    if (m_loop_ctrl) {
        if (detailed) emit foc_step_changed(e_foc_step::POSITION_LOOP);
        if (detailed) emit foc_step_changed(e_foc_step::VELOCITY_LOOP);
        if (detailed) emit foc_step_changed(e_foc_step::CURRENT_LOOP);
        control_target_t target = m_loop_ctrl->calc(state, m_config.dt);
        ud = target.id_ref;
        uq = target.iq_ref;
    }

    // 步骤5: 逆Park变换
    if (detailed) emit foc_step_changed(e_foc_step::INV_PARK);
    m_motor->set_voltage(ud, uq);

    // 步骤6: SVPWM计算
    if (detailed) emit foc_step_changed(e_foc_step::SVPWM);
    state = m_motor->get_state();
    double u_alpha, u_beta;
    m_transform.inv_park(state.ud, state.uq, state.theta_e, u_alpha, u_beta);
    m_svpwm.calc(u_alpha, u_beta, m_udc,
                 m_svpwm_out.ta, m_svpwm_out.tb, m_svpwm_out.tc);
    m_svpwm_out.sector = m_svpwm.get_sector();
    m_svpwm.get_vector(m_svpwm_out.mag, m_svpwm_out.angle);

    // 步骤7: 电机模型更新
    if (detailed) emit foc_step_changed(e_foc_step::MOTOR_MODEL);
    m_motor->step(m_config.dt);

    // 步骤8: 输出
    if (detailed) emit foc_step_changed(e_foc_step::OUTPUT);

    m_step_index++;

    // 单步模式发送步骤完成信号（不清除高亮，保持最后步骤）
    if (m_config.single_step) {
        emit step_completed(m_step_index, QString("Step %1").arg(m_step_index));
        emit state_updated(m_motor->get_state());
        // 注意：不发送IDLE信号，保持最后一步的高亮状态
    }
}
