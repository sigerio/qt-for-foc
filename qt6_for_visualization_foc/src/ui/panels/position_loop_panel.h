#ifndef UI_POSITION_LOOP_PANEL_H
#define UI_POSITION_LOOP_PANEL_H

#include <QWidget>

class position_loop_panel : public QWidget {
    Q_OBJECT
public:
    explicit position_loop_panel(QWidget* parent = nullptr);
public slots:
    void update_waveform();
};

#endif // UI_POSITION_LOOP_PANEL_H
