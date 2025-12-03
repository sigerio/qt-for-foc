#include "circuit_widget.h"
#include <QPainter>

circuit_widget::circuit_widget(QWidget* parent) : QWidget(parent)
{
    setMinimumHeight(120);
    setStyleSheet("background-color: white; border: 1px solid #ccc;");
}

void circuit_widget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int cx = width() / 2;
    int cy = height() / 2;
    
    painter.setPen(QPen(Qt::black, 2));
    painter.setFont(QFont("Arial", 10));
    
    // d轴电路
    painter.drawText(cx - 150, cy - 30, "d轴:");
    painter.drawRect(cx - 100, cy - 45, 30, 20);
    painter.drawText(cx - 95, cy - 30, "Rs");
    painter.drawRect(cx - 50, cy - 45, 30, 20);
    painter.drawText(cx - 45, cy - 30, "Ld");
    painter.drawLine(cx - 100, cy - 35, cx - 130, cy - 35);
    painter.drawLine(cx - 20, cy - 35, cx + 10, cy - 35);
    
    // q轴电路
    painter.drawText(cx - 150, cy + 30, "q轴:");
    painter.drawRect(cx - 100, cy + 15, 30, 20);
    painter.drawText(cx - 95, cy + 30, "Rs");
    painter.drawRect(cx - 50, cy + 15, 30, 20);
    painter.drawText(cx - 45, cy + 30, "Lq");
    painter.drawLine(cx - 100, cy + 25, cx - 130, cy + 25);
    painter.drawLine(cx - 20, cy + 25, cx + 10, cy + 25);
    
    // 反电动势
    painter.drawEllipse(cx + 30, cy - 20, 35, 35);
    painter.drawText(cx + 40, cy + 3, "ψf");
    
    // 标注
    painter.drawText(cx + 80, cy - 25, "Ud");
    painter.drawText(cx + 80, cy + 35, "Uq");
}
