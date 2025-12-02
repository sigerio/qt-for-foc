#include "motor_state_panel.h"
#include <QVBoxLayout>
#include <QLabel>

motor_state_panel::motor_state_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    
    auto* title = new QLabel("电机状态", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    layout->addWidget(title);
    
    auto* content = new QLabel("转子动画\n三相电流波形\n\n转速: - rad/s\n角度: - rad\n转矩: - N·m\n\n[待实现]", this);
    layout->addWidget(content, 1);
}

void motor_state_panel::update_display() {}
