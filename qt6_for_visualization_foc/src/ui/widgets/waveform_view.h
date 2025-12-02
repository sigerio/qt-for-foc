#ifndef UI_WIDGETS_WAVEFORM_VIEW_H
#define UI_WIDGETS_WAVEFORM_VIEW_H

#include <QWidget>
#include <vector>

// 波形显示控件（占位）
class waveform_view : public QWidget {
    Q_OBJECT
public:
    explicit waveform_view(QWidget* parent = nullptr);

    void add_point(double value);
    void clear();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<double> m_data;
    size_t m_max_points = 500;
};

#endif // UI_WIDGETS_WAVEFORM_VIEW_H
