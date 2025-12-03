#ifndef UI_WIDGETS_LABEL_ITEM_H
#define UI_WIDGETS_LABEL_ITEM_H

#include <QGraphicsSimpleTextItem>

// 可拖拽、可调整字体大小的文本标签
class label_item : public QGraphicsSimpleTextItem {
public:
    explicit label_item(const QString& text, QGraphicsItem* parent = nullptr);
    
    int font_size() const { return m_font_size; }
    void set_font_size(int size);
    
    // 序列化
    QJsonObject to_json() const;
    static label_item* from_json(const QJsonObject& obj);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    int m_font_size = 10;
};

#endif // UI_WIDGETS_LABEL_ITEM_H
