#include "foc_visualization_window.h"
#include "algorithm_explanation_window.h"
#include "waveform_window.h"
#include "panels/coord_transform_panel.h"
#include "panels/svpwm_panel.h"
#include "panels/motor_state_panel.h"
#include "panels/motor_params_panel.h"
#include "panels/pid_config_panel.h"
#include "panels/control_toolbar_panel.h"
#include "widgets/step_indicator.h"
#include "widgets/hall_indicator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QScrollArea>

foc_visualization_window::foc_visualization_window(QWidget* parent)
    : QMainWindow(parent)
{
    setup_ui();
    setup_connections();
}

foc_visualization_window::~foc_visualization_window() = default;

void foc_visualization_window::setup_ui()
{
    setWindowTitle("FOC算法可视化 + 参数配置");
    setMinimumSize(1200, 700);

    // 主布局：左侧可视化面板 + 右侧参数配置
    auto* central = new QWidget(this);
    auto* main_layout = new QHBoxLayout(central);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(5);
    setCentralWidget(central);

    // ========== 左侧：可视化面板 ==========
    auto* left_widget = new QWidget(this);
    auto* left_layout = new QHBoxLayout(left_widget);
    left_layout->setContentsMargins(0, 0, 0, 0);
    left_layout->setSpacing(0);
    
    // 左侧主体：使用QSplitter纵向排列三行
    m_splitter = new QSplitter(Qt::Vertical, this);
    m_coord_panel = new coord_transform_panel(this);
    m_svpwm_panel = new svpwm_panel(this);
    m_splitter->addWidget(m_coord_panel);
    m_splitter->addWidget(m_svpwm_panel);
    
    // 第三行：转子位置 | FOC步骤指示器（并列）
    auto* row3_widget = new QWidget(this);
    auto* row3_layout = new QHBoxLayout(row3_widget);
    row3_layout->setContentsMargins(0, 0, 0, 0);
    row3_layout->setSpacing(5);
    
    m_motor_panel = new motor_state_panel(this);
    row3_layout->addWidget(m_motor_panel, 1);
    
    // FOC步骤指示器/霍尔指示器
    m_indicator_stack = new QStackedWidget(this);
    m_step_indicator = new step_indicator(this);
    m_indicator_stack->addWidget(m_step_indicator);
    m_hall_indicator = new hall_indicator(this);
    m_indicator_stack->addWidget(m_hall_indicator);
    m_indicator_stack->setCurrentIndex(0);
    row3_layout->addWidget(m_indicator_stack, 1);
    
    m_splitter->addWidget(row3_widget);
    m_splitter->setSizes({300, 300, 400});
    left_layout->addWidget(m_splitter, 1);
    main_layout->addWidget(left_widget, 1);

    // ========== 右侧：参数配置区 ==========
    auto* right_scroll = new QScrollArea(this);
    right_scroll->setWidgetResizable(true);
    right_scroll->setFixedWidth(280);
    right_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    auto* right_widget = new QWidget(this);
    auto* right_layout = new QVBoxLayout(right_widget);
    right_layout->setContentsMargins(5, 5, 5, 5);
    right_layout->setSpacing(5);
    
    // 控制工具栏
    m_toolbar_panel = new control_toolbar_panel(this);
    right_layout->addWidget(m_toolbar_panel);
    
    // 电机参数配置
    m_params_panel = new motor_params_panel(this);
    right_layout->addWidget(m_params_panel);
    
    // 三环PID配置
    m_pid_panel = new pid_config_panel(this);
    right_layout->addWidget(m_pid_panel);
    
    right_layout->addStretch();
    right_scroll->setWidget(right_widget);
    main_layout->addWidget(right_scroll);
}

void foc_visualization_window::setup_connections()
{
    // 连接控制工具栏的算法说明窗口开关信号
    connect(m_toolbar_panel, &control_toolbar_panel::toggle_algo_window,
            this, &foc_visualization_window::toggle_algo_window);
    // 连接控制工具栏的波形窗口开关信号
    connect(m_toolbar_panel, &control_toolbar_panel::toggle_wave_window,
            this, &foc_visualization_window::toggle_wave_window);
}

void foc_visualization_window::update_display()
{
    if (m_coord_panel) m_coord_panel->update_display();
    if (m_svpwm_panel) m_svpwm_panel->update_display();
    if (m_motor_panel) m_motor_panel->update_display();
}

void foc_visualization_window::set_control_mode(e_control_mode mode)
{
    if (m_indicator_stack) {
        // FOC模式显示步骤指示器，六步换向模式显示霍尔指示器
        m_indicator_stack->setCurrentIndex(mode == e_control_mode::FOC ? 0 : 1);
    }
}

void foc_visualization_window::toggle_algo_window()
{
    if (m_algo_window) {
        if (m_algo_window->isVisible()) {
            m_algo_window->hide();
        } else {
            m_algo_window->show();
        }
    }
}

void foc_visualization_window::toggle_wave_window()
{
    if (m_wave_window) {
        if (m_wave_window->isVisible()) {
            m_wave_window->hide();
        } else {
            m_wave_window->show();
        }
    }
}
