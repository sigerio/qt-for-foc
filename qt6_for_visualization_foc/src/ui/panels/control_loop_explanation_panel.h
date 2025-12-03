#ifndef UI_PANELS_CONTROL_LOOP_EXPLANATION_PANEL_H
#define UI_PANELS_CONTROL_LOOP_EXPLANATION_PANEL_H

#include <QGroupBox>
#include <QLabel>

class cascade_widget;

// 三环控制说明面板
class control_loop_explanation_panel : public QGroupBox {
    Q_OBJECT

public:
    explicit control_loop_explanation_panel(QWidget* parent = nullptr);
    ~control_loop_explanation_panel() override = default;

private:
    void setup_ui();

private:
    QLabel* m_pid_formula = nullptr;
    cascade_widget* m_structure_widget = nullptr;
};

#endif // UI_PANELS_CONTROL_LOOP_EXPLANATION_PANEL_H
