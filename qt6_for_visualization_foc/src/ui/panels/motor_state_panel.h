#ifndef UI_MOTOR_STATE_PANEL_H
#define UI_MOTOR_STATE_PANEL_H

#include <QWidget>
#include <QLabel>

class rotor_animator;
class waveform_view;

// 电机状态可视化面板
class motor_state_panel : public QWidget {
    Q_OBJECT
public:
    explicit motor_state_panel(QWidget* parent = nullptr);
    
public slots:
    // 更新转子角度
    void update_angle(double theta_rad);
    
    // 更新三相电流
    void update_phase_currents(double ia, double ib, double ic);
    
    // 更新电机状态数值
    void update_motor_state(double omega, double torque, double theta);
    
    void update_display();

private:
    rotor_animator* m_rotor;
    waveform_view* m_current_wave;
    
    QLabel* m_label_omega;
    QLabel* m_label_torque;
    QLabel* m_label_theta;
};

#endif // UI_MOTOR_STATE_PANEL_H
