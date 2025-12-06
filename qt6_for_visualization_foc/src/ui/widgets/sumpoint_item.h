#ifndef UI_WIDGETS_SUMPOINT_ITEM_H
#define UI_WIDGETS_SUMPOINT_ITEM_H

#include <QGraphicsEllipseItem>

// 可拖拽、可调整大小的圆形求和点，支持中文标签
class sumpoint_item : public QGraphicsEllipseItem {
public:
    explicit sumpoint_item(QGraphicsItem* parent = nullptr);
    
    // 标签功能
    QString label() const { return m_label; }
    void set_label(const QString& label);
    
    // 序列化
    QJsonObject to_json() const;
    static sumpoint_item* from_json(const QJsonObject& obj);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    bool is_in_resize_area(const QPointF& pos) const;
    
    QString m_label;  // 求和点标签（支持中文）
    bool m_resizing = false;
    QPointF m_resize_start;
    QRectF m_resize_rect_start;
    static constexpr qreal HANDLE_SIZE = 6.0;
};

#endif // UI_WIDGETS_SUMPOINT_ITEM_H
