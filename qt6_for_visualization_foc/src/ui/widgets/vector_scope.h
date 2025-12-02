#ifndef UI_WIDGETS_VECTOR_SCOPE_H
#define UI_WIDGETS_VECTOR_SCOPE_H

#include <QWidget>

// 矢量示波器控件（占位）
class vector_scope : public QWidget {
    Q_OBJECT
public:
    explicit vector_scope(QWidget* parent = nullptr);

    void set_vector(double alpha, double beta);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    double m_alpha = 0.0;
    double m_beta = 0.0;
};

#endif // UI_WIDGETS_VECTOR_SCOPE_H
