#ifndef UI_CURRENT_LOOP_PANEL_H
#define UI_CURRENT_LOOP_PANEL_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>

class waveform_view;

// 电流环控制面板
class current_loop_panel : public QWidget {
    Q_OBJECT
public:
    explicit current_loop_panel(QWidget* parent = nullptr);
    
public slots:
    // 更新Id/Iq波形
    void update_currents(double id_ref, double id, double iq_ref, double iq);
    
    void update_waveform();
    
signals:
    // PID参数变化信号
    void pid_changed(double kp, double ki);
    void loop_enabled_changed(bool enabled);

private slots:
    void on_slider_changed();
    void on_enable_changed(int state);

private:
    waveform_view* m_id_wave;
    waveform_view* m_iq_wave;
    
    QSlider* m_slider_kp;
    QSlider* m_slider_ki;
    QLabel* m_label_kp;
    QLabel* m_label_ki;
    QCheckBox* m_check_enable;
};

#endif // UI_CURRENT_LOOP_PANEL_H
