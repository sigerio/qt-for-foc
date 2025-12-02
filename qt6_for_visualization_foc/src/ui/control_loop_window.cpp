#include "control_loop_window.h"
#include "panels/current_loop_panel.h"
#include "panels/velocity_loop_panel.h"
#include "panels/position_loop_panel.h"
#include <QVBoxLayout>

control_loop_window::control_loop_window(QWidget* parent)
    : QMainWindow(parent)
{
    setup_ui();
    setup_connections();
}

control_loop_window::~control_loop_window() = default;

void control_loop_window::setup_ui()
{
    setWindowTitle("三环控制");
    setMinimumSize(800, 600);

    // 使用QSplitter纵向排列三个面板
    m_splitter = new QSplitter(Qt::Vertical, this);
    setCentralWidget(m_splitter);

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
}

void control_loop_window::setup_connections()
{
}

void control_loop_window::update_waveform()
{
    if (m_current_panel) m_current_panel->update_waveform();
    if (m_velocity_panel) m_velocity_panel->update_waveform();
    if (m_position_panel) m_position_panel->update_waveform();
}
