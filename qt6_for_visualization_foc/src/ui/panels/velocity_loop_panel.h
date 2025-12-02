#ifndef UI_VELOCITY_LOOP_PANEL_H
#define UI_VELOCITY_LOOP_PANEL_H

#include <QWidget>

class velocity_loop_panel : public QWidget {
    Q_OBJECT
public:
    explicit velocity_loop_panel(QWidget* parent = nullptr);
public slots:
    void update_waveform();
};

#endif // UI_VELOCITY_LOOP_PANEL_H
