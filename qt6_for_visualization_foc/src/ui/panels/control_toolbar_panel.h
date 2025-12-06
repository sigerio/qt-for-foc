#ifndef UI_CONTROL_TOOLBAR_PANEL_H
#define UI_CONTROL_TOOLBAR_PANEL_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

// 控制工具栏面板
// 包含运行控制按钮、电机类型选择、控制模式选择、仿真速度等
class control_toolbar_panel : public QWidget {
    Q_OBJECT
public:
    explicit control_toolbar_panel(QWidget* parent = nullptr);
    
    // 获取运行状态
    bool is_running() const { return m_running; }

signals:
    // 运行状态变化
    void run_state_changed(bool running);
    // 复位请求
    void reset_requested();
    // 单步请求
    void step_requested();
    // 电机类型变化 (0=PMSM, 1=BLDC)
    void motor_type_changed(int type);
    // 控制模式变化 (0=FOC, 1=六步换向)
    void control_mode_changed(int mode);
    // 仿真速度变化
    void speed_ratio_changed(double ratio);
    // 配置加载请求
    void config_load_requested(const QString& path);
    // 恢复默认配置
    void config_reset_requested();
    // 算法说明窗口开关
    void toggle_algo_window();
    // 波形窗口开关
    void toggle_wave_window();

private slots:
    void on_run_clicked();
    void on_step_clicked();
    void on_reset_clicked();
    void on_motor_type_changed(int index);
    void on_control_mode_changed(int index);
    void on_speed_changed(int index);
    void on_load_config_clicked();
    void on_reset_config_clicked();
    void on_toggle_algo_clicked();
    void on_toggle_wave_clicked();

private:
    void setup_ui();

private:
    QPushButton* m_btn_run = nullptr;
    QPushButton* m_btn_step = nullptr;
    QPushButton* m_btn_reset = nullptr;
    QComboBox* m_combo_motor = nullptr;
    QComboBox* m_combo_mode = nullptr;
    QComboBox* m_combo_speed = nullptr;
    QPushButton* m_btn_load_config = nullptr;
    QPushButton* m_btn_reset_config = nullptr;
    QPushButton* m_btn_toggle_algo = nullptr;
    QPushButton* m_btn_toggle_wave = nullptr;
    QLabel* m_label_status = nullptr;
    bool m_running = false;
};

#endif // UI_CONTROL_TOOLBAR_PANEL_H
