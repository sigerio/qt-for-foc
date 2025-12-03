#include "step_indicator.h"
#include <QGroupBox>

const char* step_indicator::step_names[] = {
    "空闲",
    "1. 采样 (ADC)",
    "2. Clark变换",
    "3. Park变换",
    "4. 电流环PI",
    "5. 速度环PI",
    "6. 位置环PID",
    "7. 逆Park变换",
    "8. SVPWM",
    "9. PWM输出",
    "10. 电机模型"
};

step_indicator::step_indicator(QWidget* parent) : QWidget(parent) {
    setup_ui();
}

void step_indicator::setup_ui() {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(2);
    
    auto* group = new QGroupBox("FOC算法流程", this);
    auto* group_layout = new QVBoxLayout(group);
    group_layout->setSpacing(1);
    
    // 创建步骤标签（跳过IDLE）
    for (int i = 1; i <= 10; ++i) {
        auto* label = new QLabel(step_names[i], this);
        label->setStyleSheet("QLabel { padding: 3px; border-radius: 2px; }");
        label->setMinimumHeight(20);
        group_layout->addWidget(label);
        m_labels.push_back(label);
    }
    
    layout->addWidget(group);
    layout->addStretch();
    
    // 初始状态
    update_highlight();
}

void step_indicator::set_current_step(e_foc_step step) {
    m_current = step;
    update_highlight();
}

void step_indicator::clear_highlight() {
    m_current = e_foc_step::IDLE;
    update_highlight();
}

void step_indicator::update_highlight() {
    // 步骤索引映射（跳过IDLE）
    int highlight_idx = -1;
    switch (m_current) {
        case e_foc_step::SAMPLING:      highlight_idx = 0; break;
        case e_foc_step::CLARK:         highlight_idx = 1; break;
        case e_foc_step::PARK:          highlight_idx = 2; break;
        case e_foc_step::CURRENT_LOOP:  highlight_idx = 3; break;
        case e_foc_step::VELOCITY_LOOP: highlight_idx = 4; break;
        case e_foc_step::POSITION_LOOP: highlight_idx = 5; break;
        case e_foc_step::INV_PARK:      highlight_idx = 6; break;
        case e_foc_step::SVPWM:         highlight_idx = 7; break;
        case e_foc_step::OUTPUT:        highlight_idx = 8; break;
        case e_foc_step::MOTOR_MODEL:   highlight_idx = 9; break;
        default: break;
    }
    
    // 更新样式
    for (size_t i = 0; i < m_labels.size(); ++i) {
        if (static_cast<int>(i) == highlight_idx) {
            m_labels[i]->setStyleSheet(
                "QLabel { padding: 3px; border-radius: 2px; "
                "background-color: #4CAF50; color: white; font-weight: bold; }");
        } else {
            m_labels[i]->setStyleSheet(
                "QLabel { padding: 3px; border-radius: 2px; "
                "background-color: #f0f0f0; color: #333; }");
        }
    }
}
