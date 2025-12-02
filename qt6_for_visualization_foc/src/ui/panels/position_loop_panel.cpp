#include "position_loop_panel.h"
#include <QVBoxLayout>
#include <QLabel>

position_loop_panel::position_loop_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    
    auto* title = new QLabel("位置环", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    layout->addWidget(title);
    
    auto* content = new QLabel("位置跟踪波形\n\nKp: -  Ki: -  Kd: -\n目标: - rad\n\n[待实现]", this);
    layout->addWidget(content, 1);
}

void position_loop_panel::update_waveform() {}
