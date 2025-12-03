#include "six_step_controller.h"
#include <cmath>
#include <algorithm>

six_step_controller::six_step_controller() {
    reset();
}

// 六步换向扇区划分（每60°一个扇区）
// 扇区1: 0°-60°    扇区2: 60°-120°   扇区3: 120°-180°
// 扇区4: 180°-240° 扇区5: 240°-300°  扇区6: 300°-360°
int six_step_controller::calc_sector(double theta_e) const {
    // 归一化到[0, 2π)
    while (theta_e < 0) theta_e += TWO_PI;
    while (theta_e >= TWO_PI) theta_e -= TWO_PI;
    
    // 每60°一个扇区
    double sector_angle = PI / 3.0;
    int sector = static_cast<int>(theta_e / sector_angle) + 1;
    return std::clamp(sector, 1, 6);
}

// 六步换向相电压表（120°导通方式）
// 扇区  A相  B相  C相
//   1    +    -    0
//   2    +    0    -
//   3    0    +    -
//   4    -    +    0
//   5    -    0    +
//   6    0    -    +
void six_step_controller::calc_phase_voltage(int sector, double duty,
                                              double& ua, double& ub, double& uc) const {
    ua = ub = uc = 0.0;
    
    switch (sector) {
        case 1: ua = duty; ub = -duty; uc = 0; break;
        case 2: ua = duty; ub = 0; uc = -duty; break;
        case 3: ua = 0; ub = duty; uc = -duty; break;
        case 4: ua = -duty; ub = duty; uc = 0; break;
        case 5: ua = -duty; ub = 0; uc = duty; break;
        case 6: ua = 0; ub = -duty; uc = duty; break;
        default: break;
    }
}

void six_step_controller::calc(double theta_e, double vel_ref, double vel_actual, double dt,
                                double& duty_a, double& duty_b, double& duty_c) {
    // 速度环PI控制
    double error = vel_ref - vel_actual;
    m_integral += error * dt;
    m_integral = std::clamp(m_integral, -m_integral_max, m_integral_max);
    
    double output = m_kp * error + m_ki * m_integral;
    m_duty = std::clamp(output, 0.0, 1.0);
    
    // 根据电角度确定扇区
    int sector = calc_sector(theta_e);
    
    // 计算三相占空比
    calc_phase_voltage(sector, m_duty, duty_a, duty_b, duty_c);
    
    // 转换为PWM占空比（0-1范围）
    // 负电压用0表示，正电压用duty表示，悬空用0.5表示
    duty_a = (duty_a > 0) ? m_duty : ((duty_a < 0) ? 0.0 : 0.5);
    duty_b = (duty_b > 0) ? m_duty : ((duty_b < 0) ? 0.0 : 0.5);
    duty_c = (duty_c > 0) ? m_duty : ((duty_c < 0) ? 0.0 : 0.5);
}

void six_step_controller::reset() {
    m_duty = 0.5;
    m_integral = 0.0;
}
