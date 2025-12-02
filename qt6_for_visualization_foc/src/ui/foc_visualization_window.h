#ifndef UI_FOC_VISUALIZATION_WINDOW_H
#define UI_FOC_VISUALIZATION_WINDOW_H

#include <QMainWindow>
#include <QSplitter>

class coord_transform_panel;
class svpwm_panel;
class motor_state_panel;

// FOC算法可视化主窗口
// 纵向排列：坐标变换 / SVPWM / 电机状态
class foc_visualization_window : public QMainWindow {
    Q_OBJECT

public:
    explicit foc_visualization_window(QWidget* parent = nullptr);
    ~foc_visualization_window() override;

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
};

#endif // UI_FOC_VISUALIZATION_WINDOW_H
