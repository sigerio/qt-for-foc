#include "waveform_view.h"
#include <QPainter>
#include <QPainterPath>
#include <algorithm>
#include <cmath>
#include <limits>

waveform_view::waveform_view(QWidget* parent) : QWidget(parent) {
    setMinimumSize(300, 100);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    // 默认单通道
    m_channels.push_back({std::deque<double>(), QColor(0, 150, 255), "CH1"});
}

void waveform_view::add_point(double value) {
    if (m_channels.empty()) return;
    m_channels[0].data.push_back(value);
    while (m_channels[0].data.size() > m_max_points) {
        m_channels[0].data.pop_front();
    }
    update();
}

void waveform_view::add_points(const std::vector<double>& values) {
    for (size_t i = 0; i < values.size() && i < m_channels.size(); ++i) {
        m_channels[i].data.push_back(values[i]);
        while (m_channels[i].data.size() > m_max_points) {
            m_channels[i].data.pop_front();
        }
    }
    update();
}

void waveform_view::set_channels(int count, const std::vector<QColor>& colors) {
    m_channels.clear();
    for (int i = 0; i < count; ++i) {
        QColor c = (i < static_cast<int>(colors.size())) ? colors[i] : Qt::blue;
        m_channels.push_back({std::deque<double>(), c, QString("CH%1").arg(i+1)});
    }
}

void waveform_view::set_channel_names(const std::vector<QString>& names) {
    for (size_t i = 0; i < names.size() && i < m_channels.size(); ++i) {
        m_channels[i].name = names[i];
    }
}

void waveform_view::set_y_range(double min_val, double max_val) {
    m_y_min = min_val;
    m_y_max = max_val;
    m_auto_scale = false;
    update();
}

void waveform_view::set_auto_scale(bool enabled) {
    m_auto_scale = enabled;
    update();
}

void waveform_view::set_max_points(size_t count) {
    m_max_points = count;
}

void waveform_view::clear() {
    for (auto& ch : m_channels) {
        ch.data.clear();
    }
    update();
}

// 计算Y轴显示范围
void waveform_view::calc_y_range(double& y_min, double& y_max) const {
    if (!m_auto_scale) {
        y_min = m_y_min;
        y_max = m_y_max;
        return;
    }
    y_min = std::numeric_limits<double>::max();
    y_max = std::numeric_limits<double>::lowest();
    for (const auto& ch : m_channels) {
        for (double v : ch.data) {
            y_min = std::min(y_min, v);
            y_max = std::max(y_max, v);
        }
    }
    if (y_min == y_max) { y_min -= 1.0; y_max += 1.0; }
    double margin = (y_max - y_min) * 0.1;
    y_min -= margin;
    y_max += margin;
}

// 绘制网格和边框
void waveform_view::draw_grid(QPainter& p, const QRect& plot) {
    if (m_show_grid) {
        p.setPen(QPen(QColor(230, 230, 230), 1, Qt::DotLine));
        for (int i = 1; i < 5; ++i) {
            int y = plot.top() + plot.height() * i / 5;
            p.drawLine(plot.left(), y, plot.right(), y);
        }
        for (int i = 1; i < 10; ++i) {
            int x = plot.left() + plot.width() * i / 10;
            p.drawLine(x, plot.top(), x, plot.bottom());
        }
    }
    p.setPen(QPen(QColor(180, 180, 180), 1));
    p.drawRect(plot);
}

// 绘制Y轴刻度
void waveform_view::draw_y_axis(QPainter& p, const QRect& plot, double y_min, double y_max) {
    p.setPen(Qt::black);
    QFont font = p.font();
    font.setPointSize(8);
    p.setFont(font);
    for (int i = 0; i <= 5; ++i) {
        double val = y_max - (y_max - y_min) * i / 5;
        int y = plot.top() + plot.height() * i / 5;
        p.drawText(0, y - 6, plot.left() - 5, 12, Qt::AlignRight | Qt::AlignVCenter,
                   QString::number(val, 'f', 2));
    }
}

// 绘制波形曲线
void waveform_view::draw_waveforms(QPainter& p, const QRect& plot, double y_min, double y_max) {
    auto to_screen = [&](size_t idx, double val, size_t total) -> QPointF {
        double x = plot.left() + (double(idx) / std::max(size_t(1), total - 1)) * plot.width();
        double y = plot.top() + (1.0 - (val - y_min) / (y_max - y_min)) * plot.height();
        return QPointF(x, y);
    };
    for (const auto& ch : m_channels) {
        if (ch.data.size() < 2) continue;
        p.setPen(QPen(ch.color, 1.5));
        QPainterPath path;
        path.moveTo(to_screen(0, ch.data[0], ch.data.size()));
        for (size_t i = 1; i < ch.data.size(); ++i)
            path.lineTo(to_screen(i, ch.data[i], ch.data.size()));
        p.drawPath(path);
    }
}

// 绘制图例
void waveform_view::draw_legend(QPainter& p, const QRect& plot) {
    int x = plot.left() + 5, y = plot.top() + 5;
    for (size_t i = 0; i < m_channels.size(); ++i) {
        p.setPen(m_channels[i].color);
        p.setBrush(m_channels[i].color);
        p.drawRect(x, y + static_cast<int>(i) * 15, 10, 10);
        p.setPen(Qt::black);
        p.drawText(x + 15, y + static_cast<int>(i) * 15 + 10, m_channels[i].name);
    }
}

void waveform_view::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    
    // 计算绘图区域
    QRect plot(50, 10, width() - 60, height() - 30);
    
    // 绘制背景
    p.fillRect(rect(), QColor(250, 250, 250));
    p.fillRect(plot, Qt::white);
    
    // 计算Y轴范围
    double y_min, y_max;
    calc_y_range(y_min, y_max);
    
    // 分步绘制
    draw_grid(p, plot);
    draw_y_axis(p, plot, y_min, y_max);
    draw_waveforms(p, plot, y_min, y_max);
    draw_legend(p, plot);
}
