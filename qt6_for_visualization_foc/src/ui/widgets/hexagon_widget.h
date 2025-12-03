#ifndef UI_WIDGETS_HEXAGON_WIDGET_H
#define UI_WIDGETS_HEXAGON_WIDGET_H

#include <QWidget>

// SVPWM六边形矢量图绘制控件
class hexagon_widget : public QWidget {
    Q_OBJECT
public:
    explicit hexagon_widget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // UI_WIDGETS_HEXAGON_WIDGET_H
