#ifndef UI_CURRENT_LOOP_PANEL_H
#define UI_CURRENT_LOOP_PANEL_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QDoubleSpinBox>

class waveform_view;

// 电流环控制面板
class current_loop_panel : public QWidget {
    Q_OBJECT
public:
    explicit current_loop_panel(QWidget* parent = nullptr);
    
    // 获取使能状态
    bool is_enabled() const;
    
public slots:
    // 更新Id/Iq波形
    void update_currents(double id_ref, double id, double iq_ref, double iq);
    
    void update_waveform();
    
signals:
    // PID参数变化信号
    void pid_changed(double kp, double ki);
    void loop_enabled_changed(bool enabled);

private slots:
    void on_slider_kp_changed(int val);
    void on_slider_ki_changed(int val);
    void on_spin_kp_changed(double val);
    void on_spin_ki_changed(double val);
    void on_enable_changed(int state);

private:
    waveform_view* m_id_wave;
    waveform_view* m_iq_wave;
    
    QSlider* m_slider_kp;
    QSlider* m_slider_ki;
    QDoubleSpinBox* m_spin_kp;
    QDoubleSpinBox* m_spin_ki;
    QCheckBox* m_check_enable;
};

#endif // UI_CURRENT_LOOP_PANEL_H
