#include "rotor_animator.h"
#include <QPainter>
#include <QPolygonF>
#include <cmath>

rotor_animator::rotor_animator(QWidget* parent) : QWidget(parent) {
    setMinimumSize(200, 200);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void rotor_animator::set_angle(double angle_rad) {
    m_angle = angle_rad;
    update();
}

void rotor_animator::set_pole_pairs(int pairs) {
    m_pole_pairs = pairs;
    update();
}

void rotor_animator::set_show_stator(bool show) {
    m_show_stator = show;
    update();
}

void rotor_animator::set_show_angle_label(bool show) {
    m_show_angle_label = show;
    update();
}

// 绘制定子和三相绕组
void rotor_animator::draw_stator(QPainter& p, int cx, int cy, int r_stator, int r_rotor) {
    p.setPen(QPen(QColor(100, 100, 100), 2));
    p.setBrush(QColor(240, 240, 240));
    p.drawEllipse(QPoint(cx, cy), r_stator, r_stator);
    
    if (!m_show_stator) return;
    static const QColor colors[3] = {Qt::red, Qt::green, Qt::blue};
    static const char* names[3] = {"A", "B", "C"};
    QFont font = p.font(); font.setBold(true); p.setFont(font);
    for (int i = 0; i < 3; ++i) {
        double angle = i * 2 * M_PI / 3 - M_PI / 2;
        int x1 = cx + int(r_rotor * 1.2 * std::cos(angle));
        int y1 = cy + int(r_rotor * 1.2 * std::sin(angle));
        int x2 = cx + int(r_stator * 0.9 * std::cos(angle));
        int y2 = cy + int(r_stator * 0.9 * std::sin(angle));
        p.setPen(QPen(colors[i], 3));
        p.drawLine(x1, y1, x2, y2);
        p.drawText(cx + int((r_stator + 12) * std::cos(angle)) - 5,
                   cy + int((r_stator + 12) * std::sin(angle)) + 5, names[i]);
    }
}

// 绘制转子和磁极
void rotor_animator::draw_rotor(QPainter& p, int cx, int cy, int r_rotor, int r_shaft) {
    p.setPen(QPen(QColor(60, 60, 60), 2));
    p.setBrush(QColor(200, 200, 200));
    p.drawEllipse(QPoint(cx, cy), r_rotor, r_rotor);
    
    int num_poles = m_pole_pairs * 2;
    for (int i = 0; i < num_poles; ++i) {
        double pole_angle = m_angle + i * 2 * M_PI / num_poles;
        p.setPen(Qt::NoPen);
        p.setBrush((i % 2 == 0) ? QColor(255, 100, 100) : QColor(100, 100, 255));
        int span = int(360.0 / num_poles * 16 * 0.8);
        int start = int(-pole_angle * 180 / M_PI * 16 - span / 2);
        p.drawPie(QRect(cx - r_rotor + 5, cy - r_rotor + 5, (r_rotor - 5) * 2, (r_rotor - 5) * 2), start, span);
    }
    
    p.setPen(QPen(QColor(60, 60, 60), 2));
    p.setBrush(QColor(100, 100, 100));
    p.drawEllipse(QPoint(cx, cy), r_shaft, r_shaft);
}

// 绘制位置箭头和N/S标签
void rotor_animator::draw_arrow_and_labels(QPainter& p, int cx, int cy, int r_rotor, int h) {
    int arrow_len = r_rotor - 10;
    int ax = cx + int(arrow_len * std::cos(m_angle));
    int ay = cy - int(arrow_len * std::sin(m_angle));
    
    p.setPen(QPen(Qt::darkGreen, 3));
    p.drawLine(cx, cy, ax, ay);
    
    double arr_angle = M_PI / 6;
    QPointF tip(ax, ay);
    QPointF a1(ax - 12 * std::cos(m_angle - arr_angle), ay + 12 * std::sin(m_angle - arr_angle));
    QPointF a2(ax - 12 * std::cos(m_angle + arr_angle), ay + 12 * std::sin(m_angle + arr_angle));
    p.setBrush(Qt::darkGreen);
    p.drawPolygon(QPolygonF({tip, a1, a2}));
    
    if (m_show_angle_label) {
        double deg = std::fmod(m_angle * 180 / M_PI, 360.0);
        if (deg < 0) deg += 360;
        p.setPen(Qt::black);
        p.drawText(5, h - 5, QString("θ = %1°").arg(deg, 0, 'f', 1));
    }
    
    int lr = r_rotor * 2 / 3;
    p.setPen(Qt::white);
    QFont font = p.font(); font.setBold(true); font.setPointSize(8); p.setFont(font);
    p.drawText(cx + int(lr * std::cos(m_angle)) - 5, cy - int(lr * std::sin(m_angle)) + 5, "N");
    p.drawText(cx + int(lr * std::cos(m_angle + M_PI)) - 5, cy - int(lr * std::sin(m_angle + M_PI)) + 5, "S");
}

void rotor_animator::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    
    int size = qMin(width(), height()) - 40;
    int cx = width() / 2, cy = height() / 2 + 5;
    int r_stator = size / 2, r_rotor = size * 3 / 8, r_shaft = size / 10;
    
    draw_stator(p, cx, cy, r_stator, r_rotor);
    draw_rotor(p, cx, cy, r_rotor, r_shaft);
    draw_arrow_and_labels(p, cx, cy, r_rotor, height());
}
