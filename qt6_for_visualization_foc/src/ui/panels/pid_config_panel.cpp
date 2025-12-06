#include "pid_config_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

pid_config_panel::pid_config_panel(QWidget* parent) : QWidget(parent) {
    setup_ui();
}

void pid_config_panel::setup_ui() {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);
    
    // 添加三个环路配置组
    layout->addWidget(create_current_group());
    layout->addWidget(create_velocity_group());
    layout->addWidget(create_position_group());
    layout->addStretch();
}

QGroupBox* pid_config_panel::create_current_group() {
    auto* group = new QGroupBox("电流环 PI", this);
    auto* layout = new QGridLayout(group);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // 使能开关
    m_check_current_enable = new QCheckBox("使能", group);
    m_check_current_enable->setChecked(true);
    connect(m_check_current_enable, &QCheckBox::stateChanged, 
            this, &pid_config_panel::on_current_enable_changed);
    layout->addWidget(m_check_current_enable, 0, 0, 1, 2);
    
    // Kp
    layout->addWidget(new QLabel("Kp:", group), 1, 0);
    m_spin_current_kp = new QDoubleSpinBox(group);
    m_spin_current_kp->setRange(0, 100);
    m_spin_current_kp->setSingleStep(0.1);
    m_spin_current_kp->setValue(1.0);
    connect(m_spin_current_kp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &pid_config_panel::on_current_kp_changed);
    layout->addWidget(m_spin_current_kp, 1, 1);
    
    // Ki
    layout->addWidget(new QLabel("Ki:", group), 2, 0);
    m_spin_current_ki = new QDoubleSpinBox(group);
    m_spin_current_ki->setRange(0, 1000);
    m_spin_current_ki->setSingleStep(1);
    m_spin_current_ki->setValue(100.0);
    connect(m_spin_current_ki, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &pid_config_panel::on_current_ki_changed);
    layout->addWidget(m_spin_current_ki, 2, 1);
    
    return group;
}

QGroupBox* pid_config_panel::create_velocity_group() {
    auto* group = new QGroupBox("速度环 PI", this);
    auto* layout = new QGridLayout(group);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // 使能开关
    m_check_velocity_enable = new QCheckBox("使能", group);
    m_check_velocity_enable->setChecked(false);
    connect(m_check_velocity_enable, &QCheckBox::stateChanged, 
            this, &pid_config_panel::on_velocity_enable_changed);
    layout->addWidget(m_check_velocity_enable, 0, 0, 1, 2);
    
    // Kp
    layout->addWidget(new QLabel("Kp:", group), 1, 0);
    m_spin_velocity_kp = new QDoubleSpinBox(group);
    m_spin_velocity_kp->setRange(0, 10);
    m_spin_velocity_kp->setSingleStep(0.01);
    m_spin_velocity_kp->setValue(0.1);
    connect(m_spin_velocity_kp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &pid_config_panel::on_velocity_kp_changed);
    layout->addWidget(m_spin_velocity_kp, 1, 1);
    
    // Ki
    layout->addWidget(new QLabel("Ki:", group), 2, 0);
    m_spin_velocity_ki = new QDoubleSpinBox(group);
    m_spin_velocity_ki->setRange(0, 100);
    m_spin_velocity_ki->setSingleStep(0.1);
    m_spin_velocity_ki->setValue(1.0);
    connect(m_spin_velocity_ki, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &pid_config_panel::on_velocity_ki_changed);
    layout->addWidget(m_spin_velocity_ki, 2, 1);
    
    // 目标值
    layout->addWidget(new QLabel("目标:", group), 3, 0);
    m_spin_velocity_target = new QDoubleSpinBox(group);
    m_spin_velocity_target->setRange(-1000, 1000);
    m_spin_velocity_target->setSingleStep(10);
    m_spin_velocity_target->setValue(100);
    m_spin_velocity_target->setSuffix(" rpm");
    connect(m_spin_velocity_target, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &pid_config_panel::on_velocity_target_changed);
    layout->addWidget(m_spin_velocity_target, 3, 1);
    
    return group;
}

QGroupBox* pid_config_panel::create_position_group() {
    auto* group = new QGroupBox("位置环 PID", this);
    auto* layout = new QGridLayout(group);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // 使能开关
    m_check_position_enable = new QCheckBox("使能", group);
    m_check_position_enable->setChecked(false);
    connect(m_check_position_enable, &QCheckBox::stateChanged, 
            this, &pid_config_panel::on_position_enable_changed);
    layout->addWidget(m_check_position_enable, 0, 0, 1, 2);
    
    // Kp
    layout->addWidget(new QLabel("Kp:", group), 1, 0);
    m_spin_position_kp = new QDoubleSpinBox(group);
    m_spin_position_kp->setRange(0, 100);
    m_spin_position_kp->setSingleStep(0.1);
    m_spin_position_kp->setValue(1.0);
    connect(m_spin_position_kp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &pid_config_panel::on_position_kp_changed);
    layout->addWidget(m_spin_position_kp, 1, 1);
    
    // Ki
    layout->addWidget(new QLabel("Ki:", group), 2, 0);
    m_spin_position_ki = new QDoubleSpinBox(group);
    m_spin_position_ki->setRange(0, 100);
    m_spin_position_ki->setSingleStep(0.1);
    m_spin_position_ki->setValue(0.1);
    connect(m_spin_position_ki, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &pid_config_panel::on_position_ki_changed);
    layout->addWidget(m_spin_position_ki, 2, 1);
    
    // Kd
    layout->addWidget(new QLabel("Kd:", group), 3, 0);
    m_spin_position_kd = new QDoubleSpinBox(group);
    m_spin_position_kd->setRange(0, 10);
    m_spin_position_kd->setSingleStep(0.01);
    m_spin_position_kd->setValue(0.01);
    connect(m_spin_position_kd, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &pid_config_panel::on_position_kd_changed);
    layout->addWidget(m_spin_position_kd, 3, 1);
    
    // 目标值
    layout->addWidget(new QLabel("目标:", group), 4, 0);
    m_spin_position_target = new QDoubleSpinBox(group);
    m_spin_position_target->setRange(-360, 360);
    m_spin_position_target->setSingleStep(10);
    m_spin_position_target->setValue(0);
    m_spin_position_target->setSuffix(" °");
    connect(m_spin_position_target, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &pid_config_panel::on_position_target_changed);
    layout->addWidget(m_spin_position_target, 4, 1);
    
    return group;
}

bool pid_config_panel::is_current_enabled() const {
    return m_check_current_enable && m_check_current_enable->isChecked();
}

bool pid_config_panel::is_velocity_enabled() const {
    return m_check_velocity_enable && m_check_velocity_enable->isChecked();
}

bool pid_config_panel::is_position_enabled() const {
    return m_check_position_enable && m_check_position_enable->isChecked();
}

// 电流环槽函数
void pid_config_panel::on_current_kp_changed(double val) {
    emit current_pid_changed(val, m_spin_current_ki->value());
}

void pid_config_panel::on_current_ki_changed(double val) {
    emit current_pid_changed(m_spin_current_kp->value(), val);
}

void pid_config_panel::on_current_enable_changed(int state) {
    emit current_enabled_changed(state == Qt::Checked);
    update_loop_preset();
}

// 速度环槽函数
void pid_config_panel::on_velocity_kp_changed(double val) {
    emit velocity_pid_changed(val, m_spin_velocity_ki->value());
}

void pid_config_panel::on_velocity_ki_changed(double val) {
    emit velocity_pid_changed(m_spin_velocity_kp->value(), val);
}

void pid_config_panel::on_velocity_target_changed(double val) {
    emit velocity_target_changed(val);
}

void pid_config_panel::on_velocity_enable_changed(int state) {
    emit velocity_enabled_changed(state == Qt::Checked);
    update_loop_preset();
}

// 位置环槽函数
void pid_config_panel::on_position_kp_changed(double val) {
    emit position_pid_changed(val, m_spin_position_ki->value(), m_spin_position_kd->value());
}

void pid_config_panel::on_position_ki_changed(double val) {
    emit position_pid_changed(m_spin_position_kp->value(), val, m_spin_position_kd->value());
}

void pid_config_panel::on_position_kd_changed(double val) {
    emit position_pid_changed(m_spin_position_kp->value(), m_spin_position_ki->value(), val);
}

void pid_config_panel::on_position_target_changed(double val) {
    emit position_target_changed(val);
}

void pid_config_panel::on_position_enable_changed(int state) {
    emit position_enabled_changed(state == Qt::Checked);
    update_loop_preset();
}

void pid_config_panel::update_loop_preset() {
    QString preset;
    bool current = is_current_enabled();
    bool velocity = is_velocity_enabled();
    bool position = is_position_enabled();
    
    if (current && velocity && position) {
        preset = "full";
    } else if (current && velocity) {
        preset = "current_velocity";
    } else if (current) {
        preset = "current";
    }
    
    if (!preset.isEmpty()) {
        emit loop_preset_changed(preset);
    }
}
