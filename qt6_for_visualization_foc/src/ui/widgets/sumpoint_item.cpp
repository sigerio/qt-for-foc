#include "sumpoint_item.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QJsonObject>
#include <QCursor>
#include <QInputDialog>
#include <QLineEdit>

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

void sumpoint_item::set_label(const QString& label)
{
    prepareGeometryChange();
    m_label = label;
    update();
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

void sumpoint_item::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    // 双击编辑标签名称
    bool ok;
    QString new_label = QInputDialog::getText(nullptr, "编辑标签", "标签名称:", 
                                               QLineEdit::Normal, m_label, &ok);
    if (ok) {
        set_label(new_label);
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
    
    // 绘制标签文字（求和点下方）
    if (!m_label.isEmpty()) {
        QFont font;
        font.setPointSize(9);
        painter->setFont(font);
        painter->setPen(Qt::black);
        QPointF text_pos(cx - 20, rect().bottom() + 14);
        painter->drawText(text_pos, m_label);
    }
    
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
    if (!m_label.isEmpty()) {
        obj["label"] = m_label;
    }
    return obj;
}

sumpoint_item* sumpoint_item::from_json(const QJsonObject& obj)
{
    auto* item = new sumpoint_item();
    item->setPos(obj["x"].toDouble(), obj["y"].toDouble());
    qreal size = obj["size"].toDouble(20);
    item->setRect(0, 0, size, size);
    if (obj.contains("label")) {
        item->set_label(obj["label"].toString());
    }
    return item;
}
