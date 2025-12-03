#include "cascade_widget.h"
#include "block_item.h"
#include "sumpoint_item.h"
#include "label_item.h"
#include "arrow_item.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QRandomGenerator>

cascade_widget::cascade_widget(QWidget* parent) : QWidget(parent)
{
    setup_ui();
}

void cascade_widget::setup_ui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(2);
    
    // 工具栏
    setup_toolbar();
    
    // 图形场景和视图
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 600, 300);
    
    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setBackgroundBrush(QBrush(QColor(252, 252, 252)));
    m_view->setMinimumHeight(250);
    layout->addWidget(m_view, 1);
}

void cascade_widget::setup_toolbar()
{
    QHBoxLayout* toolbar = new QHBoxLayout();
    toolbar->setSpacing(3);
    
    m_btn_block = new QPushButton("+ 矩形块");
    m_btn_block->setFixedHeight(24);
    m_btn_block->setStyleSheet("background:#E0E8FF;");
    connect(m_btn_block, &QPushButton::clicked, this, &cascade_widget::on_add_block);
    toolbar->addWidget(m_btn_block);
    
    m_btn_sumpoint = new QPushButton("+ 求和点");
    m_btn_sumpoint->setFixedHeight(24);
    m_btn_sumpoint->setStyleSheet("background:#FFFFFF;");
    connect(m_btn_sumpoint, &QPushButton::clicked, this, &cascade_widget::on_add_sumpoint);
    toolbar->addWidget(m_btn_sumpoint);
    
    m_btn_label = new QPushButton("+ 标签");
    m_btn_label->setFixedHeight(24);
    connect(m_btn_label, &QPushButton::clicked, this, &cascade_widget::on_add_label);
    toolbar->addWidget(m_btn_label);
    
    m_btn_arrow_h = new QPushButton("+ →");
    m_btn_arrow_h->setFixedHeight(24);
    m_btn_arrow_h->setToolTip("水平箭头");
    connect(m_btn_arrow_h, &QPushButton::clicked, this, &cascade_widget::on_add_arrow_h);
    toolbar->addWidget(m_btn_arrow_h);
    
    m_btn_arrow_v = new QPushButton("+ ↓");
    m_btn_arrow_v->setFixedHeight(24);
    m_btn_arrow_v->setToolTip("垂直箭头");
    connect(m_btn_arrow_v, &QPushButton::clicked, this, &cascade_widget::on_add_arrow_v);
    toolbar->addWidget(m_btn_arrow_v);
    
    toolbar->addSpacing(10);
    
    m_btn_delete = new QPushButton("删除");
    m_btn_delete->setFixedHeight(24);
    m_btn_delete->setStyleSheet("background:#FFE0E0;");
    connect(m_btn_delete, &QPushButton::clicked, this, &cascade_widget::on_delete_selected);
    toolbar->addWidget(m_btn_delete);
    
    toolbar->addStretch();
    
    m_btn_save = new QPushButton("保存");
    m_btn_save->setFixedHeight(24);
    m_btn_save->setStyleSheet("background:#E0FFE0;");
    connect(m_btn_save, &QPushButton::clicked, this, &cascade_widget::on_save);
    toolbar->addWidget(m_btn_save);
    
    m_btn_load = new QPushButton("加载");
    m_btn_load->setFixedHeight(24);
    m_btn_load->setStyleSheet("background:#FFF0D0;");
    connect(m_btn_load, &QPushButton::clicked, this, &cascade_widget::on_load);
    toolbar->addWidget(m_btn_load);
    
    static_cast<QVBoxLayout*>(layout())->addLayout(toolbar);
}

void cascade_widget::on_add_block()
{
    auto* item = new block_item("模块", QColor(224, 232, 255));
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_add_sumpoint()
{
    auto* item = new sumpoint_item();
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_add_label()
{
    auto* item = new label_item("信号");
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_add_arrow_h()
{
    auto* item = new arrow_item(arrow_item::Horizontal);
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_add_arrow_v()
{
    auto* item = new arrow_item(arrow_item::Vertical);
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_delete_selected()
{
    for (auto* item : m_scene->selectedItems()) {
        m_scene->removeItem(item);
        delete item;
    }
}

void cascade_widget::on_save()
{
    QString path = QFileDialog::getSaveFileName(this, "保存布局", "", "JSON (*.json)");
    if (!path.isEmpty()) {
        save_layout(path);
    }
}

void cascade_widget::on_load()
{
    QString path = QFileDialog::getOpenFileName(this, "加载布局", "", "JSON (*.json)");
    if (!path.isEmpty()) {
        load_layout(path);
    }
}

void cascade_widget::save_layout(const QString& filepath)
{
    QJsonArray arr;
    for (auto* item : m_scene->items()) {
        if (auto* block = dynamic_cast<block_item*>(item)) {
            arr.append(block->to_json());
        } else if (auto* sum = dynamic_cast<sumpoint_item*>(item)) {
            arr.append(sum->to_json());
        } else if (auto* label = dynamic_cast<label_item*>(item)) {
            arr.append(label->to_json());
        } else if (auto* arrow = dynamic_cast<arrow_item*>(item)) {
            arr.append(arrow->to_json());
        }
    }
    
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(arr).toJson());
        file.close();
    }
}

void cascade_widget::load_layout(const QString& filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) return;
    
    QJsonArray arr = QJsonDocument::fromJson(file.readAll()).array();
    file.close();
    
    clear_scene();
    
    for (const auto& val : arr) {
        QJsonObject obj = val.toObject();
        QString type = obj["type"].toString();
        
        if (type == "block") {
            m_scene->addItem(block_item::from_json(obj));
        } else if (type == "sumpoint") {
            m_scene->addItem(sumpoint_item::from_json(obj));
        } else if (type == "label") {
            m_scene->addItem(label_item::from_json(obj));
        } else if (type == "arrow") {
            m_scene->addItem(arrow_item::from_json(obj));
        }
    }
}

void cascade_widget::clear_scene()
{
    for (auto* item : m_scene->items()) {
        m_scene->removeItem(item);
        delete item;
    }
}
