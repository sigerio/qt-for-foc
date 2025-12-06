#ifndef UI_PID_CONFIG_PANEL_H
#define UI_PID_CONFIG_PANEL_H

#include <QWidget>
#include <QGroupBox>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QCheckBox>

// 三环PID参数配置面板
// 集中管理电流环、速度环、位置环的PID参数和使能状态
class pid_config_panel : public QWidget {
    Q_OBJECT
public:
    explicit pid_config_panel(QWidget* parent = nullptr);
    
    // 获取各环路使能状态
    bool is_current_enabled() const;
    bool is_velocity_enabled() const;
    bool is_position_enabled() const;

signals:
    // 电流环PID参数变化
    void current_pid_changed(double kp, double ki);
    // 速度环PID参数变化
    void velocity_pid_changed(double kp, double ki);
    // 速度目标值变化
    void velocity_target_changed(double target);
    // 位置环PID参数变化
    void position_pid_changed(double kp, double ki, double kd);
    // 位置目标值变化
    void position_target_changed(double target);
    // 环路使能状态变化
    void current_enabled_changed(bool enabled);
    void velocity_enabled_changed(bool enabled);
    void position_enabled_changed(bool enabled);
    // 环路预设变化（用于框图联动）
    void loop_preset_changed(const QString& preset);

private slots:
    // 电流环参数槽
    void on_current_kp_changed(double val);
    void on_current_ki_changed(double val);
    void on_current_enable_changed(int state);
    // 速度环参数槽
    void on_velocity_kp_changed(double val);
    void on_velocity_ki_changed(double val);
    void on_velocity_target_changed(double val);
    void on_velocity_enable_changed(int state);
    // 位置环参数槽
    void on_position_kp_changed(double val);
    void on_position_ki_changed(double val);
    void on_position_kd_changed(double val);
    void on_position_target_changed(double val);
    void on_position_enable_changed(int state);
    // 更新环路预设
    void update_loop_preset();

private:
    void setup_ui();
    QGroupBox* create_current_group();
    QGroupBox* create_velocity_group();
    QGroupBox* create_position_group();

private:
    // 电流环控件
    QDoubleSpinBox* m_spin_current_kp = nullptr;
    QDoubleSpinBox* m_spin_current_ki = nullptr;
    QCheckBox* m_check_current_enable = nullptr;
    
    // 速度环控件
    QDoubleSpinBox* m_spin_velocity_kp = nullptr;
    QDoubleSpinBox* m_spin_velocity_ki = nullptr;
    QDoubleSpinBox* m_spin_velocity_target = nullptr;
    QCheckBox* m_check_velocity_enable = nullptr;
    
    // 位置环控件
    QDoubleSpinBox* m_spin_position_kp = nullptr;
    QDoubleSpinBox* m_spin_position_ki = nullptr;
    QDoubleSpinBox* m_spin_position_kd = nullptr;
    QDoubleSpinBox* m_spin_position_target = nullptr;
    QCheckBox* m_check_position_enable = nullptr;
};

#endif // UI_PID_CONFIG_PANEL_H
