#ifndef UI_WAVEFORM_WINDOW_H
#define UI_WAVEFORM_WINDOW_H

#include <QMainWindow>
#include <QScrollArea>

class waveform_view;
class resizable_group;

// 波形显示窗口
// 显示FOC算法相关的所有波形数据
class waveform_window : public QMainWindow {
    Q_OBJECT
public:
    explicit waveform_window(QWidget* parent = nullptr);
    ~waveform_window() override = default;

public slots:
    // 更新电流波形
    void update_currents(double ia, double ib, double ic,
                        double i_alpha, double i_beta,
                        double id, double iq,
                        double id_ref, double iq_ref);
    
    // 更新电压波形
    void update_voltages(double ud, double uq,
                        double u_alpha, double u_beta,
                        double ua, double ub, double uc);
    
    // 更新PWM占空比
    void update_pwm(double ta, double tb, double tc);
    
    // 更新速度波形
    void update_velocity(double vel_ref, double velocity);
    
    // 更新位置波形
    void update_position(double pos_ref, double position);

private:
    void setup_ui();

private:
    QScrollArea* m_scroll_area = nullptr;
    QWidget* m_content = nullptr;
    
    // 第1行：dq电压电流（两列）
    waveform_view* m_wave_ud_uq = nullptr;      // Ud, Uq
    waveform_view* m_wave_id_iq = nullptr;      // Id, Iq
    // 第2行：电流参考 | 速度 | 位置（三列）
    waveform_view* m_wave_i_ref = nullptr;      // id_ref, iq_ref
    waveform_view* m_wave_vel = nullptr;        // vel_ref, velocity
    waveform_view* m_wave_pos = nullptr;        // pos_ref, position
    // 第3行：αβ电压
    waveform_view* m_wave_u_ab = nullptr;       // Uα, Uβ
    // 第4行：αβ电流
    waveform_view* m_wave_i_ab = nullptr;       // Iα, Iβ
    // 第5行：三相电压
    waveform_view* m_wave_u_abc = nullptr;      // Ua, Ub, Uc
    // 第6行：三相电流
    waveform_view* m_wave_i_abc = nullptr;      // Ia, Ib, Ic
    // 第7行：PWM占空比
    waveform_view* m_wave_pwm = nullptr;        // Ta, Tb, Tc
};

#endif // UI_WAVEFORM_WINDOW_H
