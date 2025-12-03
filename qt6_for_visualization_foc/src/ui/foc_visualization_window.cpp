#include "foc_visualization_window.h"
#include "panels/coord_transform_panel.h"
#include "panels/svpwm_panel.h"
#include "panels/motor_state_panel.h"
#include "panels/motor_params_panel.h"
#include "widgets/step_indicator.h"
#include "widgets/hall_indicator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

foc_visualization_window::foc_visualization_window(QWidget* parent)
    : QMainWindow(parent)
{
    setup_ui();
    setup_connections();
}

foc_visualization_window::~foc_visualization_window() = default;

void foc_visualization_window::setup_ui()
{
    setWindowTitle("FOC算法可视化");
    setMinimumSize(900, 600);

    // 主布局：左侧面板 + 右侧（指示器+电机参数）
    auto* central = new QWidget(this);
    auto* main_layout = new QHBoxLayout(central);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    setCentralWidget(central);

    // 左侧：使用QSplitter纵向排列三个面板
    m_splitter = new QSplitter(Qt::Vertical, this);

    // 创建三个面板
    m_coord_panel = new coord_transform_panel(this);
    m_svpwm_panel = new svpwm_panel(this);
    m_motor_panel = new motor_state_panel(this);

    // 添加到分割器
    m_splitter->addWidget(m_coord_panel);
    m_splitter->addWidget(m_svpwm_panel);
    m_splitter->addWidget(m_motor_panel);
    m_splitter->setSizes({400, 400, 400});
    
    main_layout->addWidget(m_splitter, 1);
    
    // 右侧：指示器 + 电机参数配置（纵向排列）
    auto* right_widget = new QWidget(this);
    right_widget->setFixedWidth(200);
    auto* right_layout = new QVBoxLayout(right_widget);
    right_layout->setContentsMargins(0, 0, 0, 0);
    right_layout->setSpacing(0);
    
    // 上方：使用QStackedWidget切换FOC步骤指示器和霍尔状态指示器
    m_indicator_stack = new QStackedWidget(this);
    
    // FOC步骤指示器（索引0）
    m_step_indicator = new step_indicator(this);
    m_indicator_stack->addWidget(m_step_indicator);
    
    // 霍尔状态指示器（索引1）
    m_hall_indicator = new hall_indicator(this);
    m_indicator_stack->addWidget(m_hall_indicator);
    
    // 默认显示FOC步骤指示器
    m_indicator_stack->setCurrentIndex(0);
    
    right_layout->addWidget(m_indicator_stack, 1);
    
    // 下方：电机参数配置面板
    m_params_panel = new motor_params_panel(this);
    right_layout->addWidget(m_params_panel);
    
    main_layout->addWidget(right_widget);
}

void foc_visualization_window::setup_connections()
{
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
