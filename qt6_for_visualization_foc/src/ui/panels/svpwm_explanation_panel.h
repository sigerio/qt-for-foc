#ifndef UI_PANELS_SVPWM_EXPLANATION_PANEL_H
#define UI_PANELS_SVPWM_EXPLANATION_PANEL_H

#include <QGroupBox>
#include <QLabel>

class hexagon_widget;

// SVPWM算法说明面板
class svpwm_explanation_panel : public QGroupBox {
    Q_OBJECT

public:
    explicit svpwm_explanation_panel(QWidget* parent = nullptr);
    ~svpwm_explanation_panel() override = default;

private:
    void setup_ui();

private:
    QLabel* m_sector_formula = nullptr;
    QLabel* m_duty_formula = nullptr;
    hexagon_widget* m_hexagon_widget = nullptr;
};

#endif // UI_PANELS_SVPWM_EXPLANATION_PANEL_H
