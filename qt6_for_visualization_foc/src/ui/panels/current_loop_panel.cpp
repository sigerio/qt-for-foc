#include "current_loop_panel.h"
#include "../widgets/waveform_view.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>

current_loop_panel::current_loop_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // 标题和使能开关
    auto* header = new QHBoxLayout();
    auto* title = new QLabel("电流环 (Id/Iq)", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    header->addWidget(title);
    m_check_enable = new QCheckBox("使能", this);
    m_check_enable->setChecked(true);
    connect(m_check_enable, &QCheckBox::stateChanged, this, &current_loop_panel::on_enable_changed);
    header->addWidget(m_check_enable);
    header->addStretch();
    layout->addLayout(header);
    
    // 波形显示区域
    auto* wave_layout = new QHBoxLayout();
    
    auto* id_group = new QGroupBox("Id (d轴电流)", this);
    auto* id_layout = new QVBoxLayout(id_group);
    m_id_wave = new waveform_view(this);
    m_id_wave->set_channels(2, {QColor(100, 100, 255), QColor(0, 0, 200)});
    m_id_wave->set_channel_names({"Id_ref", "Id"});
    m_id_wave->setMinimumHeight(100);
    id_layout->addWidget(m_id_wave);
    wave_layout->addWidget(id_group);
    
    auto* iq_group = new QGroupBox("Iq (q轴电流)", this);
    auto* iq_layout = new QVBoxLayout(iq_group);
    m_iq_wave = new waveform_view(this);
    m_iq_wave->set_channels(2, {QColor(255, 100, 100), QColor(200, 0, 0)});
    m_iq_wave->set_channel_names({"Iq_ref", "Iq"});
    m_iq_wave->setMinimumHeight(100);
    iq_layout->addWidget(m_iq_wave);
    wave_layout->addWidget(iq_group);
    
    layout->addLayout(wave_layout, 1);
    
    // PI参数调节
    auto* param_group = new QGroupBox("PI参数", this);
    auto* param_layout = new QGridLayout(param_group);
    
    // Kp参数
    param_layout->addWidget(new QLabel("Kp:", this), 0, 0);
    m_slider_kp = new QSlider(Qt::Horizontal, this);
    m_slider_kp->setRange(0, 1000);
    m_slider_kp->setValue(100);
    param_layout->addWidget(m_slider_kp, 0, 1);
    m_spin_kp = new QDoubleSpinBox(this);
    m_spin_kp->setRange(0, 10);
    m_spin_kp->setDecimals(2);
    m_spin_kp->setSingleStep(0.1);
    m_spin_kp->setValue(1.00);
    param_layout->addWidget(m_spin_kp, 0, 2);
    
    // Ki参数
    param_layout->addWidget(new QLabel("Ki:", this), 0, 3);
    m_slider_ki = new QSlider(Qt::Horizontal, this);
    m_slider_ki->setRange(0, 10000);
    m_slider_ki->setValue(1000);
    param_layout->addWidget(m_slider_ki, 0, 4);
    m_spin_ki = new QDoubleSpinBox(this);
    m_spin_ki->setRange(0, 1000);
    m_spin_ki->setDecimals(1);
    m_spin_ki->setSingleStep(10);
    m_spin_ki->setValue(100.0);
    param_layout->addWidget(m_spin_ki, 0, 5);
    
    // 信号连接：滑块和SpinBox同步
    connect(m_slider_kp, &QSlider::valueChanged, this, &current_loop_panel::on_slider_kp_changed);
    connect(m_slider_ki, &QSlider::valueChanged, this, &current_loop_panel::on_slider_ki_changed);
    connect(m_spin_kp, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &current_loop_panel::on_spin_kp_changed);
    connect(m_spin_ki, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &current_loop_panel::on_spin_ki_changed);
    
    layout->addWidget(param_group);
}

void current_loop_panel::update_currents(double id_ref, double id, double iq_ref, double iq) {
    m_id_wave->add_points({id_ref, id});
    m_iq_wave->add_points({iq_ref, iq});
}

void current_loop_panel::on_slider_kp_changed(int val) {
    double kp = val / 100.0;
    m_spin_kp->blockSignals(true);
    m_spin_kp->setValue(kp);
    m_spin_kp->blockSignals(false);
    emit pid_changed(kp, m_spin_ki->value());
}

void current_loop_panel::on_slider_ki_changed(int val) {
    double ki = val / 10.0;
    m_spin_ki->blockSignals(true);
    m_spin_ki->setValue(ki);
    m_spin_ki->blockSignals(false);
    emit pid_changed(m_spin_kp->value(), ki);
}

void current_loop_panel::on_spin_kp_changed(double val) {
    m_slider_kp->blockSignals(true);
    m_slider_kp->setValue(static_cast<int>(val * 100));
    m_slider_kp->blockSignals(false);
    emit pid_changed(val, m_spin_ki->value());
}

void current_loop_panel::on_spin_ki_changed(double val) {
    m_slider_ki->blockSignals(true);
    m_slider_ki->setValue(static_cast<int>(val * 10));
    m_slider_ki->blockSignals(false);
    emit pid_changed(m_spin_kp->value(), val);
}

void current_loop_panel::on_enable_changed(int state) {
    emit loop_enabled_changed(state == Qt::Checked);
}

void current_loop_panel::update_waveform() {}
