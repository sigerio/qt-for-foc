#ifndef UI_WIDGETS_CIRCUIT_WIDGET_H
#define UI_WIDGETS_CIRCUIT_WIDGET_H

#include <QWidget>

// 电机等效电路图绘制控件
class circuit_widget : public QWidget {
    Q_OBJECT
public:
    explicit circuit_widget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // UI_WIDGETS_CIRCUIT_WIDGET_H
