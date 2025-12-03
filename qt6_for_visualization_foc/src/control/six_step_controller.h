#ifndef CONTROL_SIX_STEP_CONTROLLER_H
#define CONTROL_SIX_STEP_CONTROLLER_H

#include "core/types.h"

// BLDC六步换向控制器
// 根据转子位置（霍尔信号或反电动势过零检测）确定导通相
class six_step_controller {
public:
    six_step_controller();
    
    // 根据电角度计算换向扇区(1-6)
    int calc_sector(double theta_e) const;
    
    // 根据扇区和占空比计算三相电压
    // 返回：ua, ub, uc（归一化电压，乘以Udc得到实际电压）
    void calc_phase_voltage(int sector, double duty, 
                            double& ua, double& ub, double& uc) const;
    
    // 设置占空比（0-1）
    void set_duty(double duty) { m_duty = duty; }
    double get_duty() const { return m_duty; }
    
    // 根据电机状态计算控制输出
    // 输入：当前电角度、目标速度、实际速度
    // 输出：三相占空比
    void calc(double theta_e, double vel_ref, double vel_actual, double dt,
              double& duty_a, double& duty_b, double& duty_c);
    
    // 设置速度环PI参数
    void set_speed_pid(double kp, double ki) { m_kp = kp; m_ki = ki; }
    
    // 复位
    void reset();

private:
    double m_duty = 0.5;      // 当前占空比
    double m_kp = 0.01;       // 速度环Kp
    double m_ki = 0.1;        // 速度环Ki
    double m_integral = 0.0;  // 积分项
    double m_integral_max = 0.8;
};

#endif // CONTROL_SIX_STEP_CONTROLLER_H
