#ifndef CORE_PID_CONTROLLER_H
#define CORE_PID_CONTROLLER_H

#include "types.h"
#include "i_pid_controller.h"

// PID控制器实现
// 支持积分限幅和输出限幅
class pid_controller : public i_pid_controller {
public:
    pid_controller() = default;
    explicit pid_controller(const pid_params_t& params) : m_params(params) {}

    void set_params(const pid_params_t& params) override { m_params = params; }
    double calc(double target, double feedback, double dt) override;
    void reset() override { m_integral = 0.0; m_prev_error = 0.0; }
    pid_params_t get_params() const override { return m_params; }

    // 获取内部状态（用于调试）
    double get_integral() const { return m_integral; }
    double get_last_error() const { return m_prev_error; }

private:
    pid_params_t m_params;
    double m_integral = 0.0;
    double m_prev_error = 0.0;
};

#endif // CORE_PID_CONTROLLER_H
