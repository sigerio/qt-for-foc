#ifndef UI_CURRENT_LOOP_PANEL_H
#define UI_CURRENT_LOOP_PANEL_H

#include <QWidget>

class current_loop_panel : public QWidget {
    Q_OBJECT
public:
    explicit current_loop_panel(QWidget* parent = nullptr);
public slots:
    void update_waveform();
};

#endif // UI_CURRENT_LOOP_PANEL_H
