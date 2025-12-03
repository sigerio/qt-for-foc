#ifndef UI_PANELS_TRANSFORM_EXPLANATION_PANEL_H
#define UI_PANELS_TRANSFORM_EXPLANATION_PANEL_H

#include <QGroupBox>
#include <QLabel>

class coordinate_widget;

// 坐标变换说明面板
class transform_explanation_panel : public QGroupBox {
    Q_OBJECT

public:
    explicit transform_explanation_panel(QWidget* parent = nullptr);
    ~transform_explanation_panel() override = default;

private:
    void setup_ui();

private:
    QLabel* m_clark_formula = nullptr;
    QLabel* m_park_formula = nullptr;
    coordinate_widget* m_coord_widget = nullptr;
};

#endif // UI_PANELS_TRANSFORM_EXPLANATION_PANEL_H
