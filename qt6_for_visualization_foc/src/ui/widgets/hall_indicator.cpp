#include "hall_indicator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

hall_indicator::hall_indicator(QWidget* parent) : QWidget(parent) {
    setup_ui();
}

void hall_indicator::setup_ui() {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);
    
    auto* group = new QGroupBox("霍尔传感器", this);
    auto* group_layout = new QVBoxLayout(group);
    group_layout->setSpacing(8);
    
    // 霍尔传感器LED指示灯
    auto* led_layout = new QHBoxLayout();
    led_layout->setSpacing(10);
    
    // HA
    auto* ha_layout = new QVBoxLayout();
    m_led_ha = new QLabel("●", this);
    m_led_ha->setAlignment(Qt::AlignCenter);
    m_led_ha->setStyleSheet("QLabel { font-size: 24px; color: #888; }");
    ha_layout->addWidget(m_led_ha);
    auto* label_ha = new QLabel("HA", this);
    label_ha->setAlignment(Qt::AlignCenter);
    label_ha->setStyleSheet("QLabel { font-weight: bold; }");
    ha_layout->addWidget(label_ha);
    led_layout->addLayout(ha_layout);
    
    // HB
    auto* hb_layout = new QVBoxLayout();
    m_led_hb = new QLabel("●", this);
    m_led_hb->setAlignment(Qt::AlignCenter);
    m_led_hb->setStyleSheet("QLabel { font-size: 24px; color: #888; }");
    hb_layout->addWidget(m_led_hb);
    auto* label_hb = new QLabel("HB", this);
    label_hb->setAlignment(Qt::AlignCenter);
    label_hb->setStyleSheet("QLabel { font-weight: bold; }");
    hb_layout->addWidget(label_hb);
    led_layout->addLayout(hb_layout);
    
    // HC
    auto* hc_layout = new QVBoxLayout();
    m_led_hc = new QLabel("●", this);
    m_led_hc->setAlignment(Qt::AlignCenter);
    m_led_hc->setStyleSheet("QLabel { font-size: 24px; color: #888; }");
    hc_layout->addWidget(m_led_hc);
    auto* label_hc = new QLabel("HC", this);
    label_hc->setAlignment(Qt::AlignCenter);
    label_hc->setStyleSheet("QLabel { font-weight: bold; }");
    hc_layout->addWidget(label_hc);
    led_layout->addLayout(hc_layout);
    
    group_layout->addLayout(led_layout);
    
    // 分隔线
    auto* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    group_layout->addWidget(line);
    
    // 扇区和编码显示
    auto* info_layout = new QHBoxLayout();
    
    auto* sector_label = new QLabel("扇区:", this);
    info_layout->addWidget(sector_label);
    m_label_sector = new QLabel("1", this);
    m_label_sector->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #0066cc; }");
    info_layout->addWidget(m_label_sector);
    
    info_layout->addStretch();
    
    auto* code_label = new QLabel("编码:", this);
    info_layout->addWidget(code_label);
    m_label_code = new QLabel("5", this);
    m_label_code->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #006600; }");
    info_layout->addWidget(m_label_code);
    
    group_layout->addLayout(info_layout);
    
    // 换向时序表
    auto* table_label = new QLabel(
        "<table border='1' cellspacing='0' cellpadding='3' style='font-size:10px;'>"
        "<tr><th>扇区</th><th>HA</th><th>HB</th><th>HC</th><th>编码</th></tr>"
        "<tr><td>1</td><td>1</td><td>0</td><td>1</td><td>5</td></tr>"
        "<tr><td>2</td><td>1</td><td>0</td><td>0</td><td>4</td></tr>"
        "<tr><td>3</td><td>1</td><td>1</td><td>0</td><td>6</td></tr>"
        "<tr><td>4</td><td>0</td><td>1</td><td>0</td><td>2</td></tr>"
        "<tr><td>5</td><td>0</td><td>1</td><td>1</td><td>3</td></tr>"
        "<tr><td>6</td><td>0</td><td>0</td><td>1</td><td>1</td></tr>"
        "</table>", this);
    table_label->setAlignment(Qt::AlignCenter);
    group_layout->addWidget(table_label);
    
    layout->addWidget(group);
    layout->addStretch();
}

void hall_indicator::set_hall_state(const hall_state_t& state) {
    m_state = state;
    update_display();
}

void hall_indicator::clear() {
    m_state = hall_state_t{};
    update_display();
}

void hall_indicator::update_display() {
    // 更新LED颜色（高电平绿色，低电平灰色）
    QString on_style = "QLabel { font-size: 24px; color: #00cc00; }";
    QString off_style = "QLabel { font-size: 24px; color: #888888; }";
    
    m_led_ha->setStyleSheet(m_state.ha ? on_style : off_style);
    m_led_hb->setStyleSheet(m_state.hb ? on_style : off_style);
    m_led_hc->setStyleSheet(m_state.hc ? on_style : off_style);
    
    // 更新扇区和编码
    m_label_sector->setText(QString::number(m_state.sector));
    m_label_code->setText(QString::number(m_state.hall_code));
}
