#ifndef CORE_BLDC_MODEL_H
#define CORE_BLDC_MODEL_H

#include "i_motor_model.h"
#include "types.h"

// BLDC无刷直流电机模型
// 采用梯形波反电动势模型，支持FOC矢量控制
class bldc_model : public i_motor_model {
public:
    bldc_model();

    void set_params(const motor_params_t& params) override;
    void set_voltage(double ud, double uq) override;
    void set_load_torque(double tl) override;
    void step(double dt) override;
    motor_state_t get_state() const override;
    motor_params_t get_params() const override;
    void reset() override;

private:
    // 计算梯形波反电动势系数（基于电角度）
    double calc_bemf_coeff(double theta_e, int phase) const;
    
    // 计算电磁转矩
    void calc_torque();
    
    // 计算αβ电流
    void calc_alpha_beta_currents();
    
    // 计算abc相电流
    void calc_abc_currents();

    motor_params_t m_params;
    motor_state_t m_state;
    
    // 梯形波参数
    double m_flat_top = 2.094;  // 平顶区角度（120°=2π/3）
};

#endif // CORE_BLDC_MODEL_H
