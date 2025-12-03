#ifndef UI_PANELS_MOTOR_MODEL_EXPLANATION_PANEL_H
#define UI_PANELS_MOTOR_MODEL_EXPLANATION_PANEL_H

#include <QGroupBox>
#include <QTableWidget>
#include <QLabel>

class circuit_widget;

// 电机模型说明面板
class motor_model_explanation_panel : public QGroupBox {
    Q_OBJECT

public:
    explicit motor_model_explanation_panel(QWidget* parent = nullptr);
    ~motor_model_explanation_panel() override = default;

private:
    void setup_ui();

private:
    QTableWidget* m_param_table = nullptr;
    QLabel* m_description = nullptr;
    circuit_widget* m_circuit_widget = nullptr;
};

#endif // UI_PANELS_MOTOR_MODEL_EXPLANATION_PANEL_H
