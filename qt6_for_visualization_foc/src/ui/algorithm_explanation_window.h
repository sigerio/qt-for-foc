#ifndef UI_ALGORITHM_EXPLANATION_WINDOW_H
#define UI_ALGORITHM_EXPLANATION_WINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QVBoxLayout>

class motor_model_explanation_panel;
class transform_explanation_panel;
class svpwm_explanation_panel;
class control_loop_explanation_panel;

// 算法说明窗口
// 展示FOC算法的详细原理说明，包括电机模型、坐标变换、SVPWM和三环控制
class algorithm_explanation_window : public QMainWindow {
    Q_OBJECT

public:
    explicit algorithm_explanation_window(QWidget* parent = nullptr);
    ~algorithm_explanation_window() override;
    
    // 获取控制环说明面板
    control_loop_explanation_panel* control_panel() { return m_control_panel; }

private:
    void setup_ui();

private:
    QScrollArea* m_scroll_area = nullptr;
    QWidget* m_content_widget = nullptr;
    QVBoxLayout* m_main_layout = nullptr;
    
    motor_model_explanation_panel* m_motor_panel = nullptr;
    transform_explanation_panel* m_transform_panel = nullptr;
    svpwm_explanation_panel* m_svpwm_panel = nullptr;
    control_loop_explanation_panel* m_control_panel = nullptr;
};

#endif // UI_ALGORITHM_EXPLANATION_WINDOW_H
