#include "waveform_view.h"
#include <QPainter>

waveform_view::waveform_view(QWidget* parent) : QWidget(parent) {
    setMinimumSize(300, 100);
}

void waveform_view::add_point(double value) {
    m_data.push_back(value);
    if (m_data.size() > m_max_points) {
        m_data.erase(m_data.begin());
    }
    update();
}

void waveform_view::clear() {
    m_data.clear();
    update();
}

void waveform_view::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    // 后续实现波形绘制
}
