#ifndef UI_CONTROL_LOOP_WINDOW_H
#define UI_CONTROL_LOOP_WINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

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
    
    // 获取运行状态
    bool is_running() const { return m_running; }
    
    // 获取面板指针（用于信号连接）
    current_loop_panel* current_panel() { return m_current_panel; }
    velocity_loop_panel* velocity_panel() { return m_velocity_panel; }
    position_loop_panel* position_panel() { return m_position_panel; }

public slots:
    void update_waveform();
    
signals:
    // 运行状态变化信号
    void run_state_changed(bool running);
    // 复位信号
    void reset_requested();
    // 单步运行信号
    void step_requested();
    // 电机类型变化信号
    void motor_type_changed(int type);
    // 控制模式变化信号（0=FOC, 1=六步换向）
    void control_mode_changed(int mode);
    // 仿真速度变化信号
    void speed_ratio_changed(double ratio);
    // 配置加载信号（传递JSON文件路径）
    void config_load_requested(const QString& path);
    // 恢复默认配置信号
    void config_reset_requested();
    // 环路使能状态变化信号（preset: current/current_velocity/full）
    void loop_preset_changed(const QString& preset_name);

private slots:
    void on_run_clicked();
    void on_reset_clicked();
    void on_step_clicked();
    void on_motor_type_changed(int index);
    void on_control_mode_changed(int index);
    void on_speed_ratio_changed(int index);
    void on_load_config_clicked();
    void on_reset_config_clicked();
    void on_loop_enable_changed();  // 环路使能状态变化

private:
    void setup_ui();
    void setup_toolbar();
    void setup_connections();

private:
    QSplitter* m_splitter = nullptr;
    current_loop_panel* m_current_panel = nullptr;
    velocity_loop_panel* m_velocity_panel = nullptr;
    position_loop_panel* m_position_panel = nullptr;
    
    // 工具栏控件
    QPushButton* m_btn_run = nullptr;
    QPushButton* m_btn_step = nullptr;
    QPushButton* m_btn_reset = nullptr;
    QPushButton* m_btn_load_config = nullptr;
    QPushButton* m_btn_reset_config = nullptr;
    QComboBox* m_combo_motor = nullptr;
    QComboBox* m_combo_mode = nullptr;
    QComboBox* m_combo_speed = nullptr;
    QLabel* m_label_status = nullptr;
    bool m_running = false;
};

#endif // UI_CONTROL_LOOP_WINDOW_H
