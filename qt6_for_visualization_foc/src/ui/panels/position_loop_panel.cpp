#include "position_loop_panel.h"
#include "../widgets/waveform_view.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>

position_loop_panel::position_loop_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // 标题和使能开关
    auto* header = new QHBoxLayout();
    auto* title = new QLabel("位置环", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    header->addWidget(title);
    m_check_enable = new QCheckBox("使能", this);
    m_check_enable->setChecked(false);
    connect(m_check_enable, &QCheckBox::stateChanged, this, &position_loop_panel::on_enable_changed);
    header->addWidget(m_check_enable);
    header->addStretch();
    layout->addLayout(header);
    
    // 位置波形
    auto* wave_group = new QGroupBox("位置跟踪", this);
    auto* wave_layout = new QVBoxLayout(wave_group);
    m_pos_wave = new waveform_view(this);
    m_pos_wave->set_channels(2, {QColor(200, 100, 200), QColor(150, 0, 150)});
    m_pos_wave->set_channel_names({"θ_ref", "θ"});
    m_pos_wave->setMinimumHeight(100);
    wave_layout->addWidget(m_pos_wave);
    layout->addWidget(wave_group, 1);
    
    // PID参数和目标位置
    auto* param_group = new QGroupBox("PID参数", this);
    auto* param_layout = new QGridLayout(param_group);
    
    // Kp参数
    param_layout->addWidget(new QLabel("Kp:", this), 0, 0);
    m_slider_kp = new QSlider(Qt::Horizontal, this);
    m_slider_kp->setRange(0, 200);
    m_slider_kp->setValue(50);
    param_layout->addWidget(m_slider_kp, 0, 1);
    m_spin_kp = new QDoubleSpinBox(this);
    m_spin_kp->setRange(0, 20);
    m_spin_kp->setDecimals(1);
    m_spin_kp->setSingleStep(0.5);
    m_spin_kp->setValue(5.0);
    param_layout->addWidget(m_spin_kp, 0, 2);
    
    // Ki参数
    param_layout->addWidget(new QLabel("Ki:", this), 0, 3);
    m_slider_ki = new QSlider(Qt::Horizontal, this);
    m_slider_ki->setRange(0, 100);
    m_slider_ki->setValue(10);
    param_layout->addWidget(m_slider_ki, 0, 4);
    m_spin_ki = new QDoubleSpinBox(this);
    m_spin_ki->setRange(0, 10);
    m_spin_ki->setDecimals(1);
    m_spin_ki->setSingleStep(0.1);
    m_spin_ki->setValue(1.0);
    param_layout->addWidget(m_spin_ki, 0, 5);
    
    // Kd参数
    param_layout->addWidget(new QLabel("Kd:", this), 1, 0);
    m_slider_kd = new QSlider(Qt::Horizontal, this);
    m_slider_kd->setRange(0, 100);
    m_slider_kd->setValue(1);
    param_layout->addWidget(m_slider_kd, 1, 1);
    m_spin_kd = new QDoubleSpinBox(this);
    m_spin_kd->setRange(0, 1);
    m_spin_kd->setDecimals(2);
    m_spin_kd->setSingleStep(0.01);
    m_spin_kd->setValue(0.01);
    param_layout->addWidget(m_spin_kd, 1, 2);
    
    // 目标位置
    param_layout->addWidget(new QLabel("目标位置:", this), 1, 3);
    m_spin_target = new QDoubleSpinBox(this);
    m_spin_target->setRange(-100, 100);
    m_spin_target->setValue(6.28);
    m_spin_target->setDecimals(2);
    m_spin_target->setSuffix(" rad");
    param_layout->addWidget(m_spin_target, 1, 4, 1, 2);
    
    // 信号连接
    connect(m_slider_kp, &QSlider::valueChanged, this, &position_loop_panel::on_slider_kp_changed);
    connect(m_slider_ki, &QSlider::valueChanged, this, &position_loop_panel::on_slider_ki_changed);
    connect(m_slider_kd, &QSlider::valueChanged, this, &position_loop_panel::on_slider_kd_changed);
    connect(m_spin_kp, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &position_loop_panel::on_spin_kp_changed);
    connect(m_spin_ki, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &position_loop_panel::on_spin_ki_changed);
    connect(m_spin_kd, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &position_loop_panel::on_spin_kd_changed);
    connect(m_spin_target, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &position_loop_panel::on_target_changed);
    
    layout->addWidget(param_group);
}

void position_loop_panel::update_position(double pos_ref, double pos) {
    m_pos_wave->add_points({pos_ref, pos});
}

void position_loop_panel::on_slider_kp_changed(int val) {
    double kp = val / 10.0;
    m_spin_kp->blockSignals(true);
    m_spin_kp->setValue(kp);
    m_spin_kp->blockSignals(false);
    emit pid_changed(kp, m_spin_ki->value(), m_spin_kd->value());
}

void position_loop_panel::on_slider_ki_changed(int val) {
    double ki = val / 10.0;
    m_spin_ki->blockSignals(true);
    m_spin_ki->setValue(ki);
    m_spin_ki->blockSignals(false);
    emit pid_changed(m_spin_kp->value(), ki, m_spin_kd->value());
}

void position_loop_panel::on_slider_kd_changed(int val) {
    double kd = val / 100.0;
    m_spin_kd->blockSignals(true);
    m_spin_kd->setValue(kd);
    m_spin_kd->blockSignals(false);
    emit pid_changed(m_spin_kp->value(), m_spin_ki->value(), kd);
}

void position_loop_panel::on_spin_kp_changed(double val) {
    m_slider_kp->blockSignals(true);
    m_slider_kp->setValue(static_cast<int>(val * 10));
    m_slider_kp->blockSignals(false);
    emit pid_changed(val, m_spin_ki->value(), m_spin_kd->value());
}

void position_loop_panel::on_spin_ki_changed(double val) {
    m_slider_ki->blockSignals(true);
    m_slider_ki->setValue(static_cast<int>(val * 10));
    m_slider_ki->blockSignals(false);
    emit pid_changed(m_spin_kp->value(), val, m_spin_kd->value());
}

void position_loop_panel::on_spin_kd_changed(double val) {
    m_slider_kd->blockSignals(true);
    m_slider_kd->setValue(static_cast<int>(val * 100));
    m_slider_kd->blockSignals(false);
    emit pid_changed(m_spin_kp->value(), m_spin_ki->value(), val);
}

void position_loop_panel::on_target_changed(double val) {
    emit target_changed(val);
}

void position_loop_panel::on_enable_changed(int state) {
    emit loop_enabled_changed(state == Qt::Checked);
}

void position_loop_panel::update_waveform() {}
