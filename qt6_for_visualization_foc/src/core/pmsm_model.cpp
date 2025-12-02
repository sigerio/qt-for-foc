#include "pmsm_model.h"
#include <cmath>

pmsm_model::pmsm_model() {
    reset();
}

void pmsm_model::set_params(const motor_params_t& params) {
    m_params = params;
}

void pmsm_model::set_voltage(double ud, double uq) {
    m_state.ud = ud;
    m_state.uq = uq;
}

void pmsm_model::set_load_torque(double TL) {
    m_state.TL = TL;
}

// PMSM dq轴电压方程:
// ud = Rs*id + Ld*did/dt - omega_e*Lq*iq
// uq = Rs*iq + Lq*diq/dt + omega_e*(Ld*id + psi_f)
// 运动方程:
// Te = 1.5*p*(psi_f*iq + (Ld-Lq)*id*iq)
// J*domega_m/dt = Te - TL - B*omega_m
// omega_e = p * omega_m
// dtheta_e/dt = omega_e
void pmsm_model::step(double dt) {
    // 电流微分方程（欧拉法求解）
    // did/dt = (ud - Rs*id + omega_e*Lq*iq) / Ld
    // diq/dt = (uq - Rs*iq - omega_e*(Ld*id + psi_f)) / Lq
    double did_dt = (m_state.ud - m_params.Rs * m_state.id
                     + m_state.omega_e * m_params.Lq * m_state.iq) / m_params.Ld;
    double diq_dt = (m_state.uq - m_params.Rs * m_state.iq
                     - m_state.omega_e * (m_params.Ld * m_state.id + m_params.psi_f)) / m_params.Lq;

    // 更新电流
    m_state.id += did_dt * dt;
    m_state.iq += diq_dt * dt;

    // 计算电磁转矩
    calc_torque();

    // 运动方程：domega_m/dt = (Te - TL - B*omega_m) / J
    double domega_m_dt = (m_state.Te - m_state.TL - m_params.B * m_state.omega_m) / m_params.J;
    m_state.omega_m += domega_m_dt * dt;

    // 更新电角速度和电角度
    m_state.omega_e = m_params.p * m_state.omega_m;
    m_state.theta_e += m_state.omega_e * dt;

    // 角度归一化到[0, 2π)
    while (m_state.theta_e >= TWO_PI) m_state.theta_e -= TWO_PI;
    while (m_state.theta_e < 0) m_state.theta_e += TWO_PI;

    // 计算αβ电流和三相电流
    calc_alpha_beta_currents();
    calc_abc_currents();
}

motor_state_t pmsm_model::get_state() const {
    return m_state;
}

motor_params_t pmsm_model::get_params() const {
    return m_params;
}

void pmsm_model::reset() {
    m_state = motor_state_t{};
}

// 电磁转矩: Te = 1.5*p*(psi_f*iq + (Ld-Lq)*id*iq)
// 对于表贴式PMSM（Ld≈Lq），简化为: Te = 1.5*p*psi_f*iq
void pmsm_model::calc_torque() {
    double reluctance_torque = (m_params.Ld - m_params.Lq) * m_state.id * m_state.iq;
    m_state.Te = 1.5 * m_params.p * (m_params.psi_f * m_state.iq + reluctance_torque);
}

// 逆Park变换: dq -> αβ
void pmsm_model::calc_alpha_beta_currents() {
    double cos_t = std::cos(m_state.theta_e);
    double sin_t = std::sin(m_state.theta_e);
    m_state.i_alpha = m_state.id * cos_t - m_state.iq * sin_t;
    m_state.i_beta  = m_state.id * sin_t + m_state.iq * cos_t;

    // 同时计算αβ电压
    m_state.u_alpha = m_state.ud * cos_t - m_state.uq * sin_t;
    m_state.u_beta  = m_state.ud * sin_t + m_state.uq * cos_t;
}

// 逆Clark变换: αβ -> abc
void pmsm_model::calc_abc_currents() {
    m_state.ia = m_state.i_alpha;
    m_state.ib = -0.5 * m_state.i_alpha + SQRT3_DIV_2 * m_state.i_beta;
    m_state.ic = -0.5 * m_state.i_alpha - SQRT3_DIV_2 * m_state.i_beta;
}
