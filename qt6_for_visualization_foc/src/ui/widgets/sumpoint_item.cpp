#include "sumpoint_item.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QJsonObject>
#include <QCursor>

sumpoint_item::sumpoint_item(QGraphicsItem* parent)
    : QGraphicsEllipseItem(0, 0, 20, 20, parent)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setBrush(Qt::white);
    setPen(QPen(Qt::black, 1));
}

bool sumpoint_item::is_in_resize_area(const QPointF& pos) const
{
    QRectF r = rect();
    QRectF handle(r.right() - HANDLE_SIZE, r.bottom() - HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);
    return handle.contains(pos);
}

void sumpoint_item::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    if (is_in_resize_area(event->pos())) {
        setCursor(Qt::SizeFDiagCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsEllipseItem::hoverMoveEvent(event);
}

void sumpoint_item::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && is_in_resize_area(event->pos())) {
        m_resizing = true;
        m_resize_start = event->pos();
        m_resize_rect_start = rect();
        event->accept();
    } else {
        QGraphicsEllipseItem::mousePressEvent(event);
    }
}

void sumpoint_item::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_resizing) {
        QPointF delta = event->pos() - m_resize_start;
        qreal d = qMax(delta.x(), delta.y());
        qreal new_size = qMax(14.0, m_resize_rect_start.width() + d);
        setRect(0, 0, new_size, new_size);
        event->accept();
    } else {
        QGraphicsEllipseItem::mouseMoveEvent(event);
    }
}

void sumpoint_item::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_resizing) {
        m_resizing = false;
        event->accept();
    } else {
        QGraphicsEllipseItem::mouseReleaseEvent(event);
    }
}

void sumpoint_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::white);
    painter->setPen(QPen(isSelected() ? Qt::blue : Qt::black, isSelected() ? 2 : 1));
    painter->drawEllipse(rect());
    
    // 绘制十字
    qreal cx = rect().center().x();
    qreal cy = rect().center().y();
    qreal r = rect().width() / 2 - 3;
    painter->drawLine(QPointF(cx - r, cy), QPointF(cx + r, cy));
    painter->drawLine(QPointF(cx, cy - r), QPointF(cx, cy + r));
    
    // 选中时绘制调整手柄
    if (isSelected()) {
        QRectF re = rect();
        QRectF handle(re.right() - HANDLE_SIZE, re.bottom() - HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);
        painter->setBrush(Qt::darkGray);
        painter->setPen(Qt::NoPen);
        painter->drawRect(handle);
    }
}

QJsonObject sumpoint_item::to_json() const
{
    QJsonObject obj;
    obj["type"] = "sumpoint";
    obj["x"] = pos().x();
    obj["y"] = pos().y();
    obj["size"] = rect().width();
    return obj;
}

sumpoint_item* sumpoint_item::from_json(const QJsonObject& obj)
{
    auto* item = new sumpoint_item();
    item->setPos(obj["x"].toDouble(), obj["y"].toDouble());
    qreal size = obj["size"].toDouble(20);
    item->setRect(0, 0, size, size);
    return item;
}
