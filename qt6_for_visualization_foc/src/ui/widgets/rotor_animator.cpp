#include "rotor_animator.h"
#include <QPainter>

rotor_animator::rotor_animator(QWidget* parent) : QWidget(parent) {
    setMinimumSize(200, 200);
}

void rotor_animator::set_angle(double angle_rad) {
    m_angle = angle_rad;
    update();
}

void rotor_animator::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    // 后续实现转子动画
}
