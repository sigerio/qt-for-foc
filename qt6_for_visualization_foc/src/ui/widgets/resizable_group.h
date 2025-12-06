#ifndef UI_RESIZABLE_GROUP_H
#define UI_RESIZABLE_GROUP_H

#include <QGroupBox>
#include <QMouseEvent>
#include <QPaintEvent>

// 可调整高度的分组框
// 底部边缘可拖动调整高度，带有可视化手柄
class resizable_group : public QGroupBox {
    Q_OBJECT
public:
    explicit resizable_group(const QString& title, QWidget* parent = nullptr);
    
    void set_min_height(int h) { m_min_height = h; }
    int get_min_height() const { return m_min_height; }

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    bool is_in_resize_zone(const QPoint& pos) const;

private:
    bool m_resizing = false;
    bool m_hover_resize = false;  // 鼠标是否悬停在调整区域
    int m_resize_start_y = 0;
    int m_resize_start_height = 0;
    int m_min_height = 150;
    static constexpr int RESIZE_ZONE_HEIGHT = 8;  // 底部可拖动区域高度
};

#endif // UI_RESIZABLE_GROUP_H
