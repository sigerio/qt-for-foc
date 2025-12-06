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
    
    // 实线箭头（四方向）
    m_btn_arrow_right = new QPushButton("→");
    m_btn_arrow_right->setFixedHeight(24);
    m_btn_arrow_right->setToolTip("向右箭头");
    connect(m_btn_arrow_right, &QPushButton::clicked, this, &cascade_widget::on_add_arrow_right);
    toolbar->addWidget(m_btn_arrow_right);
    
    m_btn_arrow_left = new QPushButton("←");
    m_btn_arrow_left->setFixedHeight(24);
    m_btn_arrow_left->setToolTip("向左箭头");
    connect(m_btn_arrow_left, &QPushButton::clicked, this, &cascade_widget::on_add_arrow_left);
    toolbar->addWidget(m_btn_arrow_left);
    
    m_btn_arrow_down = new QPushButton("↓");
    m_btn_arrow_down->setFixedHeight(24);
    m_btn_arrow_down->setToolTip("向下箭头");
    connect(m_btn_arrow_down, &QPushButton::clicked, this, &cascade_widget::on_add_arrow_down);
    toolbar->addWidget(m_btn_arrow_down);
    
    m_btn_arrow_up = new QPushButton("↑");
    m_btn_arrow_up->setFixedHeight(24);
    m_btn_arrow_up->setToolTip("向上箭头");
    connect(m_btn_arrow_up, &QPushButton::clicked, this, &cascade_widget::on_add_arrow_up);
    toolbar->addWidget(m_btn_arrow_up);
    
    // 虚线箭头（四方向）
    m_btn_arrow_right_dashed = new QPushButton("⇢");
    m_btn_arrow_right_dashed->setFixedHeight(24);
    m_btn_arrow_right_dashed->setToolTip("向右虚线箭头");
    connect(m_btn_arrow_right_dashed, &QPushButton::clicked, this, &cascade_widget::on_add_arrow_right_dashed);
    toolbar->addWidget(m_btn_arrow_right_dashed);
    
    m_btn_arrow_left_dashed = new QPushButton("⇠");
    m_btn_arrow_left_dashed->setFixedHeight(24);
    m_btn_arrow_left_dashed->setToolTip("向左虚线箭头");
    connect(m_btn_arrow_left_dashed, &QPushButton::clicked, this, &cascade_widget::on_add_arrow_left_dashed);
    toolbar->addWidget(m_btn_arrow_left_dashed);
    
    m_btn_arrow_down_dashed = new QPushButton("⇣");
    m_btn_arrow_down_dashed->setFixedHeight(24);
    m_btn_arrow_down_dashed->setToolTip("向下虚线箭头");
    connect(m_btn_arrow_down_dashed, &QPushButton::clicked, this, &cascade_widget::on_add_arrow_down_dashed);
    toolbar->addWidget(m_btn_arrow_down_dashed);
    
    m_btn_arrow_up_dashed = new QPushButton("⇡");
    m_btn_arrow_up_dashed->setFixedHeight(24);
    m_btn_arrow_up_dashed->setToolTip("向上虚线箭头");
    connect(m_btn_arrow_up_dashed, &QPushButton::clicked, this, &cascade_widget::on_add_arrow_up_dashed);
    toolbar->addWidget(m_btn_arrow_up_dashed);
    
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

void cascade_widget::on_add_arrow_right()
{
    auto* item = new arrow_item(arrow_item::Right);
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_add_arrow_left()
{
    auto* item = new arrow_item(arrow_item::Left);
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_add_arrow_down()
{
    auto* item = new arrow_item(arrow_item::Down);
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_add_arrow_up()
{
    auto* item = new arrow_item(arrow_item::Up);
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_add_arrow_right_dashed()
{
    auto* item = new arrow_item(arrow_item::Right, arrow_item::Dashed);
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_add_arrow_left_dashed()
{
    auto* item = new arrow_item(arrow_item::Left, arrow_item::Dashed);
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_add_arrow_down_dashed()
{
    auto* item = new arrow_item(arrow_item::Down, arrow_item::Dashed);
    item->setPos(50 + QRandomGenerator::global()->bounded(100), 
                 50 + QRandomGenerator::global()->bounded(100));
    m_scene->addItem(item);
}

void cascade_widget::on_add_arrow_up_dashed()
{
    auto* item = new arrow_item(arrow_item::Up, arrow_item::Dashed);
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

void cascade_widget::load_preset(const QString& preset_name)
{
    // 根据预设名称构建配置文件路径
    QString filename;
    if (preset_name == "current") {
        filename = "cascade_current.json";
    } else if (preset_name == "current_velocity") {
        filename = "cascade_current_velocity.json";
    } else if (preset_name == "full") {
        filename = "cascade_full.json";
    } else {
        return;  // 未知预设
    }
    
    // 从 config/ 目录加载
    QString path = "config/" + filename;
    load_layout(path);
}
