#include "motor_params_panel.h"
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>

motor_params_panel::motor_params_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    
    auto* group = new QGroupBox("电机参数", this);
    auto* grid = new QGridLayout(group);
    grid->setSpacing(2);
    
    int row = 0;
    
    // Rs - 定子电阻
    grid->addWidget(new QLabel("Rs(Ω):", this), row, 0);
    m_spin_rs = new QDoubleSpinBox(this);
    m_spin_rs->setRange(0.01, 10.0);
    m_spin_rs->setDecimals(3);
    m_spin_rs->setValue(0.3);
    grid->addWidget(m_spin_rs, row++, 1);
    
    // Ld - d轴电感
    grid->addWidget(new QLabel("Ld(mH):", this), row, 0);
    m_spin_ld = new QDoubleSpinBox(this);
    m_spin_ld->setRange(0.01, 100.0);
    m_spin_ld->setDecimals(2);
    m_spin_ld->setValue(1.0);
    grid->addWidget(m_spin_ld, row++, 1);
    
    // Lq - q轴电感
    grid->addWidget(new QLabel("Lq(mH):", this), row, 0);
    m_spin_lq = new QDoubleSpinBox(this);
    m_spin_lq->setRange(0.01, 100.0);
    m_spin_lq->setDecimals(2);
    m_spin_lq->setValue(1.0);
    grid->addWidget(m_spin_lq, row++, 1);
    
    // ψf - 永磁磁链
    grid->addWidget(new QLabel("ψf(Wb):", this), row, 0);
    m_spin_psi_f = new QDoubleSpinBox(this);
    m_spin_psi_f->setRange(0.001, 1.0);
    m_spin_psi_f->setDecimals(3);
    m_spin_psi_f->setValue(0.15);
    grid->addWidget(m_spin_psi_f, row++, 1);
    
    // J - 转动惯量
    grid->addWidget(new QLabel("J:", this), row, 0);
    m_spin_j = new QDoubleSpinBox(this);
    m_spin_j->setRange(0.00001, 1.0);
    m_spin_j->setDecimals(4);
    m_spin_j->setValue(0.001);
    grid->addWidget(m_spin_j, row++, 1);
    
    // B - 阻尼系数
    grid->addWidget(new QLabel("B:", this), row, 0);
    m_spin_b = new QDoubleSpinBox(this);
    m_spin_b->setRange(0.0, 0.1);
    m_spin_b->setDecimals(5);
    m_spin_b->setValue(0.0001);
    grid->addWidget(m_spin_b, row++, 1);
    
    // 极对数
    grid->addWidget(new QLabel("p:", this), row, 0);
    m_spin_poles = new QSpinBox(this);
    m_spin_poles->setRange(1, 20);
    m_spin_poles->setValue(4);
    grid->addWidget(m_spin_poles, row++, 1);
    
    layout->addWidget(group);
    
    // 连接信号
    connect(m_spin_rs, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &motor_params_panel::on_param_changed);
    connect(m_spin_ld, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &motor_params_panel::on_param_changed);
    connect(m_spin_lq, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &motor_params_panel::on_param_changed);
    connect(m_spin_psi_f, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &motor_params_panel::on_param_changed);
    connect(m_spin_j, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &motor_params_panel::on_param_changed);
    connect(m_spin_b, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &motor_params_panel::on_param_changed);
    connect(m_spin_poles, QOverload<int>::of(&QSpinBox::valueChanged), this, &motor_params_panel::on_param_changed);
}

void motor_params_panel::set_params(const motor_params_t& params) {
    m_spin_rs->blockSignals(true);
    m_spin_ld->blockSignals(true);
    m_spin_lq->blockSignals(true);
    m_spin_psi_f->blockSignals(true);
    m_spin_j->blockSignals(true);
    m_spin_b->blockSignals(true);
    m_spin_poles->blockSignals(true);
    
    m_spin_rs->setValue(params.rs);
    m_spin_ld->setValue(params.ld * 1000.0);  // H -> mH
    m_spin_lq->setValue(params.lq * 1000.0);
    m_spin_psi_f->setValue(params.psi_f);
    m_spin_j->setValue(params.j);
    m_spin_b->setValue(params.b);
    m_spin_poles->setValue(params.pole_pairs);
    
    m_spin_rs->blockSignals(false);
    m_spin_ld->blockSignals(false);
    m_spin_lq->blockSignals(false);
    m_spin_psi_f->blockSignals(false);
    m_spin_j->blockSignals(false);
    m_spin_b->blockSignals(false);
    m_spin_poles->blockSignals(false);
}

motor_params_t motor_params_panel::get_params() const {
    motor_params_t p;
    p.rs = m_spin_rs->value();
    p.ld = m_spin_ld->value() / 1000.0;  // mH -> H
    p.lq = m_spin_lq->value() / 1000.0;
    p.psi_f = m_spin_psi_f->value();
    p.j = m_spin_j->value();
    p.b = m_spin_b->value();
    p.pole_pairs = m_spin_poles->value();
    return p;
}

void motor_params_panel::on_param_changed() {
    emit params_changed(get_params());
}
