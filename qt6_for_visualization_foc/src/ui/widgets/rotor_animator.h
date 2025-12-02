#ifndef UI_WIDGETS_ROTOR_ANIMATOR_H
#define UI_WIDGETS_ROTOR_ANIMATOR_H

#include <QWidget>

// 转子动画控件（占位）
class rotor_animator : public QWidget {
    Q_OBJECT
public:
    explicit rotor_animator(QWidget* parent = nullptr);

    void set_angle(double angle_rad);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    double m_angle = 0.0;
};

#endif // UI_WIDGETS_ROTOR_ANIMATOR_H
