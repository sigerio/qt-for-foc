/**
 * @file motor_config_widget.h
 * @brief 电机参数配置控件声明
 */

#ifndef MOTOR_CONFIG_WIDGET_H
#define MOTOR_CONFIG_WIDGET_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>

#include "params/param_manager.h"

/**
 * @brief 电机参数配置控件
 */
class motor_config_widget_t : public QWidget
{
    Q_OBJECT

public:
    explicit motor_config_widget_t(param_manager_t *manager, QWidget *parent = nullptr);
    ~motor_config_widget_t();

private slots:
    void slot_on_motor_updated(const motor_physical_t &motor);
    void slot_on_limit_updated(const limit_param_t &limit);
    void slot_read_motor_clicked();
    void slot_write_motor_clicked();
    void slot_read_limit_clicked();
    void slot_write_limit_clicked();
    void slot_mode_changed(int index);
    void slot_save_config_clicked();
    void slot_load_config_clicked();

private:
    void setup_ui();

private:
    param_manager_t *m_manager;
    
    /* 电机物理参数 */
    QSpinBox *m_pole_pairs_spin;
    QDoubleSpinBox *m_phase_r_spin;
    QDoubleSpinBox *m_phase_l_spin;
    QDoubleSpinBox *m_torque_const_spin;
    
    /* 限制参数 */
    QDoubleSpinBox *m_current_limit_spin;
    QDoubleSpinBox *m_velocity_limit_spin;
    QDoubleSpinBox *m_voltage_limit_spin;
    
    /* 控制模式 */
    QComboBox *m_mode_combo;
    
    /* 按钮 */
    QPushButton *m_read_motor_btn;
    QPushButton *m_write_motor_btn;
    QPushButton *m_read_limit_btn;
    QPushButton *m_write_limit_btn;
    QPushButton *m_save_btn;
    QPushButton *m_load_btn;
};

#endif /* MOTOR_CONFIG_WIDGET_H */
