#include "velocity_loop_panel.h"
#include "../widgets/waveform_view.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>

velocity_loop_panel::velocity_loop_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // 标题和使能开关
    auto* header = new QHBoxLayout();
    auto* title = new QLabel("速度环", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    header->addWidget(title);
    m_check_enable = new QCheckBox("使能", this);
    m_check_enable->setChecked(true);
    connect(m_check_enable, &QCheckBox::stateChanged, this, &velocity_loop_panel::on_enable_changed);
    header->addWidget(m_check_enable);
    header->addStretch();
    layout->addLayout(header);
    
    // 速度波形
    auto* wave_group = new QGroupBox("速度跟踪", this);
    auto* wave_layout = new QVBoxLayout(wave_group);
    m_vel_wave = new waveform_view(this);
    m_vel_wave->set_channels(2, {QColor(100, 200, 100), QColor(0, 150, 0)});
    m_vel_wave->set_channel_names({"ω_ref", "ω"});
    wave_layout->addWidget(m_vel_wave);
    layout->addWidget(wave_group, 1);
    
    // PI参数和目标速度
    auto* param_group = new QGroupBox("参数设置", this);
    auto* param_layout = new QGridLayout(param_group);
    
    param_layout->addWidget(new QLabel("Kp:", this), 0, 0);
    m_slider_kp = new QSlider(Qt::Horizontal, this);
    m_slider_kp->setRange(0, 500);
    m_slider_kp->setValue(50);
    connect(m_slider_kp, &QSlider::valueChanged, this, &velocity_loop_panel::on_slider_changed);
    param_layout->addWidget(m_slider_kp, 0, 1);
    m_label_kp = new QLabel("0.50", this);
    param_layout->addWidget(m_label_kp, 0, 2);
    
    param_layout->addWidget(new QLabel("Ki:", this), 0, 3);
    m_slider_ki = new QSlider(Qt::Horizontal, this);
    m_slider_ki->setRange(0, 1000);
    m_slider_ki->setValue(100);
    connect(m_slider_ki, &QSlider::valueChanged, this, &velocity_loop_panel::on_slider_changed);
    param_layout->addWidget(m_slider_ki, 0, 4);
    m_label_ki = new QLabel("10.0", this);
    param_layout->addWidget(m_label_ki, 0, 5);
    
    param_layout->addWidget(new QLabel("目标速度:", this), 1, 0);
    m_spin_target = new QDoubleSpinBox(this);
    m_spin_target->setRange(-500, 500);
    m_spin_target->setValue(100);
    m_spin_target->setSuffix(" rad/s");
    connect(m_spin_target, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, &velocity_loop_panel::on_target_changed);
    param_layout->addWidget(m_spin_target, 1, 1, 1, 2);
    
    layout->addWidget(param_group);
}

void velocity_loop_panel::update_velocity(double vel_ref, double vel) {
    m_vel_wave->add_points({vel_ref, vel});
}

void velocity_loop_panel::on_slider_changed() {
    double kp = m_slider_kp->value() / 100.0;
    double ki = m_slider_ki->value() / 10.0;
    m_label_kp->setText(QString::number(kp, 'f', 2));
    m_label_ki->setText(QString::number(ki, 'f', 1));
    emit pid_changed(kp, ki);
}

void velocity_loop_panel::on_target_changed(double val) {
    emit target_changed(val);
}

void velocity_loop_panel::on_enable_changed(int state) {
    emit loop_enabled_changed(state == Qt::Checked);
}

void velocity_loop_panel::update_waveform() {}
