#include "motor_model_explanation_panel.h"
#include "../widgets/circuit_widget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>

motor_model_explanation_panel::motor_model_explanation_panel(QWidget* parent)
    : QGroupBox("一、电机模型", parent)
{
    setup_ui();
}

void motor_model_explanation_panel::setup_ui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(5);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // 说明文字
    m_description = new QLabel(
        "<b>PMSM（永磁同步电机）</b>基于dq旋转坐标系建模，将三相交流量转换为直流量。"
        "<b>特性：</b>d轴对齐永磁磁链，q轴产生转矩。"
    );
    m_description->setTextFormat(Qt::RichText);
    m_description->setWordWrap(true);
    layout->addWidget(m_description);
    
    // 参数表格
    m_param_table = new QTableWidget(7, 3);
    m_param_table->setHorizontalHeaderLabels({"参数", "符号", "含义"});
    m_param_table->horizontalHeader()->setStretchLastSection(true);
    m_param_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_param_table->verticalHeader()->setDefaultSectionSize(22);
    
    QStringList params = {"定子电阻", "d轴电感", "q轴电感", "永磁磁链", "转动惯量", "阻尼系数", "极对数"};
    QStringList symbols = {"Rs", "Ld", "Lq", "ψf", "J", "B", "p"};
    QStringList meanings = {"铜损", "弱磁控制", "转矩响应", "反电动势", "加速性能", "稳态误差", "电角度转换"};
    
    for (int i = 0; i < 7; ++i) {
        m_param_table->setItem(i, 0, new QTableWidgetItem(params[i]));
        m_param_table->setItem(i, 1, new QTableWidgetItem(symbols[i]));
        m_param_table->setItem(i, 2, new QTableWidgetItem(meanings[i]));
    }
    layout->addWidget(m_param_table);
    
    // 等效电路
    QLabel* circuit_title = new QLabel("<b>dq轴等效电路：</b>");
    layout->addWidget(circuit_title);
    
    m_circuit_widget = new circuit_widget();
    layout->addWidget(m_circuit_widget, 1);
}
