#ifndef UI_VELOCITY_LOOP_PANEL_H
#define UI_VELOCITY_LOOP_PANEL_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QDoubleSpinBox>

class waveform_view;

// 速度环控制面板
class velocity_loop_panel : public QWidget {
    Q_OBJECT
public:
    explicit velocity_loop_panel(QWidget* parent = nullptr);
    
public slots:
    // 更新速度波形
    void update_velocity(double vel_ref, double vel);
    
    void update_waveform();
    
signals:
    void pid_changed(double kp, double ki);
    void target_changed(double target);
    void loop_enabled_changed(bool enabled);

private slots:
    void on_slider_kp_changed(int val);
    void on_slider_ki_changed(int val);
    void on_spin_kp_changed(double val);
    void on_spin_ki_changed(double val);
    void on_target_changed(double val);
    void on_enable_changed(int state);

private:
    waveform_view* m_vel_wave;
    QSlider* m_slider_kp;
    QSlider* m_slider_ki;
    QDoubleSpinBox* m_spin_kp;
    QDoubleSpinBox* m_spin_ki;
    QDoubleSpinBox* m_spin_target;
    QCheckBox* m_check_enable;
};

#endif // UI_VELOCITY_LOOP_PANEL_H
