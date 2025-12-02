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

    motor_state_t state = m_motor->get_state();

    // 1. 获取控制输出
    double ud = 0.0, uq = 0.0;
    if (m_loop_ctrl) {
        control_target_t target = m_loop_ctrl->calc(state, m_config.dt);
        ud = target.id_ref;  // 简化：直接使用参考值作为电压
        uq = target.iq_ref;
    }

    // 2. 设置电机电压
    m_motor->set_voltage(ud, uq);

    // 3. 执行电机模型仿真
    m_motor->step(m_config.dt);

    // 4. 获取新状态，计算SVPWM
    state = m_motor->get_state();
    double u_alpha, u_beta;
    m_transform.inv_park(state.ud, state.uq, state.theta_e, u_alpha, u_beta);
    m_svpwm.calc(u_alpha, u_beta, m_udc,
                 m_svpwm_out.Ta, m_svpwm_out.Tb, m_svpwm_out.Tc);
    m_svpwm_out.sector = m_svpwm.get_sector();
    m_svpwm.get_vector(m_svpwm_out.mag, m_svpwm_out.angle);

    m_step_index++;

    // 单步模式发送步骤完成信号
    if (m_config.single_step) {
        emit step_completed(m_step_index, QString("Step %1").arg(m_step_index));
        emit state_updated(state);
    }
}
