#ifndef UI_WIDGETS_COORDINATE_WIDGET_H
#define UI_WIDGETS_COORDINATE_WIDGET_H

#include <QWidget>

// 坐标系示意图绘制控件
class coordinate_widget : public QWidget {
    Q_OBJECT
public:
    explicit coordinate_widget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // UI_WIDGETS_COORDINATE_WIDGET_H
