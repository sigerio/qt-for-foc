#ifndef UI_WIDGETS_VECTOR_SCOPE_H
#define UI_WIDGETS_VECTOR_SCOPE_H

#include <QWidget>
#include <QColor>
#include <deque>

// αβ坐标系矢量示波器
class vector_scope : public QWidget {
    Q_OBJECT
public:
    explicit vector_scope(QWidget* parent = nullptr);

    // 设置当前矢量值
    void set_vector(double alpha, double beta);
    
    // 设置显示范围
    void set_range(double range);
    
    // 设置矢量颜色
    void set_color(const QColor& color);
    
    // 设置是否显示轨迹
    void set_show_trail(bool show);
    
    // 清除轨迹
    void clear_trail();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    double m_alpha = 0.0;
    double m_beta = 0.0;
    double m_range = 1.0;  // 坐标轴范围
    QColor m_color = QColor(0, 150, 255);  // 矢量颜色
    bool m_show_trail = true;  // 是否显示轨迹
    std::deque<std::pair<double, double>> m_trail;  // 历史轨迹
    size_t m_max_trail = 200;  // 最大轨迹点数
};

#endif // UI_WIDGETS_VECTOR_SCOPE_H
