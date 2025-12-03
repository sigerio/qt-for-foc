#include "control_loop_explanation_panel.h"
#include "../widgets/cascade_widget.h"
#include <QVBoxLayout>
#include <QLabel>

control_loop_explanation_panel::control_loop_explanation_panel(QWidget* parent)
    : QGroupBox("四、三环控制", parent)
{
    setup_ui();
}

void control_loop_explanation_panel::setup_ui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(5);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // PID公式（简洁版）
    m_pid_formula = new QLabel(
        "<b>PID:</b> u=K<sub>p</sub>·e + K<sub>i</sub>·∫e + K<sub>d</sub>·de/dt "
        "<span style='color:#666;'>(比例+积分+微分)</span>"
    );
    m_pid_formula->setTextFormat(Qt::RichText);
    layout->addWidget(m_pid_formula);
    
    // 三环闭环控制流程图（文本标签方式）
    m_structure_widget = new cascade_widget();
    layout->addWidget(m_structure_widget, 1);
}
