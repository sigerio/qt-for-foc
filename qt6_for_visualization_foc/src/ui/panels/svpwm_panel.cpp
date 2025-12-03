#include "svpwm_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QPainter>
#include <QPolygonF>
#include <cmath>

// === svpwm_hexagon 实现 ===
svpwm_hexagon::svpwm_hexagon(QWidget* parent) : QWidget(parent) {
    setMinimumSize(200, 200);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void svpwm_hexagon::set_vector(double u_alpha, double u_beta) {
    m_u_alpha = u_alpha;
    m_u_beta = u_beta;
    update();
}

void svpwm_hexagon::set_sector(int sector) {
    m_sector = sector;
    update();
}

void svpwm_hexagon::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    
    int w = width();
    int h = height();
    int size = qMin(w, h) - 20;
    int cx = w / 2;
    int cy = h / 2;
    int r = size / 2;
    
    // 六边形顶点（基本矢量方向）
    QPointF vertices[6];
    for (int i = 0; i < 6; ++i) {
        double angle = i * M_PI / 3;
        vertices[i] = QPointF(cx + r * std::cos(angle), cy - r * std::sin(angle));
    }
    
    // 绘制六边形区域（根据扇区着色）
    QColor sector_colors[6] = {
        QColor(255, 200, 200), QColor(200, 255, 200), QColor(200, 200, 255),
        QColor(255, 255, 200), QColor(255, 200, 255), QColor(200, 255, 255)
    };
    
    for (int i = 0; i < 6; ++i) {
        QPolygonF triangle;
        triangle << QPointF(cx, cy) << vertices[i] << vertices[(i + 1) % 6];
        if (i + 1 == m_sector) {
            p.setBrush(sector_colors[i]);
        } else {
            p.setBrush(QColor(245, 245, 245));
        }
        p.setPen(QPen(QColor(180, 180, 180), 1));
        p.drawPolygon(triangle);
    }
    
    // 绘制六边形边框
    p.setPen(QPen(QColor(100, 100, 100), 2));
    p.setBrush(Qt::NoBrush);
    QPolygonF hexagon;
    for (int i = 0; i < 6; ++i) {
        hexagon << vertices[i];
    }
    p.drawPolygon(hexagon);
    
    // 绘制坐标轴
    p.setPen(QPen(QColor(150, 150, 150), 1, Qt::DashLine));
    p.drawLine(cx - r, cy, cx + r, cy);
    p.drawLine(cx, cy - r, cx, cy + r);
    
    // 绘制基本矢量标签
    p.setPen(Qt::black);
    QFont font = p.font();
    font.setPointSize(9);
    font.setBold(true);
    p.setFont(font);
    QString labels[6] = {"U1", "U2", "U3", "U4", "U5", "U6"};
    for (int i = 0; i < 6; ++i) {
        double angle = i * M_PI / 3;
        int lx = cx + static_cast<int>((r + 15) * std::cos(angle));
        int ly = cy - static_cast<int>((r + 15) * std::sin(angle));
        p.drawText(lx - 10, ly + 5, labels[i]);
    }
    
    // 绘制电压矢量（限制在六边形内）
    double mag = std::sqrt(m_u_alpha * m_u_alpha + m_u_beta * m_u_beta);
    double max_voltage = 24.0;  // 最大电压（对应六边形边界）
    double scale = r / max_voltage;
    
    // 限制矢量长度不超出六边形
    double draw_alpha = m_u_alpha;
    double draw_beta = m_u_beta;
    if (mag > max_voltage) {
        draw_alpha = m_u_alpha * max_voltage / mag;
        draw_beta = m_u_beta * max_voltage / mag;
    }
    
    int vx = cx + static_cast<int>(draw_alpha * scale);
    int vy = cy - static_cast<int>(draw_beta * scale);
    
    p.setPen(QPen(Qt::red, 2));
    p.drawLine(cx, cy, vx, vy);
    
    // 绘制箭头
    double angle = std::atan2(cy - vy, vx - cx);
    double arr_len = 10;
    double arr_angle = M_PI / 6;
    QPointF tip(vx, vy);
    QPointF arr1(vx - arr_len * std::cos(angle - arr_angle),
                 vy + arr_len * std::sin(angle - arr_angle));
    QPointF arr2(vx - arr_len * std::cos(angle + arr_angle),
                 vy + arr_len * std::sin(angle + arr_angle));
    p.setBrush(Qt::red);
    p.drawPolygon(QPolygonF({tip, arr1, arr2}));
    
    // 绘制扇区编号
    p.setPen(Qt::darkGray);
    font.setPointSize(8);
    font.setBold(false);
    p.setFont(font);
    for (int i = 0; i < 6; ++i) {
        double angle = (i + 0.5) * M_PI / 3;
        int sx = cx + static_cast<int>(r * 0.5 * std::cos(angle));
        int sy = cy - static_cast<int>(r * 0.5 * std::sin(angle));
        p.drawText(sx - 5, sy + 5, QString::number(i + 1));
    }
}

// === svpwm_panel 实现 ===
svpwm_panel::svpwm_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    auto* title = new QLabel("SVPWM空间矢量调制", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    layout->addWidget(title);
    
    // 横向布局：六边形 + 参数
    auto* h_layout = new QHBoxLayout();
    
    // 六边形矢量图
    auto* hex_group = new QGroupBox("空间矢量六边形", this);
    auto* hex_layout = new QVBoxLayout(hex_group);
    m_hexagon = new svpwm_hexagon(this);
    hex_layout->addWidget(m_hexagon);
    h_layout->addWidget(hex_group, 1);
    
    // 数据显示区域
    auto* data_group = new QGroupBox("SVPWM参数", this);
    auto* data_layout = new QGridLayout(data_group);
    
    data_layout->addWidget(new QLabel("扇区:", this), 0, 0);
    m_label_sector = new QLabel("1", this);
    m_label_sector->setStyleSheet("font-weight: bold; color: blue;");
    data_layout->addWidget(m_label_sector, 0, 1);
    
    data_layout->addWidget(new QLabel("Uα:", this), 1, 0);
    m_label_u_alpha = new QLabel("0.00 V", this);
    data_layout->addWidget(m_label_u_alpha, 1, 1);
    
    data_layout->addWidget(new QLabel("Uβ:", this), 2, 0);
    m_label_u_beta = new QLabel("0.00 V", this);
    data_layout->addWidget(m_label_u_beta, 2, 1);
    
    // 占空比进度条
    data_layout->addWidget(new QLabel("Ta:", this), 3, 0);
    m_bar_ta = new QProgressBar(this);
    m_bar_ta->setRange(0, 100);
    m_bar_ta->setStyleSheet("QProgressBar::chunk { background-color: red; }");
    data_layout->addWidget(m_bar_ta, 3, 1);
    
    data_layout->addWidget(new QLabel("Tb:", this), 4, 0);
    m_bar_tb = new QProgressBar(this);
    m_bar_tb->setRange(0, 100);
    m_bar_tb->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    data_layout->addWidget(m_bar_tb, 4, 1);
    
    data_layout->addWidget(new QLabel("Tc:", this), 5, 0);
    m_bar_tc = new QProgressBar(this);
    m_bar_tc->setRange(0, 100);
    m_bar_tc->setStyleSheet("QProgressBar::chunk { background-color: blue; }");
    data_layout->addWidget(m_bar_tc, 5, 1);
    
    h_layout->addWidget(data_group);
    layout->addLayout(h_layout, 1);
}

void svpwm_panel::update_voltage(double u_alpha, double u_beta) {
    m_hexagon->set_vector(u_alpha, u_beta);
    m_label_u_alpha->setText(QString("%1 V").arg(u_alpha, 0, 'f', 2));
    m_label_u_beta->setText(QString("%1 V").arg(u_beta, 0, 'f', 2));
}

void svpwm_panel::update_sector(int sector) {
    m_hexagon->set_sector(sector);
    m_label_sector->setText(QString::number(sector));
}

void svpwm_panel::update_duty(double ta, double tb, double tc) {
    m_bar_ta->setValue(static_cast<int>(ta * 100));
    m_bar_tb->setValue(static_cast<int>(tb * 100));
    m_bar_tc->setValue(static_cast<int>(tc * 100));
}

void svpwm_panel::update_display() {}
