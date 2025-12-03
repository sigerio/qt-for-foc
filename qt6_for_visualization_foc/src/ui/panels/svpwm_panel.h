#ifndef UI_SVPWM_PANEL_H
#define UI_SVPWM_PANEL_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>

// SVPWM六边形控件（内嵌类）
class svpwm_hexagon : public QWidget {
    Q_OBJECT
public:
    explicit svpwm_hexagon(QWidget* parent = nullptr);
    void set_vector(double u_alpha, double u_beta);
    void set_sector(int sector);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    double m_u_alpha = 0.0;
    double m_u_beta = 0.0;
    int m_sector = 1;
};

// SVPWM可视化面板
class svpwm_panel : public QWidget {
    Q_OBJECT
public:
    explicit svpwm_panel(QWidget* parent = nullptr);
    
public slots:
    // 更新电压矢量
    void update_voltage(double u_alpha, double u_beta);
    
    // 更新扇区
    void update_sector(int sector);
    
    // 更新占空比
    void update_duty(double ta, double tb, double tc);
    
    void update_display();

private:
    svpwm_hexagon* m_hexagon;
    QLabel* m_label_sector;
    QLabel* m_label_u_alpha;
    QLabel* m_label_u_beta;
    QProgressBar* m_bar_ta;
    QProgressBar* m_bar_tb;
    QProgressBar* m_bar_tc;
};

#endif // UI_SVPWM_PANEL_H
