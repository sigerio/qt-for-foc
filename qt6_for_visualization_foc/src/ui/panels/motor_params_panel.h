#ifndef UI_MOTOR_PARAMS_PANEL_H
#define UI_MOTOR_PARAMS_PANEL_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include "core/types.h"

// 电机参数配置面板
class motor_params_panel : public QWidget {
    Q_OBJECT
public:
    explicit motor_params_panel(QWidget* parent = nullptr);
    
    // 设置/获取参数
    void set_params(const motor_params_t& params);
    motor_params_t get_params() const;
    
signals:
    // 参数变化信号
    void params_changed(const motor_params_t& params);

private slots:
    void on_param_changed();

private:
    QDoubleSpinBox* m_spin_rs = nullptr;       // 定子电阻
    QDoubleSpinBox* m_spin_ld = nullptr;       // d轴电感
    QDoubleSpinBox* m_spin_lq = nullptr;       // q轴电感
    QDoubleSpinBox* m_spin_psi_f = nullptr;    // 磁链
    QDoubleSpinBox* m_spin_j = nullptr;        // 转动惯量
    QDoubleSpinBox* m_spin_b = nullptr;        // 阻尼系数
    QSpinBox* m_spin_poles = nullptr;          // 极对数
};

#endif // UI_MOTOR_PARAMS_PANEL_H
