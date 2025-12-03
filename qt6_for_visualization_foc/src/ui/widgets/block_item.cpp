#include "block_item.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QInputDialog>
#include <QJsonObject>
#include <QCursor>

block_item::block_item(const QString& name, const QColor& color, QGraphicsItem* parent)
    : QGraphicsRectItem(0, 0, 70, 30, parent), m_name(name), m_color(color)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setBrush(color);
    setPen(QPen(Qt::black, 1));
}

void block_item::set_name(const QString& name)
{
    m_name = name;
    update();
}

void block_item::set_color(const QColor& color)
{
    m_color = color;
    setBrush(color);
    update();
}

bool block_item::is_in_resize_area(const QPointF& pos) const
{
    QRectF r = rect();
    QRectF handle(r.right() - HANDLE_SIZE, r.bottom() - HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);
    return handle.contains(pos);
}

void block_item::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    if (is_in_resize_area(event->pos())) {
        setCursor(Qt::SizeFDiagCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsRectItem::hoverMoveEvent(event);
}

void block_item::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && is_in_resize_area(event->pos())) {
        m_resizing = true;
        m_resize_start = event->pos();
        m_resize_rect_start = rect();
        event->accept();
    } else {
        QGraphicsRectItem::mousePressEvent(event);
    }
}

void block_item::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_resizing) {
        QPointF delta = event->pos() - m_resize_start;
        qreal new_w = qMax(40.0, m_resize_rect_start.width() + delta.x());
        qreal new_h = qMax(20.0, m_resize_rect_start.height() + delta.y());
        setRect(0, 0, new_w, new_h);
        event->accept();
    } else {
        QGraphicsRectItem::mouseMoveEvent(event);
    }
}

void block_item::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_resizing) {
        m_resizing = false;
        event->accept();
    } else {
        QGraphicsRectItem::mouseReleaseEvent(event);
    }
}

void block_item::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    bool ok;
    QString new_name = QInputDialog::getText(nullptr, "编辑名称", "名称:", 
                                              QLineEdit::Normal, m_name, &ok);
    if (ok && !new_name.isEmpty()) {
        set_name(new_name);
    }
}

void block_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(m_color);
    painter->setPen(QPen(isSelected() ? Qt::blue : Qt::black, isSelected() ? 2 : 1));
    painter->drawRoundedRect(rect(), 3, 3);
    
    // 绘制名称
    painter->setPen(Qt::black);
    painter->setFont(QFont("SimHei", 9));
    painter->drawText(rect(), Qt::AlignCenter, m_name);
    
    // 选中时绘制调整手柄
    if (isSelected()) {
        QRectF r = rect();
        QRectF handle(r.right() - HANDLE_SIZE, r.bottom() - HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);
        painter->setBrush(Qt::darkGray);
        painter->setPen(Qt::NoPen);
        painter->drawRect(handle);
    }
}

QJsonObject block_item::to_json() const
{
    QJsonObject obj;
    obj["type"] = "block";
    obj["name"] = m_name;
    obj["x"] = pos().x();
    obj["y"] = pos().y();
    obj["w"] = rect().width();
    obj["h"] = rect().height();
    obj["color"] = m_color.name();
    return obj;
}

block_item* block_item::from_json(const QJsonObject& obj)
{
    QString name = obj["name"].toString();
    QColor color(obj["color"].toString());
    auto* item = new block_item(name, color);
    item->setPos(obj["x"].toDouble(), obj["y"].toDouble());
    item->setRect(0, 0, obj["w"].toDouble(70), obj["h"].toDouble(30));
    return item;
}
