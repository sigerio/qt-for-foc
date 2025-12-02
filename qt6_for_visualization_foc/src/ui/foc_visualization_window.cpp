#include "foc_visualization_window.h"
#include "panels/coord_transform_panel.h"
#include "panels/svpwm_panel.h"
#include "panels/motor_state_panel.h"
#include <QVBoxLayout>

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
    setMinimumSize(800, 600);

    // 使用QSplitter纵向排列三个面板
    m_splitter = new QSplitter(Qt::Vertical, this);
    setCentralWidget(m_splitter);

    // 创建三个面板
    m_coord_panel = new coord_transform_panel(this);
    m_svpwm_panel = new svpwm_panel(this);
    m_motor_panel = new motor_state_panel(this);

    // 添加到分割器
    m_splitter->addWidget(m_coord_panel);
    m_splitter->addWidget(m_svpwm_panel);
    m_splitter->addWidget(m_motor_panel);

    // 设置初始比例
    m_splitter->setSizes({400, 400, 400});
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
