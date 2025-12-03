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

void rotor_animator::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    
    int w = width();
    int h = height();
    int size = qMin(w, h) - 40;  // 增加边距以容纳标签
    int cx = w / 2;
    int cy = h / 2 + 5;  // 稍微下移以为A相腾出空间
    int r_stator = size / 2;
    int r_rotor = size * 3 / 8;
    int r_shaft = size / 10;
    
    // 绘制定子外壳
    p.setPen(QPen(QColor(100, 100, 100), 2));
    p.setBrush(QColor(240, 240, 240));
    p.drawEllipse(QPoint(cx, cy), r_stator, r_stator);
    
    // 绘制三相绕组（A/B/C相，120度间隔）
    if (m_show_stator) {
        QColor phase_colors[3] = {Qt::red, Qt::green, Qt::blue};
        QString phase_names[3] = {"A", "B", "C"};
        for (int i = 0; i < 3; ++i) {
            double angle = i * 2 * M_PI / 3 - M_PI / 2;  // 从顶部开始
            int x1 = cx + static_cast<int>(r_rotor * 1.2 * std::cos(angle));
            int y1 = cy + static_cast<int>(r_rotor * 1.2 * std::sin(angle));
            int x2 = cx + static_cast<int>(r_stator * 0.9 * std::cos(angle));
            int y2 = cy + static_cast<int>(r_stator * 0.9 * std::sin(angle));
            
            // 绘制绕组线圈
            p.setPen(QPen(phase_colors[i], 3));
            p.drawLine(x1, y1, x2, y2);
            
            // 绘制相位标签
            int lx = cx + static_cast<int>((r_stator + 12) * std::cos(angle));
            int ly = cy + static_cast<int>((r_stator + 12) * std::sin(angle));
            p.setPen(phase_colors[i]);
            QFont font = p.font();
            font.setBold(true);
            p.setFont(font);
            p.drawText(lx - 5, ly + 5, phase_names[i]);
        }
    }
    
    // 绘制转子
    p.setPen(QPen(QColor(60, 60, 60), 2));
    p.setBrush(QColor(200, 200, 200));
    p.drawEllipse(QPoint(cx, cy), r_rotor, r_rotor);
    
    // 绘制转子磁极（N/S极）
    int num_poles = m_pole_pairs * 2;
    for (int i = 0; i < num_poles; ++i) {
        double pole_angle = m_angle + i * 2 * M_PI / num_poles;
        bool is_north = (i % 2 == 0);
        
        // 绘制扇形磁极
        p.setPen(Qt::NoPen);
        p.setBrush(is_north ? QColor(255, 100, 100) : QColor(100, 100, 255));
        
        int span_angle = static_cast<int>(360.0 / num_poles * 16 * 0.8);
        int start_angle = static_cast<int>(-pole_angle * 180 / M_PI * 16 - span_angle / 2);
        QRect rect(cx - r_rotor + 5, cy - r_rotor + 5, 
                   (r_rotor - 5) * 2, (r_rotor - 5) * 2);
        p.drawPie(rect, start_angle, span_angle);
    }
    
    // 绘制转子中心轴
    p.setPen(QPen(QColor(60, 60, 60), 2));
    p.setBrush(QColor(100, 100, 100));
    p.drawEllipse(QPoint(cx, cy), r_shaft, r_shaft);
    
    // 绘制转子位置指示箭头
    double arrow_angle = m_angle;
    int arrow_len = r_rotor - 10;
    int ax = cx + static_cast<int>(arrow_len * std::cos(arrow_angle));
    int ay = cy - static_cast<int>(arrow_len * std::sin(arrow_angle));
    
    p.setPen(QPen(Qt::darkGreen, 3));
    p.drawLine(cx, cy, ax, ay);
    
    // 绘制箭头头部
    double arr_angle = M_PI / 6;
    int arr_len = 12;
    QPointF tip(ax, ay);
    QPointF arr1(ax - arr_len * std::cos(arrow_angle - arr_angle),
                 ay + arr_len * std::sin(arrow_angle - arr_angle));
    QPointF arr2(ax - arr_len * std::cos(arrow_angle + arr_angle),
                 ay + arr_len * std::sin(arrow_angle + arr_angle));
    p.setBrush(Qt::darkGreen);
    p.drawPolygon(QPolygonF({tip, arr1, arr2}));
    
    // 绘制角度标注
    if (m_show_angle_label) {
        p.setPen(Qt::black);
        QFont font = p.font();
        font.setPointSize(9);
        p.setFont(font);
        double deg = m_angle * 180 / M_PI;
        while (deg < 0) deg += 360;
        while (deg >= 360) deg -= 360;
        QString text = QString("θ = %1°").arg(deg, 0, 'f', 1);
        p.drawText(5, h - 5, text);
    }
    
    // 绘制N/S标签
    p.setPen(Qt::white);
    QFont font = p.font();
    font.setBold(true);
    font.setPointSize(8);
    p.setFont(font);
    int label_r = r_rotor * 2 / 3;
    int nx = cx + static_cast<int>(label_r * std::cos(m_angle));
    int ny = cy - static_cast<int>(label_r * std::sin(m_angle));
    p.drawText(nx - 5, ny + 5, "N");
    int sx = cx + static_cast<int>(label_r * std::cos(m_angle + M_PI));
    int sy = cy - static_cast<int>(label_r * std::sin(m_angle + M_PI));
    p.drawText(sx - 5, sy + 5, "S");
}
