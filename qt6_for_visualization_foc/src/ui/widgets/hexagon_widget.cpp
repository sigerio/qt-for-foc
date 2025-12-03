#include "hexagon_widget.h"
#include <QPainter>
#include <cmath>

hexagon_widget::hexagon_widget(QWidget* parent) : QWidget(parent)
{
    setMinimumHeight(180);
    setStyleSheet("background-color: white; border: 1px solid #ccc;");
}

void hexagon_widget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int cx = width() / 2;
    int cy = height() / 2;
    int radius = qMin(width(), height()) / 2 - 40;
    
    painter.setFont(QFont("Arial", 9));
    
    // 计算六边形顶点
    QPolygonF hexagon;
    for (int i = 0; i < 6; ++i) {
        double angle = i * M_PI / 3;
        hexagon << QPointF(cx + radius * cos(angle), cy - radius * sin(angle));
    }
    
    // 绘制六边形
    painter.setPen(QPen(Qt::blue, 2));
    painter.drawPolygon(hexagon);
    
    // 绘制从原点到顶点的矢量
    painter.setPen(QPen(Qt::red, 2));
    for (int i = 0; i < 6; ++i) {
        painter.drawLine(cx, cy, hexagon[i].x(), hexagon[i].y());
    }
    
    // 标注基本矢量
    QStringList vectors = {"V4(100)", "V6(110)", "V2(010)", "V3(011)", "V1(001)", "V5(101)"};
    painter.setPen(Qt::black);
    for (int i = 0; i < 6; ++i) {
        double angle = i * M_PI / 3;
        int tx = cx + (radius + 20) * cos(angle) - 20;
        int ty = cy - (radius + 20) * sin(angle) + 5;
        painter.drawText(tx, ty, vectors[i]);
    }
    
    // 标注扇区
    painter.setPen(QPen(Qt::darkGreen, 1));
    for (int i = 0; i < 6; ++i) {
        double angle = (i + 0.5) * M_PI / 3;
        int tx = cx + radius * 0.5 * cos(angle) - 5;
        int ty = cy - radius * 0.5 * sin(angle) + 5;
        painter.drawText(tx, ty, QString::number(i + 1));
    }
}
