#include "arrow_item.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QJsonObject>
#include <QCursor>
#include <QInputDialog>
#include <QLineEdit>
#include <QFontMetrics>

arrow_item::arrow_item(Direction dir, LineStyle style, QGraphicsItem* parent)
    : QGraphicsItem(parent), m_direction(dir), m_line_style(style), m_length(60.0)
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

void arrow_item::set_line_style(LineStyle style)
{
    m_line_style = style;
    update();
}

void arrow_item::set_label(const QString& label)
{
    prepareGeometryChange();
    m_label = label;
    update();
}

QRectF arrow_item::boundingRect() const
{
    qreal margin = ARROW_SIZE + 2;
    qreal label_size = m_label.isEmpty() ? 0 : 16;
    qreal label_width = m_label.isEmpty() ? 0 : 60;
    
    switch (m_direction) {
    case Right:  // →
        return QRectF(-margin, -margin - label_size, m_length + 2 * margin, 2 * margin + label_size);
    case Left:   // ←
        return QRectF(-m_length - margin, -margin - label_size, m_length + 2 * margin, 2 * margin + label_size);
    case Down:   // ↓
        return QRectF(-margin, -margin, 2 * margin + label_width, m_length + 2 * margin);
    case Up:     // ↑
        return QRectF(-margin, -m_length - margin, 2 * margin + label_width, m_length + 2 * margin);
    }
    return QRectF();
}

void arrow_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    painter->setRenderHint(QPainter::Antialiasing);
    
    // 使用半透明颜色避免交叉重叠时的视觉问题
    QColor color = isSelected() ? QColor(0, 0, 255, 200) : QColor(0, 0, 0, 180);
    QPen pen(color, 2);
    if (m_line_style == Dashed) {
        pen.setStyle(Qt::DashLine);
    }
    painter->setPen(pen);
    
    QPointF start(0, 0);
    QPointF end;
    
    // 根据方向计算终点
    switch (m_direction) {
    case Right: end = QPointF(m_length, 0); break;
    case Left:  end = QPointF(-m_length, 0); break;
    case Down:  end = QPointF(0, m_length); break;
    case Up:    end = QPointF(0, -m_length); break;
    }
    
    // 绘制线段
    painter->drawLine(start, end);
    
    // 绘制箭头头部
    QPolygonF arrow_head;
    switch (m_direction) {
    case Right:  // →
        arrow_head << end
                   << QPointF(end.x() - ARROW_SIZE, end.y() - ARROW_SIZE / 2)
                   << QPointF(end.x() - ARROW_SIZE, end.y() + ARROW_SIZE / 2);
        break;
    case Left:   // ←
        arrow_head << end
                   << QPointF(end.x() + ARROW_SIZE, end.y() - ARROW_SIZE / 2)
                   << QPointF(end.x() + ARROW_SIZE, end.y() + ARROW_SIZE / 2);
        break;
    case Down:   // ↓
        arrow_head << end
                   << QPointF(end.x() - ARROW_SIZE / 2, end.y() - ARROW_SIZE)
                   << QPointF(end.x() + ARROW_SIZE / 2, end.y() - ARROW_SIZE);
        break;
    case Up:     // ↑
        arrow_head << end
                   << QPointF(end.x() - ARROW_SIZE / 2, end.y() + ARROW_SIZE)
                   << QPointF(end.x() + ARROW_SIZE / 2, end.y() + ARROW_SIZE);
        break;
    }
    
    painter->setBrush(color);
    painter->setPen(Qt::NoPen);
    painter->drawPolygon(arrow_head);
    
    // 绘制标签文字（紧靠箭头）
    if (!m_label.isEmpty()) {
        QFont font;
        font.setPointSize(9);
        painter->setFont(font);
        painter->setPen(Qt::black);
        
        QFontMetrics fm(font);
        int text_width = fm.horizontalAdvance(m_label);
        
        QPointF text_pos;
        switch (m_direction) {
        case Right:  // 标签在箭头上方，居中
            text_pos = QPointF((m_length - text_width) / 2, -2);
            break;
        case Left:   // 标签在箭头上方，居中
            text_pos = QPointF((-m_length - text_width) / 2, -2);
            break;
        case Down:   // 标签在箭头右侧
            text_pos = QPointF(6, m_length / 2 + 4);
            break;
        case Up:     // 标签在箭头右侧
            text_pos = QPointF(6, -m_length / 2 + 4);
            break;
        }
        painter->drawText(text_pos, m_label);
    }
    
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
        QPointF end;
        switch (m_direction) {
        case Right: end = QPointF(m_length, 0); break;
        case Left:  end = QPointF(-m_length, 0); break;
        case Down:  end = QPointF(0, m_length); break;
        case Up:    end = QPointF(0, -m_length); break;
        }
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
        qreal d = 0;
        switch (m_direction) {
        case Right: d = delta.x(); break;
        case Left:  d = -delta.x(); break;
        case Down:  d = delta.y(); break;
        case Up:    d = -delta.y(); break;
        }
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
    // 双击编辑标签名称
    bool ok;
    QString new_label = QInputDialog::getText(nullptr, "编辑标签", "标签名称:", 
                                               QLineEdit::Normal, m_label, &ok);
    if (ok) {
        set_label(new_label);
    }
}

QJsonObject arrow_item::to_json() const
{
    QJsonObject obj;
    obj["type"] = "arrow";
    obj["x"] = pos().x();
    obj["y"] = pos().y();
    obj["length"] = m_length;
    QString dir_str;
    switch (m_direction) {
    case Right: dir_str = "right"; break;
    case Left:  dir_str = "left"; break;
    case Down:  dir_str = "down"; break;
    case Up:    dir_str = "up"; break;
    }
    obj["direction"] = dir_str;
    obj["style"] = (m_line_style == Dashed) ? "dashed" : "solid";
    if (!m_label.isEmpty()) {
        obj["label"] = m_label;
    }
    return obj;
}

arrow_item* arrow_item::from_json(const QJsonObject& obj)
{
    QString dir_str = obj["direction"].toString();
    Direction dir = Right;
    if (dir_str == "right" || dir_str == "h") dir = Right;
    else if (dir_str == "left") dir = Left;
    else if (dir_str == "down" || dir_str == "v") dir = Down;
    else if (dir_str == "up") dir = Up;
    LineStyle style = (obj["style"].toString() == "dashed") ? Dashed : Solid;
    auto* item = new arrow_item(dir, style);
    item->setPos(obj["x"].toDouble(), obj["y"].toDouble());
    item->set_length(obj["length"].toDouble(60));
    if (obj.contains("label")) {
        item->set_label(obj["label"].toString());
    }
    return item;
}
