#include "label_item.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QInputDialog>
#include <QJsonObject>

label_item::label_item(const QString& text, QGraphicsItem* parent)
    : QGraphicsSimpleTextItem(text, parent), m_font_size(10)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setFont(QFont("SimHei", m_font_size));
    setBrush(Qt::black);
}

void label_item::set_font_size(int size)
{
    m_font_size = qBound(6, size, 24);
    QFont f = font();
    f.setPointSize(m_font_size);
    setFont(f);
}

void label_item::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    if (isSelected()) {
        int delta = event->delta() > 0 ? 1 : -1;
        set_font_size(m_font_size + delta);
        event->accept();
    } else {
        QGraphicsSimpleTextItem::wheelEvent(event);
    }
}

void label_item::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    bool ok;
    QString new_text = QInputDialog::getText(nullptr, "编辑文本", "文本:", 
                                              QLineEdit::Normal, text(), &ok);
    if (ok && !new_text.isEmpty()) {
        setText(new_text);
    }
}

void label_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (isSelected()) {
        painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
        painter->drawRect(boundingRect());
    }
    QGraphicsSimpleTextItem::paint(painter, option, widget);
}

QJsonObject label_item::to_json() const
{
    QJsonObject obj;
    obj["type"] = "label";
    obj["text"] = text();
    obj["x"] = pos().x();
    obj["y"] = pos().y();
    obj["fontsize"] = m_font_size;
    return obj;
}

label_item* label_item::from_json(const QJsonObject& obj)
{
    QString text = obj["text"].toString();
    auto* item = new label_item(text);
    item->setPos(obj["x"].toDouble(), obj["y"].toDouble());
    item->set_font_size(obj["fontsize"].toInt(10));
    return item;
}
