#include "control_loop_window.h"
#include "panels/current_loop_panel.h"
#include "panels/velocity_loop_panel.h"
#include "panels/position_loop_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QWidget>
#include <QFileDialog>

control_loop_window::control_loop_window(QWidget* parent)
    : QMainWindow(parent)
{
    setup_ui();
    setup_toolbar();
    setup_connections();
}

control_loop_window::~control_loop_window() = default;

void control_loop_window::setup_ui()
{
    setWindowTitle("三环控制");
    setMinimumSize(900, 600);

    // 主容器
    auto* central = new QWidget(this);
    auto* main_layout = new QHBoxLayout(central);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    setCentralWidget(central);
    
    // 左侧：使用QSplitter纵向排列三个面板
    m_splitter = new QSplitter(Qt::Vertical, this);

    // 创建三个控制环面板
    m_current_panel = new current_loop_panel(this);
    m_velocity_panel = new velocity_loop_panel(this);
    m_position_panel = new position_loop_panel(this);

    // 添加到分割器
    m_splitter->addWidget(m_current_panel);
    m_splitter->addWidget(m_velocity_panel);
    m_splitter->addWidget(m_position_panel);

    // 设置初始比例
    m_splitter->setSizes({400, 400, 400});
    
    main_layout->addWidget(m_splitter, 1);
}

void control_loop_window::setup_toolbar()
{
    auto* toolbar = addToolBar("仿真控制");
    toolbar->setMovable(false);
    
    // 运行/暂停按钮
    m_btn_run = new QPushButton("▶ 运行", this);
    m_btn_run->setMinimumWidth(80);
    m_btn_run->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; border-radius: 3px; }"
                             "QPushButton:hover { background-color: #45a049; }");
    toolbar->addWidget(m_btn_run);
    
    toolbar->addSeparator();
    
    // 单步按钮
    m_btn_step = new QPushButton("⏭ 单步", this);
    m_btn_step->setMinimumWidth(80);
    m_btn_step->setStyleSheet("QPushButton { background-color: #9C27B0; color: white; padding: 5px 15px; border-radius: 3px; }"
                              "QPushButton:hover { background-color: #7B1FA2; }");
    toolbar->addWidget(m_btn_step);
    
    toolbar->addSeparator();
    
    // 复位按钮
    m_btn_reset = new QPushButton("⟲ 复位", this);
    m_btn_reset->setMinimumWidth(80);
    m_btn_reset->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 5px 15px; border-radius: 3px; }"
                               "QPushButton:hover { background-color: #1976D2; }");
    toolbar->addWidget(m_btn_reset);
    
    toolbar->addSeparator();
    
    // 状态标签
    m_label_status = new QLabel("状态: 已停止", this);
    m_label_status->setStyleSheet("QLabel { color: #666; padding: 5px; }");
    toolbar->addWidget(m_label_status);
    
    toolbar->addSeparator();
    
    // 电机类型选择
    toolbar->addWidget(new QLabel("电机:", this));
    m_combo_motor = new QComboBox(this);
    m_combo_motor->addItem("PMSM永磁同步", 0);
    m_combo_motor->addItem("BLDC无刷直流", 1);
    m_combo_motor->setMinimumWidth(120);
    toolbar->addWidget(m_combo_motor);
    
    toolbar->addSeparator();
    
    // 控制模式选择
    toolbar->addWidget(new QLabel("控制:", this));
    m_combo_mode = new QComboBox(this);
    m_combo_mode->addItem("FOC矢量", 0);
    m_combo_mode->addItem("六步换向", 1);
    m_combo_mode->setMinimumWidth(100);
    toolbar->addWidget(m_combo_mode);
    
    toolbar->addSeparator();
    
    // 仿真速度选择
    toolbar->addWidget(new QLabel("速度:", this));
    m_combo_speed = new QComboBox(this);
    m_combo_speed->addItem("1x", QVariant(1.0));
    m_combo_speed->addItem("2x", QVariant(2.0));
    m_combo_speed->addItem("5x", QVariant(5.0));
    m_combo_speed->addItem("10x", QVariant(10.0));
    m_combo_speed->setMinimumWidth(60);
    toolbar->addWidget(m_combo_speed);
    
    toolbar->addSeparator();
    
    // 加载配置按钮
    m_btn_load_config = new QPushButton("📂 导入", this);
    m_btn_load_config->setMinimumWidth(70);
    m_btn_load_config->setStyleSheet("QPushButton { background-color: #607D8B; color: white; padding: 5px 10px; border-radius: 3px; }"
                                     "QPushButton:hover { background-color: #455A64; }");
    toolbar->addWidget(m_btn_load_config);
    
    // 恢复默认配置按钮
    m_btn_reset_config = new QPushButton("🔄 默认", this);
    m_btn_reset_config->setMinimumWidth(70);
    m_btn_reset_config->setStyleSheet("QPushButton { background-color: #78909C; color: white; padding: 5px 10px; border-radius: 3px; }"
                                      "QPushButton:hover { background-color: #546E7A; }");
    toolbar->addWidget(m_btn_reset_config);
    
    // 弹簧
    auto* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);
}

void control_loop_window::setup_connections()
{
    connect(m_btn_run, &QPushButton::clicked, this, &control_loop_window::on_run_clicked);
    connect(m_btn_step, &QPushButton::clicked, this, &control_loop_window::on_step_clicked);
    connect(m_btn_reset, &QPushButton::clicked, this, &control_loop_window::on_reset_clicked);
    connect(m_combo_motor, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &control_loop_window::on_motor_type_changed);
    connect(m_combo_mode, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &control_loop_window::on_control_mode_changed);
    connect(m_combo_speed, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &control_loop_window::on_speed_ratio_changed);
    connect(m_btn_load_config, &QPushButton::clicked, this, &control_loop_window::on_load_config_clicked);
    connect(m_btn_reset_config, &QPushButton::clicked, this, &control_loop_window::on_reset_config_clicked);
    
    // 连接各环路使能信号
    connect(m_current_panel, &current_loop_panel::loop_enabled_changed, 
            this, &control_loop_window::on_loop_enable_changed);
    connect(m_velocity_panel, &velocity_loop_panel::loop_enabled_changed, 
            this, &control_loop_window::on_loop_enable_changed);
    connect(m_position_panel, &position_loop_panel::loop_enabled_changed, 
            this, &control_loop_window::on_loop_enable_changed);
}

void control_loop_window::on_run_clicked()
{
    m_running = !m_running;
    if (m_running) {
        m_btn_run->setText("⏸ 暂停");
        m_btn_run->setStyleSheet("QPushButton { background-color: #FF9800; color: white; padding: 5px 15px; border-radius: 3px; }"
                                 "QPushButton:hover { background-color: #F57C00; }");
        m_label_status->setText("状态: 运行中");
        m_label_status->setStyleSheet("QLabel { color: #4CAF50; padding: 5px; font-weight: bold; }");
    } else {
        m_btn_run->setText("▶ 运行");
        m_btn_run->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; border-radius: 3px; }"
                                 "QPushButton:hover { background-color: #45a049; }");
        m_label_status->setText("状态: 已暂停");
        m_label_status->setStyleSheet("QLabel { color: #FF9800; padding: 5px; }");
    }
    emit run_state_changed(m_running);
}

void control_loop_window::on_step_clicked()
{
    // 单步执行（暂停状态下有效）
    if (!m_running) {
        m_label_status->setText("状态: 单步执行");
        m_label_status->setStyleSheet("QLabel { color: #9C27B0; padding: 5px; }");
        emit step_requested();
    }
}

void control_loop_window::on_motor_type_changed(int index)
{
    // 发送电机类型变化信号
    emit motor_type_changed(index);
    m_label_status->setText(QString("电机: %1").arg(m_combo_motor->currentText()));
}

void control_loop_window::on_control_mode_changed(int index)
{
    // 发送控制模式变化信号
    emit control_mode_changed(index);
    m_label_status->setText(QString("控制: %1").arg(m_combo_mode->currentText()));
}

void control_loop_window::on_reset_clicked()
{
    m_running = false;
    m_btn_run->setText("▶ 运行");
    m_btn_run->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 5px 15px; border-radius: 3px; }"
                             "QPushButton:hover { background-color: #45a049; }");
    m_label_status->setText("状态: 已复位");
    m_label_status->setStyleSheet("QLabel { color: #2196F3; padding: 5px; }");
    emit reset_requested();
}

void control_loop_window::on_speed_ratio_changed(int index)
{
    double ratio = m_combo_speed->currentData().toDouble();
    emit speed_ratio_changed(ratio);
    m_label_status->setText(QString("速度: %1x").arg(ratio));
}

void control_loop_window::on_load_config_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "导入配置文件", 
                                                QString(), "JSON文件 (*.json)");
    if (!path.isEmpty()) {
        emit config_load_requested(path);
        m_label_status->setText("配置已加载");
    }
}

void control_loop_window::on_reset_config_clicked()
{
    emit config_reset_requested();
    m_label_status->setText("已恢复默认配置");
}

void control_loop_window::update_waveform()
{
    if (m_current_panel) m_current_panel->update_waveform();
    if (m_velocity_panel) m_velocity_panel->update_waveform();
    if (m_position_panel) m_position_panel->update_waveform();
}

void control_loop_window::on_loop_enable_changed()
{
    // 获取各环路使能状态
    bool current_enabled = m_current_panel && m_current_panel->is_enabled();
    bool velocity_enabled = m_velocity_panel && m_velocity_panel->is_enabled();
    bool position_enabled = m_position_panel && m_position_panel->is_enabled();
    
    // 根据使能状态确定预设类型
    QString preset;
    if (current_enabled && velocity_enabled && position_enabled) {
        preset = "full";
    } else if (current_enabled && velocity_enabled) {
        preset = "current_velocity";
    } else if (current_enabled) {
        preset = "current";
    }
    
    // 发出信号
    if (!preset.isEmpty()) {
        emit loop_preset_changed(preset);
    }
}
