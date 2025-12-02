#include "velocity_loop_panel.h"
#include <QVBoxLayout>
#include <QLabel>

velocity_loop_panel::velocity_loop_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    
    auto* title = new QLabel("速度环", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    layout->addWidget(title);
    
    auto* content = new QLabel("速度跟踪波形\n\nKp: -  Ki: -  Kd: -\n目标: - rad/s\n\n[待实现]", this);
    layout->addWidget(content, 1);
}

void velocity_loop_panel::update_waveform() {}
