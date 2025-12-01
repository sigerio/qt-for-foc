/**
 * @file motor_config_widget.cpp
 * @brief 电机参数配置控件实现
 */

#include "motor_config_widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFileDialog>

motor_config_widget_t::motor_config_widget_t(param_manager_t *manager, QWidget *parent)
    : QWidget(parent)
    , m_manager(manager)
{
    setup_ui();
    
    connect(m_manager, &param_manager_t::signal_motor_updated,
            this, &motor_config_widget_t::slot_on_motor_updated);
    connect(m_manager, &param_manager_t::signal_limit_updated,
            this, &motor_config_widget_t::slot_on_limit_updated);
}

motor_config_widget_t::~motor_config_widget_t()
{
}

/**
 * @brief 初始化UI
 */
void motor_config_widget_t::setup_ui()
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    
    /* 电机物理参数组 */
    QGroupBox *motor_group = new QGroupBox("电机物理参数", this);
    QGridLayout *motor_layout = new QGridLayout(motor_group);
    
    motor_layout->addWidget(new QLabel("极对数:"), 0, 0);
    m_pole_pairs_spin = new QSpinBox(this);
    m_pole_pairs_spin->setRange(1, 100);
    motor_layout->addWidget(m_pole_pairs_spin, 0, 1);
    
    motor_layout->addWidget(new QLabel("相电阻(Ω):"), 1, 0);
    m_phase_r_spin = new QDoubleSpinBox(this);
    m_phase_r_spin->setRange(0, 100);
    m_phase_r_spin->setDecimals(4);
    motor_layout->addWidget(m_phase_r_spin, 1, 1);
    
    motor_layout->addWidget(new QLabel("相电感(H):"), 2, 0);
    m_phase_l_spin = new QDoubleSpinBox(this);
    m_phase_l_spin->setRange(0, 1);
    m_phase_l_spin->setDecimals(6);
    motor_layout->addWidget(m_phase_l_spin, 2, 1);
    
    motor_layout->addWidget(new QLabel("转矩常数(Nm/A):"), 3, 0);
    m_torque_const_spin = new QDoubleSpinBox(this);
    m_torque_const_spin->setRange(0, 100);
    m_torque_const_spin->setDecimals(4);
    motor_layout->addWidget(m_torque_const_spin, 3, 1);
    
    m_read_motor_btn = new QPushButton("读取", this);
    m_write_motor_btn = new QPushButton("写入", this);
    motor_layout->addWidget(m_read_motor_btn, 4, 0);
    motor_layout->addWidget(m_write_motor_btn, 4, 1);
    
    main_layout->addWidget(motor_group);
    
    /* 限制参数组 */
    QGroupBox *limit_group = new QGroupBox("限制参数", this);
    QGridLayout *limit_layout = new QGridLayout(limit_group);
    
    limit_layout->addWidget(new QLabel("电流限制(A):"), 0, 0);
    m_current_limit_spin = new QDoubleSpinBox(this);
    m_current_limit_spin->setRange(0, 100);
    m_current_limit_spin->setDecimals(2);
    limit_layout->addWidget(m_current_limit_spin, 0, 1);
    
    limit_layout->addWidget(new QLabel("速度限制(rad/s):"), 1, 0);
    m_velocity_limit_spin = new QDoubleSpinBox(this);
    m_velocity_limit_spin->setRange(0, 10000);
    m_velocity_limit_spin->setDecimals(2);
    limit_layout->addWidget(m_velocity_limit_spin, 1, 1);
    
    limit_layout->addWidget(new QLabel("电压限制(V):"), 2, 0);
    m_voltage_limit_spin = new QDoubleSpinBox(this);
    m_voltage_limit_spin->setRange(0, 100);
    m_voltage_limit_spin->setDecimals(2);
    limit_layout->addWidget(m_voltage_limit_spin, 2, 1);
    
    m_read_limit_btn = new QPushButton("读取", this);
    m_write_limit_btn = new QPushButton("写入", this);
    limit_layout->addWidget(m_read_limit_btn, 3, 0);
    limit_layout->addWidget(m_write_limit_btn, 3, 1);
    
    main_layout->addWidget(limit_group);

    /* 控制模式组 */
    QGroupBox *mode_group = new QGroupBox("控制模式", this);
    QHBoxLayout *mode_layout = new QHBoxLayout(mode_group);
    
    mode_layout->addWidget(new QLabel("模式:"));
    m_mode_combo = new QComboBox(this);
    m_mode_combo->addItems({"力矩模式", "速度模式", "位置模式"});
    mode_layout->addWidget(m_mode_combo);
    mode_layout->addStretch();
    
    main_layout->addWidget(mode_group);
    
    /* 配置文件操作 */
    QHBoxLayout *file_layout = new QHBoxLayout();
    m_save_btn = new QPushButton("保存配置到文件", this);
    m_load_btn = new QPushButton("从文件加载配置", this);
    file_layout->addWidget(m_save_btn);
    file_layout->addWidget(m_load_btn);
    file_layout->addStretch();
    main_layout->addLayout(file_layout);
    
    main_layout->addStretch();
    
    /* 连接信号 */
    connect(m_read_motor_btn, &QPushButton::clicked, this, &motor_config_widget_t::slot_read_motor_clicked);
    connect(m_write_motor_btn, &QPushButton::clicked, this, &motor_config_widget_t::slot_write_motor_clicked);
    connect(m_read_limit_btn, &QPushButton::clicked, this, &motor_config_widget_t::slot_read_limit_clicked);
    connect(m_write_limit_btn, &QPushButton::clicked, this, &motor_config_widget_t::slot_write_limit_clicked);
    connect(m_mode_combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &motor_config_widget_t::slot_mode_changed);
    connect(m_save_btn, &QPushButton::clicked, this, &motor_config_widget_t::slot_save_config_clicked);
    connect(m_load_btn, &QPushButton::clicked, this, &motor_config_widget_t::slot_load_config_clicked);
}

/* ============== 槽函数 ============== */

/**
 * @brief 电机物理参数更新槽
 * @param motor 电机物理参数结构
 */
void motor_config_widget_t::slot_on_motor_updated(const motor_physical_t &motor)
{
    m_pole_pairs_spin->setValue(motor.pole_pairs);
    m_phase_r_spin->setValue(motor.phase_resistance);
    m_phase_l_spin->setValue(motor.phase_inductance);
    m_torque_const_spin->setValue(motor.torque_constant);
}

/**
 * @brief 限制参数更新槽
 * @param limit 限制参数结构
 */
void motor_config_widget_t::slot_on_limit_updated(const limit_param_t &limit)
{
    m_current_limit_spin->setValue(limit.current_limit);
    m_velocity_limit_spin->setValue(limit.velocity_limit);
    m_voltage_limit_spin->setValue(limit.voltage_limit);
}

/**
 * @brief 读取电机参数按钮点击
 */
void motor_config_widget_t::slot_read_motor_clicked()
{
    m_manager->read_motor_params();
}

/**
 * @brief 写入电机参数按钮点击
 */
void motor_config_widget_t::slot_write_motor_clicked()
{
    motor_physical_t motor;
    motor.pole_pairs = m_pole_pairs_spin->value();
    motor.phase_resistance = m_phase_r_spin->value();
    motor.phase_inductance = m_phase_l_spin->value();
    motor.torque_constant = m_torque_const_spin->value();
    m_manager->write_motor_params(motor);
}

/**
 * @brief 读取限制参数按钮点击
 */
void motor_config_widget_t::slot_read_limit_clicked()
{
    m_manager->read_limit_params();
}

/**
 * @brief 写入限制参数按钮点击
 */
void motor_config_widget_t::slot_write_limit_clicked()
{
    limit_param_t limit;
    limit.current_limit = m_current_limit_spin->value();
    limit.velocity_limit = m_velocity_limit_spin->value();
    limit.voltage_limit = m_voltage_limit_spin->value();
    m_manager->write_limit_params(limit);
}

/**
 * @brief 控制模式切换
 * @param index 模式索引 (0:力矩, 1:速度, 2:位置)
 */
void motor_config_widget_t::slot_mode_changed(int index)
{
    m_manager->write_control_mode(static_cast<control_mode_E>(index));
}

/**
 * @brief 保存配置到文件按钮点击
 */
void motor_config_widget_t::slot_save_config_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(this, "保存配置", "", "JSON文件 (*.json)");
    if (!file_path.isEmpty()) {
        m_manager->save_to_file(file_path);
    }
}

/**
 * @brief 从文件加载配置按钮点击
 */
void motor_config_widget_t::slot_load_config_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "加载配置", "", "JSON文件 (*.json)");
    if (!file_path.isEmpty()) {
        m_manager->load_from_file(file_path);
    }
}
