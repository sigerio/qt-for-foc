#ifndef UI_CONTROL_LOOP_WINDOW_H
#define UI_CONTROL_LOOP_WINDOW_H

#include <QMainWindow>
#include <QSplitter>

class current_loop_panel;
class velocity_loop_panel;
class position_loop_panel;

// 三环控制窗口
// 纵向排列：电流环 / 速度环 / 位置环
class control_loop_window : public QMainWindow {
    Q_OBJECT

public:
    explicit control_loop_window(QWidget* parent = nullptr);
    ~control_loop_window() override;

public slots:
    void update_waveform();

private:
    void setup_ui();
    void setup_connections();

private:
    QSplitter* m_splitter = nullptr;
    current_loop_panel* m_current_panel = nullptr;
    velocity_loop_panel* m_velocity_panel = nullptr;
    position_loop_panel* m_position_panel = nullptr;
};

#endif // UI_CONTROL_LOOP_WINDOW_H
