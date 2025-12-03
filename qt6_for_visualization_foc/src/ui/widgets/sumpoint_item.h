#ifndef UI_WIDGETS_SUMPOINT_ITEM_H
#define UI_WIDGETS_SUMPOINT_ITEM_H

#include <QGraphicsEllipseItem>

// 可拖拽、可调整大小的圆形求和点
class sumpoint_item : public QGraphicsEllipseItem {
public:
    explicit sumpoint_item(QGraphicsItem* parent = nullptr);
    
    // 序列化
    QJsonObject to_json() const;
    static sumpoint_item* from_json(const QJsonObject& obj);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    bool is_in_resize_area(const QPointF& pos) const;
    
    bool m_resizing = false;
    QPointF m_resize_start;
    QRectF m_resize_rect_start;
    static constexpr qreal HANDLE_SIZE = 6.0;
};

#endif // UI_WIDGETS_SUMPOINT_ITEM_H
