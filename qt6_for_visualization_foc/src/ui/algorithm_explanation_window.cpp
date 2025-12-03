#include "algorithm_explanation_window.h"
#include "panels/motor_model_explanation_panel.h"
#include "panels/transform_explanation_panel.h"
#include "panels/svpwm_explanation_panel.h"
#include "panels/control_loop_explanation_panel.h"
#include <QLabel>
#include <QHBoxLayout>

algorithm_explanation_window::algorithm_explanation_window(QWidget* parent)
    : QMainWindow(parent)
{
    setup_ui();
}

algorithm_explanation_window::~algorithm_explanation_window() = default;

void algorithm_explanation_window::setup_ui()
{
    setWindowTitle("算法说明");
    setMinimumSize(1200, 900);
    
    // 创建滚动区域
    m_scroll_area = new QScrollArea(this);
    m_scroll_area->setWidgetResizable(true);
    m_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(m_scroll_area);
    
    // 创建内容容器
    m_content_widget = new QWidget();
    m_main_layout = new QVBoxLayout(m_content_widget);
    m_main_layout->setSpacing(10);
    m_main_layout->setContentsMargins(10, 10, 10, 10);
    
    // 添加标题
    QLabel* title = new QLabel("<h2>FOC算法原理说明</h2>");
    title->setAlignment(Qt::AlignCenter);
    title->setMaximumHeight(35);
    m_main_layout->addWidget(title);
    
    // 创建四个说明面板
    m_motor_panel = new motor_model_explanation_panel();
    m_transform_panel = new transform_explanation_panel();
    m_svpwm_panel = new svpwm_explanation_panel();
    m_control_panel = new control_loop_explanation_panel();
    
    // 第一行：电机模型 | 坐标变换 | SVPWM（三个并排，较小）
    QHBoxLayout* row1 = new QHBoxLayout();
    row1->setSpacing(8);
    row1->addWidget(m_motor_panel, 1);
    row1->addWidget(m_transform_panel, 1);
    row1->addWidget(m_svpwm_panel, 1);
    m_main_layout->addLayout(row1, 2);
    
    // 第二行：三环控制（单独一行，占更大空间）
    m_control_panel->setMinimumHeight(450);
    m_main_layout->addWidget(m_control_panel, 3);
    
    m_scroll_area->setWidget(m_content_widget);
}
