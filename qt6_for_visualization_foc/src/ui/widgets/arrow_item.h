#ifndef UI_WIDGETS_ARROW_ITEM_H
#define UI_WIDGETS_ARROW_ITEM_H

#include <QGraphicsItem>

// 可拖拽、可拉伸的箭头组件
class arrow_item : public QGraphicsItem {
public:
    enum Direction { Horizontal, Vertical };
    
    explicit arrow_item(Direction dir = Horizontal, QGraphicsItem* parent = nullptr);
    
    Direction direction() const { return m_direction; }
    void set_direction(Direction dir);
    
    qreal length() const { return m_length; }
    void set_length(qreal len);
    
    // 序列化
    QJsonObject to_json() const;
    static arrow_item* from_json(const QJsonObject& obj);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    Direction m_direction = Horizontal;
    qreal m_length = 60.0;
    
    bool m_resizing = false;
    QPointF m_drag_start;
    qreal m_length_start = 0;
    
    static constexpr qreal ARROW_SIZE = 8.0;
};

#endif // UI_WIDGETS_ARROW_ITEM_H
