/**
 * @file pid_config_widget.cpp
 * @brief PID参数配置控件实现
 */

#include "pid_config_widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

pid_config_widget_t::pid_config_widget_t(param_manager_t *manager, QWidget *parent)
    : QWidget(parent)
    , m_manager(manager)
{
    setup_ui();
    
    connect(m_manager, &param_manager_t::signal_pid_updated,
            this, &pid_config_widget_t::slot_on_pid_updated);
}

pid_config_widget_t::~pid_config_widget_t()
{
}

/**
 * @brief 创建PID参数组控件
 */
QGroupBox* pid_config_widget_t::create_pid_group(const QString &title,
                                                   QDoubleSpinBox *&kp,
                                                   QDoubleSpinBox *&ki,
                                                   QDoubleSpinBox *&kd)
{
    QGroupBox *group = new QGroupBox(title, this);
    QGridLayout *layout = new QGridLayout(group);
    
    /* Kp */
    layout->addWidget(new QLabel("Kp:"), 0, 0);
    kp = new QDoubleSpinBox(this);
    kp->setRange(0, 10000);
    kp->setDecimals(4);
    kp->setSingleStep(0.1);
    layout->addWidget(kp, 0, 1);
    
    /* Ki */
    layout->addWidget(new QLabel("Ki:"), 1, 0);
    ki = new QDoubleSpinBox(this);
    ki->setRange(0, 10000);
    ki->setDecimals(4);
    ki->setSingleStep(0.01);
    layout->addWidget(ki, 1, 1);
    
    /* Kd */
    layout->addWidget(new QLabel("Kd:"), 2, 0);
    kd = new QDoubleSpinBox(this);
    kd->setRange(0, 10000);
    kd->setDecimals(6);
    kd->setSingleStep(0.001);
    layout->addWidget(kd, 2, 1);
    
    return group;
}

/**
 * @brief 初始化UI
 */
void pid_config_widget_t::setup_ui()
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    
    /* 电流环 */
    QHBoxLayout *current_layout = new QHBoxLayout();
    QGroupBox *current_group = create_pid_group("电流环PID", m_current_kp, m_current_ki, m_current_kd);
    m_current_write_btn = new QPushButton("写入", this);
    current_layout->addWidget(current_group);
    current_layout->addWidget(m_current_write_btn);
    main_layout->addLayout(current_layout);
    
    /* 速度环 */
    QHBoxLayout *velocity_layout = new QHBoxLayout();
    QGroupBox *velocity_group = create_pid_group("速度环PID", m_velocity_kp, m_velocity_ki, m_velocity_kd);
    m_velocity_write_btn = new QPushButton("写入", this);
    velocity_layout->addWidget(velocity_group);
    velocity_layout->addWidget(m_velocity_write_btn);
    main_layout->addLayout(velocity_layout);
    
    /* 位置环 */
    QHBoxLayout *position_layout = new QHBoxLayout();
    QGroupBox *position_group = create_pid_group("位置环PID", m_position_kp, m_position_ki, m_position_kd);
    m_position_write_btn = new QPushButton("写入", this);
    position_layout->addWidget(position_group);
    position_layout->addWidget(m_position_write_btn);
    main_layout->addLayout(position_layout);
    
    /* 读取按钮 */
    QHBoxLayout *btn_layout = new QHBoxLayout();
    m_read_all_btn = new QPushButton("读取所有PID参数", this);
    btn_layout->addWidget(m_read_all_btn);
    btn_layout->addStretch();
    main_layout->addLayout(btn_layout);
    
    main_layout->addStretch();
    
    /* 连接信号 */
    connect(m_read_all_btn, &QPushButton::clicked, this, &pid_config_widget_t::slot_read_clicked);
    connect(m_current_write_btn, &QPushButton::clicked, this, &pid_config_widget_t::slot_write_current_clicked);
    connect(m_velocity_write_btn, &QPushButton::clicked, this, &pid_config_widget_t::slot_write_velocity_clicked);
    connect(m_position_write_btn, &QPushButton::clicked, this, &pid_config_widget_t::slot_write_position_clicked);
}

/**
 * @brief PID参数更新槽
 */
void pid_config_widget_t::slot_on_pid_updated(const pid_config_t &pid)
{
    m_current_kp->setValue(pid.current.kp);
    m_current_ki->setValue(pid.current.ki);
    m_current_kd->setValue(pid.current.kd);
    
    m_velocity_kp->setValue(pid.velocity.kp);
    m_velocity_ki->setValue(pid.velocity.ki);
    m_velocity_kd->setValue(pid.velocity.kd);
    
    m_position_kp->setValue(pid.position.kp);
    m_position_ki->setValue(pid.position.ki);
    m_position_kd->setValue(pid.position.kd);
}

void pid_config_widget_t::slot_read_clicked()
{
    m_manager->read_pid_params();
}

void pid_config_widget_t::slot_write_current_clicked()
{
    pid_param_t pid = get_pid_from_spinboxes(m_current_kp, m_current_ki, m_current_kd);
    m_manager->write_pid_current(pid);
}

void pid_config_widget_t::slot_write_velocity_clicked()
{
    pid_param_t pid = get_pid_from_spinboxes(m_velocity_kp, m_velocity_ki, m_velocity_kd);
    m_manager->write_pid_velocity(pid);
}

void pid_config_widget_t::slot_write_position_clicked()
{
    pid_param_t pid = get_pid_from_spinboxes(m_position_kp, m_position_ki, m_position_kd);
    m_manager->write_pid_position(pid);
}

/**
 * @brief 从SpinBox获取PID参数
 */
pid_param_t pid_config_widget_t::get_pid_from_spinboxes(QDoubleSpinBox *kp, QDoubleSpinBox *ki, QDoubleSpinBox *kd)
{
    pid_param_t pid;
    pid.kp = kp->value();
    pid.ki = ki->value();
    pid.kd = kd->value();
    return pid;
}
