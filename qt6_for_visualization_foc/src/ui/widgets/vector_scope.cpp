#include "vector_scope.h"
#include <QPainter>
#include <QPolygonF>
#include <cmath>

vector_scope::vector_scope(QWidget* parent) : QWidget(parent) {
    setMinimumSize(200, 200);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void vector_scope::set_vector(double alpha, double beta) {
    m_alpha = alpha;
    m_beta = beta;
    if (m_show_trail) {
        m_trail.push_back({alpha, beta});
        while (m_trail.size() > m_max_trail) {
            m_trail.pop_front();
        }
    }
    update();
}

void vector_scope::set_range(double range) {
    m_range = range;
    update();
}

void vector_scope::set_color(const QColor& color) {
    m_color = color;
    update();
}

void vector_scope::set_show_trail(bool show) {
    m_show_trail = show;
    if (!show) clear_trail();
}

void vector_scope::clear_trail() {
    m_trail.clear();
    update();
}

void vector_scope::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    
    int w = width();
    int h = height();
    int size = qMin(w, h) - 20;  // 留出边距
    int cx = w / 2;
    int cy = h / 2;
    int r = size / 2;
    
    // 绘制背景圆
    p.setPen(QPen(QColor(200, 200, 200), 1));
    p.setBrush(QColor(250, 250, 250));
    p.drawEllipse(QPoint(cx, cy), r, r);
    
    // 绘制同心圆（刻度线）
    p.setPen(QPen(QColor(220, 220, 220), 1, Qt::DotLine));
    for (int i = 1; i <= 4; ++i) {
        p.drawEllipse(QPoint(cx, cy), r * i / 4, r * i / 4);
    }
    
    // 绘制坐标轴
    p.setPen(QPen(QColor(180, 180, 180), 1));
    p.drawLine(cx - r, cy, cx + r, cy);  // α轴
    p.drawLine(cx, cy - r, cx, cy + r);  // β轴
    
    // 绘制轴标签
    p.setPen(Qt::black);
    QFont font = p.font();
    font.setPointSize(9);
    p.setFont(font);
    p.drawText(cx + r + 3, cy + 4, "α");
    p.drawText(cx - 4, cy - r - 5, "β");
    
    // 坐标转换函数
    auto to_screen = [&](double alpha, double beta) -> QPointF {
        double x = cx + (alpha / m_range) * r;
        double y = cy - (beta / m_range) * r;  // Y轴向上为正
        return QPointF(x, y);
    };
    
    // 绘制轨迹
    if (m_show_trail && m_trail.size() > 1) {
        for (size_t i = 1; i < m_trail.size(); ++i) {
            int alpha_val = static_cast<int>(255 * i / m_trail.size());
            QColor trail_color = m_color;
            trail_color.setAlpha(alpha_val);
            p.setPen(QPen(trail_color, 1));
            QPointF p1 = to_screen(m_trail[i-1].first, m_trail[i-1].second);
            QPointF p2 = to_screen(m_trail[i].first, m_trail[i].second);
            p.drawLine(p1, p2);
        }
    }
    
    // 绘制当前矢量箭头
    QPointF tip = to_screen(m_alpha, m_beta);
    p.setPen(QPen(m_color, 2));
    p.drawLine(QPointF(cx, cy), tip);
    
    // 绘制箭头头部
    double angle = std::atan2(cy - tip.y(), tip.x() - cx);
    double arrow_len = 8;
    double arrow_angle = M_PI / 6;
    QPointF arr1(tip.x() - arrow_len * std::cos(angle - arrow_angle),
                 tip.y() + arrow_len * std::sin(angle - arrow_angle));
    QPointF arr2(tip.x() - arrow_len * std::cos(angle + arrow_angle),
                 tip.y() + arrow_len * std::sin(angle + arrow_angle));
    p.setBrush(m_color);
    p.drawPolygon(QPolygonF({tip, arr1, arr2}));
    
    // 绘制矢量端点
    p.setBrush(m_color);
    p.drawEllipse(tip, 4, 4);
}
