#include "coordinate_widget.h"
#include <QPainter>
#include <cmath>

coordinate_widget::coordinate_widget(QWidget* parent) : QWidget(parent)
{
    setMinimumHeight(150);
    setStyleSheet("background-color: white; border: 1px solid #ccc;");
}

void coordinate_widget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int w = width();
    int h = height();
    int radius = qMin(w / 5, h / 2 - 30);
    
    painter.setFont(QFont("Arial", 9));
    
    // abc坐标系（左侧）
    int ax1 = w / 6;
    int cy = h / 2;
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(ax1, cy, ax1, cy - radius);
    painter.drawText(ax1 - 5, cy - radius - 5, "a");
    painter.drawLine(ax1, cy, ax1 + radius * 0.866, cy + radius * 0.5);
    painter.drawText(ax1 + radius * 0.866 + 2, cy + radius * 0.5 + 5, "b");
    painter.drawLine(ax1, cy, ax1 - radius * 0.866, cy + radius * 0.5);
    painter.drawText(ax1 - radius * 0.866 - 12, cy + radius * 0.5 + 5, "c");
    painter.drawText(ax1 - 15, h - 10, "abc");
    
    // αβ坐标系（中间）
    int ax2 = w / 2;
    painter.setPen(QPen(Qt::blue, 2));
    painter.drawLine(ax2, cy, ax2 + radius, cy);
    painter.drawText(ax2 + radius + 3, cy + 5, "α");
    painter.drawLine(ax2, cy, ax2, cy - radius);
    painter.drawText(ax2 + 3, cy - radius - 3, "β");
    painter.drawText(ax2 - 10, h - 10, "αβ");
    
    // dq坐标系（右侧，带旋转）
    int ax3 = w * 5 / 6;
    double theta = M_PI / 6;
    painter.setPen(QPen(Qt::red, 2));
    painter.drawLine(ax3, cy, ax3 + radius * cos(theta), cy - radius * sin(theta));
    painter.drawText(ax3 + radius * cos(theta) + 3, cy - radius * sin(theta), "d");
    painter.drawLine(ax3, cy, ax3 - radius * sin(theta), cy - radius * cos(theta));
    painter.drawText(ax3 - radius * sin(theta) - 5, cy - radius * cos(theta) - 3, "q");
    painter.drawText(ax3 - 10, h - 10, "dq");
    
    // 箭头标注
    painter.setPen(QPen(Qt::darkGreen, 1));
    painter.drawText(w / 3 - 15, 15, "Clark →");
    painter.drawText(w * 2 / 3 - 15, 15, "Park →");
}
