#ifndef UI_FOC_VISUALIZATION_WINDOW_H
#define UI_FOC_VISUALIZATION_WINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QStackedWidget>
#include "core/types.h"

class coord_transform_panel;
class svpwm_panel;
class motor_state_panel;
class motor_params_panel;
class step_indicator;
class hall_indicator;
class pid_config_panel;
class control_toolbar_panel;
class algorithm_explanation_window;
class waveform_window;

// FOC算法可视化主窗口
// 纵向排列：坐标变换 / SVPWM / 电机状态
class foc_visualization_window : public QMainWindow {
    Q_OBJECT

public:
    explicit foc_visualization_window(QWidget* parent = nullptr);
    ~foc_visualization_window() override;
    
    // 获取面板指针（用于信号连接）
    coord_transform_panel* coord_panel() { return m_coord_panel; }
    svpwm_panel* svpwm_panel_ptr() { return m_svpwm_panel; }
    motor_state_panel* motor_panel() { return m_motor_panel; }
    motor_params_panel* params_panel() { return m_params_panel; }
    step_indicator* step_ind() { return m_step_indicator; }
    hall_indicator* hall_ind() { return m_hall_indicator; }
    pid_config_panel* pid_panel() { return m_pid_panel; }
    control_toolbar_panel* toolbar_panel() { return m_toolbar_panel; }
    
    // 设置算法说明窗口引用（用于开关控制）
    void set_algo_window(algorithm_explanation_window* win) { m_algo_window = win; }
    // 设置波形窗口引用（用于开关控制）
    void set_wave_window(waveform_window* win) { m_wave_window = win; }

public slots:
    void update_display();
    // 切换控制模式（FOC/六步换向）
    void set_control_mode(e_control_mode mode);
    // 切换算法说明窗口显示
    void toggle_algo_window();
    // 切换波形窗口显示
    void toggle_wave_window();

private:
    void setup_ui();
    void setup_connections();

private:
    QSplitter* m_splitter = nullptr;
    coord_transform_panel* m_coord_panel = nullptr;
    svpwm_panel* m_svpwm_panel = nullptr;
    motor_state_panel* m_motor_panel = nullptr;
    motor_params_panel* m_params_panel = nullptr;
    step_indicator* m_step_indicator = nullptr;
    hall_indicator* m_hall_indicator = nullptr;
    QStackedWidget* m_indicator_stack = nullptr;
    pid_config_panel* m_pid_panel = nullptr;
    control_toolbar_panel* m_toolbar_panel = nullptr;
    algorithm_explanation_window* m_algo_window = nullptr;
    waveform_window* m_wave_window = nullptr;
};

#endif // UI_FOC_VISUALIZATION_WINDOW_H
