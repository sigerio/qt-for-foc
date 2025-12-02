#ifndef UI_COORD_TRANSFORM_PANEL_H
#define UI_COORD_TRANSFORM_PANEL_H

#include <QWidget>
#include <QLabel>

class vector_scope;

// 坐标变换可视化面板
class coord_transform_panel : public QWidget {
    Q_OBJECT
public:
    explicit coord_transform_panel(QWidget* parent = nullptr);
    
public slots:
    // 更新αβ坐标系数据
    void update_alpha_beta(double i_alpha, double i_beta);
    
    // 更新dq坐标系数据  
    void update_dq(double i_d, double i_q);
    
    // 更新abc三相电流
    void update_abc(double i_a, double i_b, double i_c);
    
    void update_display();

private:
    vector_scope* m_ab_scope;  // αβ坐标系矢量图
    vector_scope* m_dq_scope;  // dq坐标系矢量图
    
    QLabel* m_label_ia;
    QLabel* m_label_ib;
    QLabel* m_label_ic;
    QLabel* m_label_i_alpha;
    QLabel* m_label_i_beta;
    QLabel* m_label_id;
    QLabel* m_label_iq;
};

#endif // UI_COORD_TRANSFORM_PANEL_H
