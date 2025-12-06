#include "resizable_group.h"
#include <QCursor>
#include <QPainter>

resizable_group::resizable_group(const QString& title, QWidget* parent)
    : QGroupBox(title, parent)
    , m_min_height(150)  // 增加默认最小高度以适应波形+手柄
{
    setMouseTracking(true);
    setFixedHeight(m_min_height);
    
    // 设置内容边距，底部留出空间显示手柄
    setContentsMargins(5, 20, 5, RESIZE_ZONE_HEIGHT + 2);
}

bool resizable_group::is_in_resize_zone(const QPoint& pos) const {
    return pos.y() >= height() - RESIZE_ZONE_HEIGHT;
}

void resizable_group::paintEvent(QPaintEvent* event) {
    // 先调用父类绘制
    QGroupBox::paintEvent(event);
    
    // 绘制底部调整手柄
    QPainter painter(this);
    
    // 选择颜色：悬停或拖动时高亮
    QColor handleColor;
    if (m_resizing) {
        handleColor = QColor(0, 120, 215);  // 蓝色（拖动中）
    } else if (m_hover_resize) {
        handleColor = QColor(100, 100, 100);  // 深灰（悬停）
    } else {
        handleColor = QColor(180, 180, 180);  // 浅灰（正常）
    }
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(handleColor);
    
    // 绘制手柄条（底部居中的短条）
    int handleWidth = 60;
    int handleHeight = 4;
    int x = (width() - handleWidth) / 2;
    int y = height() - RESIZE_ZONE_HEIGHT + (RESIZE_ZONE_HEIGHT - handleHeight) / 2;
    painter.drawRoundedRect(x, y, handleWidth, handleHeight, 2, 2);
}

void resizable_group::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && is_in_resize_zone(event->pos())) {
        m_resizing = true;
        m_resize_start_y = event->globalPosition().toPoint().y();
        m_resize_start_height = height();
        setCursor(Qt::SizeVerCursor);
    }
    QGroupBox::mousePressEvent(event);
}

void resizable_group::mouseMoveEvent(QMouseEvent* event) {
    if (m_resizing) {
        int delta = event->globalPosition().toPoint().y() - m_resize_start_y;
        int new_height = qMax(m_min_height, m_resize_start_height + delta);
        setFixedHeight(new_height);
        
        // 通知父窗口更新布局
        if (parentWidget()) {
            parentWidget()->adjustSize();
        }
    } else {
        // 更新悬停状态和鼠标指针
        bool in_zone = is_in_resize_zone(event->pos());
        if (in_zone != m_hover_resize) {
            m_hover_resize = in_zone;
            update();  // 触发重绘以更新手柄颜色
        }
        setCursor(in_zone ? Qt::SizeVerCursor : Qt::ArrowCursor);
    }
    QGroupBox::mouseMoveEvent(event);
}

void resizable_group::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_resizing) {
        m_resizing = false;
        if (!is_in_resize_zone(event->pos())) {
            setCursor(Qt::ArrowCursor);
        }
    }
    QGroupBox::mouseReleaseEvent(event);
}

void resizable_group::enterEvent(QEnterEvent* event) {
    QGroupBox::enterEvent(event);
}

void resizable_group::leaveEvent(QEvent* event) {
    if (!m_resizing) {
        setCursor(Qt::ArrowCursor);
        if (m_hover_resize) {
            m_hover_resize = false;
            update();  // 触发重绘以更新手柄颜色
        }
    }
    QGroupBox::leaveEvent(event);
}
