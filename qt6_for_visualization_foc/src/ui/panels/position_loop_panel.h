#ifndef UI_POSITION_LOOP_PANEL_H
#define UI_POSITION_LOOP_PANEL_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QDoubleSpinBox>

class waveform_view;

// 位置环控制面板
class position_loop_panel : public QWidget {
    Q_OBJECT
public:
    explicit position_loop_panel(QWidget* parent = nullptr);
    
    // 获取使能状态
    bool is_enabled() const;
    
public slots:
    // 更新位置波形
    void update_position(double pos_ref, double pos);
    
    void update_waveform();
    
signals:
    void pid_changed(double kp, double ki, double kd);
    void target_changed(double target);
    void loop_enabled_changed(bool enabled);

private slots:
    void on_slider_kp_changed(int val);
    void on_slider_ki_changed(int val);
    void on_slider_kd_changed(int val);
    void on_spin_kp_changed(double val);
    void on_spin_ki_changed(double val);
    void on_spin_kd_changed(double val);
    void on_target_changed(double val);
    void on_enable_changed(int state);

private:
    waveform_view* m_pos_wave;
    QSlider* m_slider_kp;
    QSlider* m_slider_ki;
    QSlider* m_slider_kd;
    QDoubleSpinBox* m_spin_kp;
    QDoubleSpinBox* m_spin_ki;
    QDoubleSpinBox* m_spin_kd;
    QDoubleSpinBox* m_spin_target;
    QCheckBox* m_check_enable;
};

#endif // UI_POSITION_LOOP_PANEL_H
