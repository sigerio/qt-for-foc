#include "transform_explanation_panel.h"
#include "../widgets/coordinate_widget.h"
#include <QVBoxLayout>
#include <QLabel>

transform_explanation_panel::transform_explanation_panel(QWidget* parent)
    : QGroupBox("二、坐标变换", parent)
{
    setup_ui();
}

void transform_explanation_panel::setup_ui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(5);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // Clark变换公式
    m_clark_formula = new QLabel(
        "<b>Clark变换 (abc→αβ)：</b>"
        "I<sub>α</sub>=I<sub>a</sub>, "
        "I<sub>β</sub>=(I<sub>a</sub>+2I<sub>b</sub>)/√3"
    );
    m_clark_formula->setTextFormat(Qt::RichText);
    m_clark_formula->setWordWrap(true);
    layout->addWidget(m_clark_formula);
    
    // Park变换公式
    m_park_formula = new QLabel(
        "<b>Park变换 (αβ→dq)：</b>"
        "I<sub>d</sub>=I<sub>α</sub>·cosθ+I<sub>β</sub>·sinθ, "
        "I<sub>q</sub>=-I<sub>α</sub>·sinθ+I<sub>β</sub>·cosθ "
        "<span style='color:#666;'>(θ为电角度)</span>"
    );
    m_park_formula->setTextFormat(Qt::RichText);
    m_park_formula->setWordWrap(true);
    layout->addWidget(m_park_formula);
    
    // 坐标系示意图
    QLabel* coord_title = new QLabel("<b>坐标系关系：</b>");
    layout->addWidget(coord_title);
    
    m_coord_widget = new coordinate_widget();
    layout->addWidget(m_coord_widget, 1);
}
