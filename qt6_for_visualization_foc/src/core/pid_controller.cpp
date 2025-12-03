#include "pid_controller.h"
#include <algorithm>

double pid_controller::calc(double target, double feedback, double dt) {
    double error = target - feedback;
    m_integral += error * dt;
    m_integral = std::clamp(m_integral, -m_params.integral_max, m_params.integral_max);
    double derivative = (error - m_prev_error) / dt;
    m_prev_error = error;
    double output = m_params.kp * error + m_params.ki * m_integral + m_params.kd * derivative;
    return std::clamp(output, m_params.out_min, m_params.out_max);
}
