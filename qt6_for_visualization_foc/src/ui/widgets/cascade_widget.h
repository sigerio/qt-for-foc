#ifndef UI_WIDGETS_CASCADE_WIDGET_H
#define UI_WIDGETS_CASCADE_WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>

// 可交互的控制流程图编辑器
class cascade_widget : public QWidget {
    Q_OBJECT
public:
    explicit cascade_widget(QWidget* parent = nullptr);
    
    void save_layout(const QString& filepath);
    void load_layout(const QString& filepath);

public slots:
    // 根据预设名称加载配置（current/current_velocity/full）
    void load_preset(const QString& preset_name);

private slots:
    void on_add_block();
    void on_add_sumpoint();
    void on_add_label();
    void on_add_arrow_right();    // →
    void on_add_arrow_left();     // ←
    void on_add_arrow_down();     // ↓
    void on_add_arrow_up();       // ↑
    void on_add_arrow_right_dashed();  // ⇢
    void on_add_arrow_left_dashed();   // ⇠
    void on_add_arrow_down_dashed();   // ⇣
    void on_add_arrow_up_dashed();     // ⇡
    void on_save();
    void on_load();
    void on_delete_selected();

private:
    void setup_ui();
    void setup_toolbar();
    void clear_scene();
    
    QGraphicsScene* m_scene = nullptr;
    QGraphicsView* m_view = nullptr;
    
    // 工具按钮
    QPushButton* m_btn_block = nullptr;
    QPushButton* m_btn_sumpoint = nullptr;
    QPushButton* m_btn_label = nullptr;
    // 实线箭头（四方向）
    QPushButton* m_btn_arrow_right = nullptr;
    QPushButton* m_btn_arrow_left = nullptr;
    QPushButton* m_btn_arrow_down = nullptr;
    QPushButton* m_btn_arrow_up = nullptr;
    // 虚线箭头（四方向）
    QPushButton* m_btn_arrow_right_dashed = nullptr;
    QPushButton* m_btn_arrow_left_dashed = nullptr;
    QPushButton* m_btn_arrow_down_dashed = nullptr;
    QPushButton* m_btn_arrow_up_dashed = nullptr;
    QPushButton* m_btn_save = nullptr;
    QPushButton* m_btn_load = nullptr;
    QPushButton* m_btn_delete = nullptr;
};

#endif // UI_WIDGETS_CASCADE_WIDGET_H
