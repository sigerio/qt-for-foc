#include "arrow_item.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QJsonObject>
#include <QCursor>

arrow_item::arrow_item(Direction dir, QGraphicsItem* parent)
    : QGraphicsItem(parent), m_direction(dir), m_length(60.0)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
}

void arrow_item::set_direction(Direction dir)
{
    prepareGeometryChange();
    m_direction = dir;
    update();
}

void arrow_item::set_length(qreal len)
{
    prepareGeometryChange();
    m_length = qMax(20.0, len);
    update();
}

QRectF arrow_item::boundingRect() const
{
    qreal margin = ARROW_SIZE + 2;
    if (m_direction == Horizontal) {
        return QRectF(-margin, -margin, m_length + 2 * margin, 2 * margin);
    } else {
        return QRectF(-margin, -margin, 2 * margin, m_length + 2 * margin);
    }
}

void arrow_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    painter->setRenderHint(QPainter::Antialiasing);
    
    QColor color = isSelected() ? Qt::blue : Qt::black;
    painter->setPen(QPen(color, 2));
    
    QPointF start(0, 0);
    QPointF end;
    
    if (m_direction == Horizontal) {
        end = QPointF(m_length, 0);
    } else {
        end = QPointF(0, m_length);
    }
    
    // 绘制线段
    painter->drawLine(start, end);
    
    // 绘制箭头头部
    QPolygonF arrow_head;
    if (m_direction == Horizontal) {
        arrow_head << end
                   << QPointF(end.x() - ARROW_SIZE, end.y() - ARROW_SIZE / 2)
                   << QPointF(end.x() - ARROW_SIZE, end.y() + ARROW_SIZE / 2);
    } else {
        arrow_head << end
                   << QPointF(end.x() - ARROW_SIZE / 2, end.y() - ARROW_SIZE)
                   << QPointF(end.x() + ARROW_SIZE / 2, end.y() - ARROW_SIZE);
    }
    
    painter->setBrush(color);
    painter->setPen(Qt::NoPen);
    painter->drawPolygon(arrow_head);
    
    // 选中时绘制端点标记
    if (isSelected()) {
        painter->setBrush(Qt::white);
        painter->setPen(QPen(Qt::blue, 1));
        painter->drawEllipse(start, 4, 4);
        painter->drawEllipse(end, 4, 4);
    }
}

void arrow_item::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPointF end = (m_direction == Horizontal) ? QPointF(m_length, 0) : QPointF(0, m_length);
        QPointF localPos = event->pos();
        
        // 检查是否点击终点附近（用于拉伸）
        qreal dist = QLineF(localPos, end).length();
        if (dist < 10) {
            m_resizing = true;
            m_drag_start = event->pos();
            m_length_start = m_length;
            event->accept();
            return;
        }
    }
    QGraphicsItem::mousePressEvent(event);
}

void arrow_item::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_resizing) {
        QPointF delta = event->pos() - m_drag_start;
        qreal d = (m_direction == Horizontal) ? delta.x() : delta.y();
        set_length(m_length_start + d);
        event->accept();
    } else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void arrow_item::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_resizing) {
        m_resizing = false;
        event->accept();
    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

void arrow_item::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    set_direction(m_direction == Horizontal ? Vertical : Horizontal);
}

QJsonObject arrow_item::to_json() const
{
    QJsonObject obj;
    obj["type"] = "arrow";
    obj["x"] = pos().x();
    obj["y"] = pos().y();
    obj["length"] = m_length;
    obj["direction"] = (m_direction == Horizontal) ? "h" : "v";
    return obj;
}

arrow_item* arrow_item::from_json(const QJsonObject& obj)
{
    Direction dir = (obj["direction"].toString() == "h") ? Horizontal : Vertical;
    auto* item = new arrow_item(dir);
    item->setPos(obj["x"].toDouble(), obj["y"].toDouble());
    item->set_length(obj["length"].toDouble(60));
    return item;
}
