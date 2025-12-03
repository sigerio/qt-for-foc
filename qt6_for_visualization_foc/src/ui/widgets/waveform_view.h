#ifndef UI_WIDGETS_WAVEFORM_VIEW_H
#define UI_WIDGETS_WAVEFORM_VIEW_H

#include <QWidget>
#include <QColor>
#include <QRect>
#include <vector>
#include <deque>

class QPainter;

// 多通道波形显示控件
class waveform_view : public QWidget {
    Q_OBJECT
public:
    explicit waveform_view(QWidget* parent = nullptr);

    // 添加单通道数据点
    void add_point(double value);
    
    // 添加多通道数据点
    void add_points(const std::vector<double>& values);
    
    // 设置通道数和颜色
    void set_channels(int count, const std::vector<QColor>& colors);
    
    // 设置通道名称
    void set_channel_names(const std::vector<QString>& names);
    
    // 设置Y轴范围（自动缩放时忽略）
    void set_y_range(double min_val, double max_val);
    
    // 设置自动缩放
    void set_auto_scale(bool enabled);
    
    // 设置最大显示点数
    void set_max_points(size_t count);
    
    // 清除所有数据
    void clear();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    // 绘制辅助方法
    void calc_y_range(double& y_min, double& y_max) const;
    void draw_grid(QPainter& p, const QRect& plot);
    void draw_y_axis(QPainter& p, const QRect& plot, double y_min, double y_max);
    void draw_waveforms(QPainter& p, const QRect& plot, double y_min, double y_max);
    void draw_legend(QPainter& p, const QRect& plot);

    struct channel_data {
        std::deque<double> data;
        QColor color;
        QString name;
    };
    
    std::vector<channel_data> m_channels;
    size_t m_max_points = 500;
    double m_y_min = -1.0;
    double m_y_max = 1.0;
    bool m_auto_scale = true;
    bool m_show_grid = true;
};

#endif // UI_WIDGETS_WAVEFORM_VIEW_H
