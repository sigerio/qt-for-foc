#ifndef UI_MOTOR_STATE_PANEL_H
#define UI_MOTOR_STATE_PANEL_H

#include <QWidget>

class motor_state_panel : public QWidget {
    Q_OBJECT
public:
    explicit motor_state_panel(QWidget* parent = nullptr);
public slots:
    void update_display();
};

#endif // UI_MOTOR_STATE_PANEL_H
