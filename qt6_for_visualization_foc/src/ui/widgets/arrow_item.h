#ifndef UI_WIDGETS_ARROW_ITEM_H
#define UI_WIDGETS_ARROW_ITEM_H

#include <QGraphicsItem>

// 可拖拽、可拉伸的箭头组件，支持中文标签和虚线样式
class arrow_item : public QGraphicsItem {
public:
    enum Direction { Right, Left, Down, Up };  // 四个方向：右、左、下、上
    enum LineStyle { Solid, Dashed };  // 线条样式：实线/虚线
    
    explicit arrow_item(Direction dir = Right, LineStyle style = Solid, QGraphicsItem* parent = nullptr);
    
    Direction direction() const { return m_direction; }
    void set_direction(Direction dir);
    
    qreal length() const { return m_length; }
    void set_length(qreal len);
    
    // 线条样式
    LineStyle line_style() const { return m_line_style; }
    void set_line_style(LineStyle style);
    
    // 标签功能
    QString label() const { return m_label; }
    void set_label(const QString& label);
    
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
    Direction m_direction = Right;
    LineStyle m_line_style = Solid;  // 线条样式
    qreal m_length = 60.0;
    QString m_label;  // 箭头标签（支持中文）
    
    bool m_resizing = false;
    QPointF m_drag_start;
    qreal m_length_start = 0;
    
    static constexpr qreal ARROW_SIZE = 8.0;
};

#endif // UI_WIDGETS_ARROW_ITEM_H
