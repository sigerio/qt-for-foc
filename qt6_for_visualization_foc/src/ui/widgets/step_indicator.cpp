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
    // 步骤到索引映射表（IDLE=-1, SAMPLING=0, ..., MOTOR_MODEL=9）
    static constexpr int STEP_MAP[] = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int idx = static_cast<int>(m_current);
    int highlight = (idx >= 0 && idx < 11) ? STEP_MAP[idx] : -1;
    
    static const char* ON_STYLE = "QLabel { padding: 3px; border-radius: 2px; "
                                   "background-color: #4CAF50; color: white; font-weight: bold; }";
    static const char* OFF_STYLE = "QLabel { padding: 3px; border-radius: 2px; "
                                    "background-color: #f0f0f0; color: #333; }";
    for (size_t i = 0; i < m_labels.size(); ++i)
        m_labels[i]->setStyleSheet(static_cast<int>(i) == highlight ? ON_STYLE : OFF_STYLE);
}
