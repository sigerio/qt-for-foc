#include "svpwm_explanation_panel.h"
#include "../widgets/hexagon_widget.h"
#include <QVBoxLayout>
#include <QLabel>

svpwm_explanation_panel::svpwm_explanation_panel(QWidget* parent)
    : QGroupBox("三、SVPWM算法", parent)
{
    setup_ui();
}

void svpwm_explanation_panel::setup_ui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(5);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // 扇区判断公式
    m_sector_formula = new QLabel(
        "<b>扇区判断：</b>根据U<sub>α</sub>和U<sub>β</sub>判断电压矢量所在扇区<br>"
        "&nbsp;&nbsp;U<sub>1</sub>=U<sub>β</sub>, "
        "U<sub>2</sub>=(√3·U<sub>α</sub>-U<sub>β</sub>)/2, "
        "U<sub>3</sub>=(-√3·U<sub>α</sub>-U<sub>β</sub>)/2"
    );
    m_sector_formula->setTextFormat(Qt::RichText);
    m_sector_formula->setWordWrap(true);
    layout->addWidget(m_sector_formula);
    
    // 占空比计算公式
    m_duty_formula = new QLabel(
        "<b>占空比计算：</b>T<sub>4</sub>=√3·T<sub>s</sub>·U<sub>β</sub>/U<sub>dc</sub>, "
        "T<sub>6</sub>=√3·T<sub>s</sub>·(√3·U<sub>α</sub>/2-U<sub>β</sub>/2)/U<sub>dc</sub>, "
        "T<sub>0</sub>=T<sub>s</sub>-T<sub>4</sub>-T<sub>6</sub>"
    );
    m_duty_formula->setTextFormat(Qt::RichText);
    m_duty_formula->setWordWrap(true);
    layout->addWidget(m_duty_formula);
    
    // 六边形示意图
    QLabel* hex_title = new QLabel("<b>六边形矢量图：</b>");
    layout->addWidget(hex_title);
    
    m_hexagon_widget = new hexagon_widget();
    layout->addWidget(m_hexagon_widget, 1);
}
