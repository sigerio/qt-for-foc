#ifndef UI_FOC_VISUALIZATION_WINDOW_H
#define UI_FOC_VISUALIZATION_WINDOW_H

#include <QMainWindow>
#include <QSplitter>

class coord_transform_panel;
class svpwm_panel;
class motor_state_panel;
class step_indicator;

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
    step_indicator* step_ind() { return m_step_indicator; }

public slots:
    void update_display();

private:
    void setup_ui();
    void setup_connections();

private:
    QSplitter* m_splitter = nullptr;
    coord_transform_panel* m_coord_panel = nullptr;
    svpwm_panel* m_svpwm_panel = nullptr;
    motor_state_panel* m_motor_panel = nullptr;
    step_indicator* m_step_indicator = nullptr;
};

#endif // UI_FOC_VISUALIZATION_WINDOW_H
