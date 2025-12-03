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

void waveform_view::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    
    int w = width();
    int h = height();
    int margin_left = 50;
    int margin_right = 10;
    int margin_top = 10;
    int margin_bottom = 20;
    int plot_w = w - margin_left - margin_right;
    int plot_h = h - margin_top - margin_bottom;
    
    // 绘制背景
    p.fillRect(rect(), QColor(250, 250, 250));
    p.fillRect(margin_left, margin_top, plot_w, plot_h, Qt::white);
    
    // 计算Y轴范围
    double y_min = m_y_min;
    double y_max = m_y_max;
    if (m_auto_scale) {
        y_min = std::numeric_limits<double>::max();
        y_max = std::numeric_limits<double>::lowest();
        for (const auto& ch : m_channels) {
            for (double v : ch.data) {
                y_min = std::min(y_min, v);
                y_max = std::max(y_max, v);
            }
        }
        if (y_min == y_max) {
            y_min -= 1.0;
            y_max += 1.0;
        }
        double margin = (y_max - y_min) * 0.1;
        y_min -= margin;
        y_max += margin;
    }
    
    // 绘制网格
    if (m_show_grid) {
        p.setPen(QPen(QColor(230, 230, 230), 1, Qt::DotLine));
        for (int i = 1; i < 5; ++i) {
            int y = margin_top + plot_h * i / 5;
            p.drawLine(margin_left, y, margin_left + plot_w, y);
        }
        for (int i = 1; i < 10; ++i) {
            int x = margin_left + plot_w * i / 10;
            p.drawLine(x, margin_top, x, margin_top + plot_h);
        }
    }
    
    // 绘制边框
    p.setPen(QPen(QColor(180, 180, 180), 1));
    p.drawRect(margin_left, margin_top, plot_w, plot_h);
    
    // 绘制Y轴刻度
    p.setPen(Qt::black);
    QFont font = p.font();
    font.setPointSize(8);
    p.setFont(font);
    for (int i = 0; i <= 5; ++i) {
        double val = y_max - (y_max - y_min) * i / 5;
        int y = margin_top + plot_h * i / 5;
        p.drawText(0, y - 6, margin_left - 5, 12, Qt::AlignRight | Qt::AlignVCenter,
                   QString::number(val, 'f', 2));
    }
    
    // 坐标转换
    auto to_screen = [&](size_t idx, double val, size_t total) -> QPointF {
        double x = margin_left + (double(idx) / std::max(size_t(1), total - 1)) * plot_w;
        double y = margin_top + (1.0 - (val - y_min) / (y_max - y_min)) * plot_h;
        return QPointF(x, y);
    };
    
    // 绘制各通道波形
    for (const auto& ch : m_channels) {
        if (ch.data.size() < 2) continue;
        p.setPen(QPen(ch.color, 1.5));
        QPainterPath path;
        QPointF first = to_screen(0, ch.data[0], ch.data.size());
        path.moveTo(first);
        for (size_t i = 1; i < ch.data.size(); ++i) {
            path.lineTo(to_screen(i, ch.data[i], ch.data.size()));
        }
        p.drawPath(path);
    }
    
    // 绘制图例
    int legend_x = margin_left + 5;
    int legend_y = margin_top + 5;
    for (size_t i = 0; i < m_channels.size(); ++i) {
        p.setPen(m_channels[i].color);
        p.setBrush(m_channels[i].color);
        p.drawRect(legend_x, legend_y + static_cast<int>(i) * 15, 10, 10);
        p.setPen(Qt::black);
        p.drawText(legend_x + 15, legend_y + static_cast<int>(i) * 15 + 10, m_channels[i].name);
    }
}
