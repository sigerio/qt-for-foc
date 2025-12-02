#include "vector_scope.h"
#include <QPainter>

vector_scope::vector_scope(QWidget* parent) : QWidget(parent) {
    setMinimumSize(200, 200);
}

void vector_scope::set_vector(double alpha, double beta) {
    m_alpha = alpha;
    m_beta = beta;
    update();
}

void vector_scope::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    // 后续实现矢量绘制
}
