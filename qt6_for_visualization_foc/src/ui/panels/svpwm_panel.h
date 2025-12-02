#ifndef UI_SVPWM_PANEL_H
#define UI_SVPWM_PANEL_H

#include <QWidget>

class svpwm_panel : public QWidget {
    Q_OBJECT
public:
    explicit svpwm_panel(QWidget* parent = nullptr);
public slots:
    void update_display();
};

#endif // UI_SVPWM_PANEL_H
