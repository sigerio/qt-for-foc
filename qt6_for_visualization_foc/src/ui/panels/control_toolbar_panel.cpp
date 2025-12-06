#include "control_toolbar_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileDialog>

control_toolbar_panel::control_toolbar_panel(QWidget* parent) : QWidget(parent) {
    setup_ui();
}

void control_toolbar_panel::setup_ui() {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);
    
    // 运行控制组
    auto* run_group = new QGroupBox("仿真控制", this);
    auto* run_layout = new QHBoxLayout(run_group);
    run_layout->setContentsMargins(5, 5, 5, 5);
    
    m_btn_run = new QPushButton("▶ 运行", run_group);
    m_btn_run->setCheckable(true);
    connect(m_btn_run, &QPushButton::clicked, this, &control_toolbar_panel::on_run_clicked);
    run_layout->addWidget(m_btn_run);
    
    m_btn_step = new QPushButton("⏭ 单步", run_group);
    connect(m_btn_step, &QPushButton::clicked, this, &control_toolbar_panel::on_step_clicked);
    run_layout->addWidget(m_btn_step);
    
    m_btn_reset = new QPushButton("⟲ 复位", run_group);
    connect(m_btn_reset, &QPushButton::clicked, this, &control_toolbar_panel::on_reset_clicked);
    run_layout->addWidget(m_btn_reset);
    
    layout->addWidget(run_group);
    
    // 电机类型选择
    auto* motor_group = new QGroupBox("电机类型", this);
    auto* motor_layout = new QHBoxLayout(motor_group);
    motor_layout->setContentsMargins(5, 5, 5, 5);
    
    m_combo_motor = new QComboBox(motor_group);
    m_combo_motor->addItem("PMSM");
    m_combo_motor->addItem("BLDC");
    connect(m_combo_motor, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &control_toolbar_panel::on_motor_type_changed);
    motor_layout->addWidget(m_combo_motor);
    
    layout->addWidget(motor_group);
    
    // 控制模式选择
    auto* mode_group = new QGroupBox("控制模式", this);
    auto* mode_layout = new QHBoxLayout(mode_group);
    mode_layout->setContentsMargins(5, 5, 5, 5);
    
    m_combo_mode = new QComboBox(mode_group);
    m_combo_mode->addItem("FOC");
    m_combo_mode->addItem("六步换向");
    connect(m_combo_mode, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &control_toolbar_panel::on_control_mode_changed);
    mode_layout->addWidget(m_combo_mode);
    
    layout->addWidget(mode_group);
    
    // 仿真速度选择
    auto* speed_group = new QGroupBox("仿真速度", this);
    auto* speed_layout = new QHBoxLayout(speed_group);
    speed_layout->setContentsMargins(5, 5, 5, 5);
    
    m_combo_speed = new QComboBox(speed_group);
    m_combo_speed->addItem("1x", 1.0);
    m_combo_speed->addItem("2x", 2.0);
    m_combo_speed->addItem("5x", 5.0);
    m_combo_speed->addItem("10x", 10.0);
    connect(m_combo_speed, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &control_toolbar_panel::on_speed_changed);
    speed_layout->addWidget(m_combo_speed);
    
    layout->addWidget(speed_group);
    
    // 配置管理
    auto* config_group = new QGroupBox("配置管理", this);
    auto* config_layout = new QVBoxLayout(config_group);
    config_layout->setContentsMargins(5, 5, 5, 5);
    
    m_btn_load_config = new QPushButton("导入配置", config_group);
    connect(m_btn_load_config, &QPushButton::clicked, this, &control_toolbar_panel::on_load_config_clicked);
    config_layout->addWidget(m_btn_load_config);
    
    m_btn_reset_config = new QPushButton("恢复默认", config_group);
    connect(m_btn_reset_config, &QPushButton::clicked, this, &control_toolbar_panel::on_reset_config_clicked);
    config_layout->addWidget(m_btn_reset_config);
    
    layout->addWidget(config_group);
    
    // 窗口开关按钮组
    auto* window_group = new QGroupBox("窗口管理", this);
    auto* window_layout = new QVBoxLayout(window_group);
    window_layout->setContentsMargins(5, 5, 5, 5);
    
    m_btn_toggle_wave = new QPushButton("📊 波形显示", window_group);
    m_btn_toggle_wave->setCheckable(true);
    m_btn_toggle_wave->setChecked(true);  // 默认显示波形窗口
    connect(m_btn_toggle_wave, &QPushButton::clicked, this, &control_toolbar_panel::on_toggle_wave_clicked);
    window_layout->addWidget(m_btn_toggle_wave);
    
    m_btn_toggle_algo = new QPushButton("📖 算法说明", window_group);
    m_btn_toggle_algo->setCheckable(true);
    connect(m_btn_toggle_algo, &QPushButton::clicked, this, &control_toolbar_panel::on_toggle_algo_clicked);
    window_layout->addWidget(m_btn_toggle_algo);
    
    layout->addWidget(window_group);
    
    // 状态标签
    m_label_status = new QLabel("就绪", this);
    m_label_status->setAlignment(Qt::AlignCenter);
    m_label_status->setStyleSheet("color: gray; font-size: 10px;");
    layout->addWidget(m_label_status);
    
    layout->addStretch();
}

void control_toolbar_panel::on_run_clicked() {
    m_running = !m_running;
    m_btn_run->setText(m_running ? "⏸ 暂停" : "▶ 运行");
    m_label_status->setText(m_running ? "运行中..." : "已暂停");
    emit run_state_changed(m_running);
}

void control_toolbar_panel::on_step_clicked() {
    m_label_status->setText("单步执行");
    emit step_requested();
}

void control_toolbar_panel::on_reset_clicked() {
    m_running = false;
    m_btn_run->setChecked(false);
    m_btn_run->setText("▶ 运行");
    m_label_status->setText("已复位");
    emit reset_requested();
}

void control_toolbar_panel::on_motor_type_changed(int index) {
    emit motor_type_changed(index);
    m_label_status->setText(index == 0 ? "PMSM模式" : "BLDC模式");
}

void control_toolbar_panel::on_control_mode_changed(int index) {
    emit control_mode_changed(index);
    m_label_status->setText(index == 0 ? "FOC控制" : "六步换向");
}

void control_toolbar_panel::on_speed_changed(int index) {
    double ratio = m_combo_speed->currentData().toDouble();
    emit speed_ratio_changed(ratio);
    m_label_status->setText(QString("速度: %1x").arg(ratio));
}

void control_toolbar_panel::on_load_config_clicked() {
    QString path = QFileDialog::getOpenFileName(this, "导入配置文件", 
                                                QString(), "JSON文件 (*.json)");
    if (!path.isEmpty()) {
        emit config_load_requested(path);
        m_label_status->setText("配置已加载");
    }
}

void control_toolbar_panel::on_reset_config_clicked() {
    emit config_reset_requested();
    m_label_status->setText("已恢复默认");
}

void control_toolbar_panel::on_toggle_algo_clicked() {
    emit toggle_algo_window();
}

void control_toolbar_panel::on_toggle_wave_clicked() {
    emit toggle_wave_window();
}
