/**
 * @file pid_config_widget.h
 * @brief PID参数配置控件声明
 */

#ifndef PID_CONFIG_WIDGET_H
#define PID_CONFIG_WIDGET_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QGroupBox>

#include "params/param_manager.h"

/**
 * @brief PID参数配置控件
 */
class pid_config_widget_t : public QWidget
{
    Q_OBJECT

public:
    explicit pid_config_widget_t(param_manager_t *manager, QWidget *parent = nullptr);
    ~pid_config_widget_t();

private slots:
    void slot_on_pid_updated(const pid_config_t &pid);
    void slot_read_clicked();
    void slot_write_current_clicked();
    void slot_write_velocity_clicked();
    void slot_write_position_clicked();

private:
    void setup_ui();
    QGroupBox* create_pid_group(const QString &title, 
                                 QDoubleSpinBox *&kp, 
                                 QDoubleSpinBox *&ki, 
                                 QDoubleSpinBox *&kd);
    pid_param_t get_pid_from_spinboxes(QDoubleSpinBox *kp, QDoubleSpinBox *ki, QDoubleSpinBox *kd);

private:
    param_manager_t *m_manager;
    
    /* 电流环 */
    QDoubleSpinBox *m_current_kp;
    QDoubleSpinBox *m_current_ki;
    QDoubleSpinBox *m_current_kd;
    QPushButton *m_current_write_btn;
    
    /* 速度环 */
    QDoubleSpinBox *m_velocity_kp;
    QDoubleSpinBox *m_velocity_ki;
    QDoubleSpinBox *m_velocity_kd;
    QPushButton *m_velocity_write_btn;
    
    /* 位置环 */
    QDoubleSpinBox *m_position_kp;
    QDoubleSpinBox *m_position_ki;
    QDoubleSpinBox *m_position_kd;
    QPushButton *m_position_write_btn;
    
    QPushButton *m_read_all_btn;
};

#endif /* PID_CONFIG_WIDGET_H */
