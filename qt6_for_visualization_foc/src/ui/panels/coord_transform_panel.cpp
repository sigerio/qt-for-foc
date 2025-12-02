#include "coord_transform_panel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>

coord_transform_panel::coord_transform_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    
    auto* title = new QLabel("坐标变换 (Clark/Park)", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    layout->addWidget(title);
    
    // 占位内容
    auto* content = new QLabel("Iα-Iβ矢量圆\n\n[待实现]", this);
    layout->addWidget(content, 1);
}

void coord_transform_panel::update_display() {}
