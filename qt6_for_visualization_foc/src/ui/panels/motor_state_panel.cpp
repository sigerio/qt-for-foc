#include "motor_state_panel.h"
#include "../widgets/rotor_animator.h"
#include "../widgets/waveform_view.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <cmath>

motor_state_panel::motor_state_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    auto* title = new QLabel("电机状态", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    layout->addWidget(title);
    
    // 转子动画和电流波形区域
    auto* view_layout = new QHBoxLayout();
    
    // 转子动画（固定大小）
    auto* rotor_group = new QGroupBox("转子位置", this);
    auto* rotor_layout = new QVBoxLayout(rotor_group);
    m_rotor = new rotor_animator(this);
    m_rotor->set_pole_pairs(4);
    m_rotor->setFixedSize(220, 220);
    rotor_layout->addWidget(m_rotor);
    rotor_group->setFixedWidth(240);
    view_layout->addWidget(rotor_group, 0);
    
    // 三相电流波形
    auto* wave_group = new QGroupBox("三相电流波形", this);
    auto* wave_layout = new QVBoxLayout(wave_group);
    m_current_wave = new waveform_view(this);
    m_current_wave->set_channels(3, {Qt::red, Qt::green, Qt::blue});
    m_current_wave->set_channel_names({"Ia", "Ib", "Ic"});
    m_current_wave->set_max_points(1000);  // 增加采样点数避免锯齿
    wave_layout->addWidget(m_current_wave);
    view_layout->addWidget(wave_group);
    
    layout->addLayout(view_layout, 1);
    
    // 状态数值显示
    auto* data_group = new QGroupBox("实时状态", this);
    auto* data_layout = new QGridLayout(data_group);
    
    data_layout->addWidget(new QLabel("转速 ω:", this), 0, 0);
    m_label_omega = new QLabel("0.00 rad/s", this);
    m_label_omega->setStyleSheet("color: #0066cc;");
    data_layout->addWidget(m_label_omega, 0, 1);
    
    data_layout->addWidget(new QLabel("转矩 Te:", this), 0, 2);
    m_label_torque = new QLabel("0.00 N·m", this);
    m_label_torque->setStyleSheet("color: #cc6600;");
    data_layout->addWidget(m_label_torque, 0, 3);
    
    data_layout->addWidget(new QLabel("角度 θ:", this), 0, 4);
    m_label_theta = new QLabel("0.00°", this);
    m_label_theta->setStyleSheet("color: #006600;");
    data_layout->addWidget(m_label_theta, 0, 5);
    
    // 负载转矩设置
    data_layout->addWidget(new QLabel("负载扰动 Tl:", this), 1, 0);
    m_slider_load = new QSlider(Qt::Horizontal, this);
    m_slider_load->setRange(0, 100);  // 0-1.0 N·m (步长0.01)
    m_slider_load->setValue(20);      // 默认0.2 N·m
    data_layout->addWidget(m_slider_load, 1, 1, 1, 3);
    
    m_spin_load = new QDoubleSpinBox(this);
    m_spin_load->setRange(0.0, 1.0);
    m_spin_load->setDecimals(2);
    m_spin_load->setSingleStep(0.01);
    m_spin_load->setValue(0.2);
    m_spin_load->setSuffix(" N·m");
    data_layout->addWidget(m_spin_load, 1, 4, 1, 2);
    
    connect(m_slider_load, &QSlider::valueChanged, this, &motor_state_panel::on_load_slider_changed);
    connect(m_spin_load, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &motor_state_panel::on_load_spin_changed);
    
    layout->addWidget(data_group);
}

void motor_state_panel::update_angle(double theta_rad) {
    m_rotor->set_angle(theta_rad);
}

void motor_state_panel::update_phase_currents(double ia, double ib, double ic) {
    m_current_wave->add_points({ia, ib, ic});
}

void motor_state_panel::update_motor_state(double omega, double torque, double theta) {
    m_label_omega->setText(QString("%1 rad/s").arg(omega, 0, 'f', 2));
    m_label_torque->setText(QString("%1 N·m").arg(torque, 0, 'f', 3));
    double deg = theta * 180.0 / M_PI;
    while (deg < 0) deg += 360;
    while (deg >= 360) deg -= 360;
    m_label_theta->setText(QString("%1°").arg(deg, 0, 'f', 1));
}

void motor_state_panel::update_display() {}

void motor_state_panel::on_load_slider_changed(int val) {
    double tl = val / 100.0;
    m_spin_load->blockSignals(true);
    m_spin_load->setValue(tl);
    m_spin_load->blockSignals(false);
    emit load_torque_changed(tl);
}

void motor_state_panel::on_load_spin_changed(double val) {
    m_slider_load->blockSignals(true);
    m_slider_load->setValue(static_cast<int>(val * 100));
    m_slider_load->blockSignals(false);
    emit load_torque_changed(val);
}
