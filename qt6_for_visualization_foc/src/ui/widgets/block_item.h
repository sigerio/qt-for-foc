#ifndef UI_WIDGETS_BLOCK_ITEM_H
#define UI_WIDGETS_BLOCK_ITEM_H

#include <QGraphicsRectItem>

// 可拖拽、可调整大小的矩形块（算法模块）
class block_item : public QGraphicsRectItem {
public:
    block_item(const QString& name, const QColor& color, QGraphicsItem* parent = nullptr);
    
    QString name() const { return m_name; }
    void set_name(const QString& name);
    
    QColor color() const { return m_color; }
    void set_color(const QColor& color);
    
    // 序列化
    QJsonObject to_json() const;
    static block_item* from_json(const QJsonObject& obj);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    bool is_in_resize_area(const QPointF& pos) const;
    
    QString m_name;
    QColor m_color;
    bool m_resizing = false;
    QPointF m_resize_start;
    QRectF m_resize_rect_start;
    static constexpr qreal HANDLE_SIZE = 8.0;
};

#endif // UI_WIDGETS_BLOCK_ITEM_H
