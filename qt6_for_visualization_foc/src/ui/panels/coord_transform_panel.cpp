#include "coord_transform_panel.h"
#include "../widgets/vector_scope.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>

coord_transform_panel::coord_transform_panel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    auto* title = new QLabel("坐标变换 (Clark/Park)", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    layout->addWidget(title);
    
    // 矢量图区域
    auto* scope_layout = new QHBoxLayout();
    
    // αβ坐标系
    auto* ab_group = new QGroupBox("αβ坐标系", this);
    auto* ab_layout = new QVBoxLayout(ab_group);
    m_ab_scope = new vector_scope(this);
    m_ab_scope->set_range(2.0);
    m_ab_scope->set_color(QColor(0, 150, 255));
    ab_layout->addWidget(m_ab_scope);
    scope_layout->addWidget(ab_group);
    
    // dq坐标系
    auto* dq_group = new QGroupBox("dq坐标系", this);
    auto* dq_layout = new QVBoxLayout(dq_group);
    m_dq_scope = new vector_scope(this);
    m_dq_scope->set_range(2.0);
    m_dq_scope->set_color(QColor(255, 100, 50));
    m_dq_scope->set_show_trail(false);
    dq_layout->addWidget(m_dq_scope);
    scope_layout->addWidget(dq_group);
    
    layout->addLayout(scope_layout, 1);
    
    // 数值显示区域
    auto* data_group = new QGroupBox("电流数值", this);
    auto* data_layout = new QGridLayout(data_group);
    
    // abc三相
    data_layout->addWidget(new QLabel("Ia:", this), 0, 0);
    m_label_ia = new QLabel("0.00 A", this);
    m_label_ia->setStyleSheet("color: red;");
    data_layout->addWidget(m_label_ia, 0, 1);
    
    data_layout->addWidget(new QLabel("Ib:", this), 0, 2);
    m_label_ib = new QLabel("0.00 A", this);
    m_label_ib->setStyleSheet("color: green;");
    data_layout->addWidget(m_label_ib, 0, 3);
    
    data_layout->addWidget(new QLabel("Ic:", this), 0, 4);
    m_label_ic = new QLabel("0.00 A", this);
    m_label_ic->setStyleSheet("color: blue;");
    data_layout->addWidget(m_label_ic, 0, 5);
    
    // αβ
    data_layout->addWidget(new QLabel("Iα:", this), 1, 0);
    m_label_i_alpha = new QLabel("0.00 A", this);
    data_layout->addWidget(m_label_i_alpha, 1, 1);
    
    data_layout->addWidget(new QLabel("Iβ:", this), 1, 2);
    m_label_i_beta = new QLabel("0.00 A", this);
    data_layout->addWidget(m_label_i_beta, 1, 3);
    
    // dq
    data_layout->addWidget(new QLabel("Id:", this), 1, 4);
    m_label_id = new QLabel("0.00 A", this);
    data_layout->addWidget(m_label_id, 1, 5);
    
    data_layout->addWidget(new QLabel("Iq:", this), 2, 0);
    m_label_iq = new QLabel("0.00 A", this);
    data_layout->addWidget(m_label_iq, 2, 1);
    
    layout->addWidget(data_group);
}

void coord_transform_panel::update_alpha_beta(double i_alpha, double i_beta) {
    m_ab_scope->set_vector(i_alpha, i_beta);
    m_label_i_alpha->setText(QString("%1 A").arg(i_alpha, 0, 'f', 2));
    m_label_i_beta->setText(QString("%1 A").arg(i_beta, 0, 'f', 2));
}

void coord_transform_panel::update_dq(double i_d, double i_q) {
    m_dq_scope->set_vector(i_d, i_q);
    m_label_id->setText(QString("%1 A").arg(i_d, 0, 'f', 2));
    m_label_iq->setText(QString("%1 A").arg(i_q, 0, 'f', 2));
}

void coord_transform_panel::update_abc(double i_a, double i_b, double i_c) {
    m_label_ia->setText(QString("%1 A").arg(i_a, 0, 'f', 2));
    m_label_ib->setText(QString("%1 A").arg(i_b, 0, 'f', 2));
    m_label_ic->setText(QString("%1 A").arg(i_c, 0, 'f', 2));
}

void coord_transform_panel::update_display() {}
