#ifndef CORE_PMSM_MODEL_H
#define CORE_PMSM_MODEL_H

#include "types.h"
#include "i_motor_model.h"

// PMSM永磁同步电机数学模型
// 基于dq轴旋转坐标系建模
class pmsm_model : public i_motor_model {
public:
    pmsm_model();
    ~pmsm_model() override = default;

    // i_motor_model接口实现
    void set_params(const motor_params_t& params) override;
    void set_voltage(double ud, double uq) override;
    void set_load_torque(double TL) override;
    void step(double dt) override;
    motor_state_t get_state() const override;
    motor_params_t get_params() const override;
    void reset() override;

private:
    // 计算电磁转矩
    void calc_torque();

    // 计算三相电流
    void calc_abc_currents();

    // 计算αβ电流
    void calc_alpha_beta_currents();

    motor_params_t m_params;
    motor_state_t m_state;
};

#endif // CORE_PMSM_MODEL_H
