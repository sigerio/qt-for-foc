#include "bldc_model.h"
#include <cmath>

bldc_model::bldc_model() {
    reset();
}

void bldc_model::set_params(const motor_params_t& params) {
    m_params = params;
}

void bldc_model::set_voltage(double ud, double uq) {
    m_state.ud = ud;
    m_state.uq = uq;
}

void bldc_model::set_load_torque(double tl) {
    m_state.tl = tl;
}

// 梯形波反电动势系数计算
// 返回值范围[-1, 1]，表示该相反电动势相对于最大值的比例
double bldc_model::calc_bemf_coeff(double theta_e, int phase) const {
    // 相位偏移：A相0°，B相-120°，C相-240°
    double offset = phase * TWO_PI / 3.0;
    double angle = std::fmod(theta_e - offset + TWO_PI, TWO_PI);
    
    // 梯形波：平顶120°，斜坡60°
    // [0, π/3): 上升沿 0->1
    // [π/3, 2π/3): 平顶 1
    // [2π/3, π): 下降沿 1->0
    // [π, 4π/3): 下降沿 0->-1
    // [4π/3, 5π/3): 平底 -1
    // [5π/3, 2π): 上升沿 -1->0
    
    double slope = PI / 3.0;  // 60°
    
    if (angle < slope) {
        return angle / slope;
    } else if (angle < 2.0 * slope) {
        return 1.0;
    } else if (angle < 3.0 * slope) {
        return 1.0 - (angle - 2.0 * slope) / slope;
    } else if (angle < 4.0 * slope) {
        return -(angle - 3.0 * slope) / slope;
    } else if (angle < 5.0 * slope) {
        return -1.0;
    } else {
        return -1.0 + (angle - 5.0 * slope) / slope;
    }
}

// BLDC电机仿真步进
// 使用dq轴模型，但反电动势采用梯形波
void bldc_model::step(double dt) {
    // 电流微分方程（简化模型，使用等效电感）
    // 假设Ld≈Lq=L，Rs为定子电阻
    double L = (m_params.ld + m_params.lq) / 2.0;
    
    // dq轴电流方程
    double did_dt = (m_state.ud - m_params.rs * m_state.id
                     + m_state.omega_e * L * m_state.iq) / L;
    double diq_dt = (m_state.uq - m_params.rs * m_state.iq
                     - m_state.omega_e * (L * m_state.id + m_params.psi_f)) / L;
    
    // 更新电流
    m_state.id += did_dt * dt;
    m_state.iq += diq_dt * dt;
    
    // 计算电磁转矩
    calc_torque();
    
    // 运动方程
    double domega_m_dt = (m_state.te - m_state.tl - m_params.b * m_state.omega_m) / m_params.j;
    m_state.omega_m += domega_m_dt * dt;
    
    // 更新电角速度和角度
    m_state.omega_e = m_params.pole_pairs * m_state.omega_m;
    m_state.theta_e += m_state.omega_e * dt;
    m_state.theta_m = m_state.theta_e / m_params.pole_pairs;
    
    // 角度归一化
    while (m_state.theta_e >= TWO_PI) m_state.theta_e -= TWO_PI;
    while (m_state.theta_e < 0) m_state.theta_e += TWO_PI;
    
    // 计算αβ和abc电流
    calc_alpha_beta_currents();
    calc_abc_currents();
}

motor_state_t bldc_model::get_state() const {
    return m_state;
}

motor_params_t bldc_model::get_params() const {
    return m_params;
}

void bldc_model::reset() {
    m_state = motor_state_t{};
}

// BLDC电磁转矩计算
// Te = Kt * iq，其中Kt = 1.5 * p * psi_f
void bldc_model::calc_torque() {
    m_state.te = 1.5 * m_params.pole_pairs * m_params.psi_f * m_state.iq;
}

// 逆Park变换
void bldc_model::calc_alpha_beta_currents() {
    double cos_t = std::cos(m_state.theta_e);
    double sin_t = std::sin(m_state.theta_e);
    m_state.i_alpha = m_state.id * cos_t - m_state.iq * sin_t;
    m_state.i_beta  = m_state.id * sin_t + m_state.iq * cos_t;
    
    m_state.u_alpha = m_state.ud * cos_t - m_state.uq * sin_t;
    m_state.u_beta  = m_state.ud * sin_t + m_state.uq * cos_t;
}

// 逆Clark变换
void bldc_model::calc_abc_currents() {
    // 三相电流
    m_state.ia = m_state.i_alpha;
    m_state.ib = -0.5 * m_state.i_alpha + SQRT3_DIV_2 * m_state.i_beta;
    m_state.ic = -0.5 * m_state.i_alpha - SQRT3_DIV_2 * m_state.i_beta;
    
    // 三相电压（从αβ电压逆Clark变换）
    m_state.ua = m_state.u_alpha;
    m_state.ub = -0.5 * m_state.u_alpha + SQRT3_DIV_2 * m_state.u_beta;
    m_state.uc = -0.5 * m_state.u_alpha - SQRT3_DIV_2 * m_state.u_beta;
}
