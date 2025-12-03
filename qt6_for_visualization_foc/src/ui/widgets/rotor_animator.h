#ifndef UI_WIDGETS_ROTOR_ANIMATOR_H
#define UI_WIDGETS_ROTOR_ANIMATOR_H

#include <QWidget>

class QPainter;

// PMSM转子动画控件
class rotor_animator : public QWidget {
    Q_OBJECT
public:
    explicit rotor_animator(QWidget* parent = nullptr);

    // 设置转子角度(rad)
    void set_angle(double angle_rad);
    
    // 设置极对数
    void set_pole_pairs(int pairs);
    
    // 设置是否显示三相绕组
    void set_show_stator(bool show);
    
    // 设置是否显示角度标注
    void set_show_angle_label(bool show);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void draw_stator(QPainter& p, int cx, int cy, int r_stator, int r_rotor);
    void draw_rotor(QPainter& p, int cx, int cy, int r_rotor, int r_shaft);
    void draw_arrow_and_labels(QPainter& p, int cx, int cy, int r_rotor, int h);

    double m_angle = 0.0;
    int m_pole_pairs = 4;
    bool m_show_stator = true;
    bool m_show_angle_label = true;
};

#endif // UI_WIDGETS_ROTOR_ANIMATOR_H
