#include "svpwm_panel.h"
#include <QVBoxLayout>
#include <QLabel>

svpwm_panel::svpwm_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    
    auto* title = new QLabel("SVPWM空间矢量调制", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    layout->addWidget(title);
    
    auto* content = new QLabel("六边形矢量图\n扇区: -\n占空比: Ta=- Tb=- Tc=-\n\n[待实现]", this);
    layout->addWidget(content, 1);
}

void svpwm_panel::update_display() {}
