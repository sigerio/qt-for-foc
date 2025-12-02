#ifndef UI_WIDGETS_STEP_INDICATOR_H
#define UI_WIDGETS_STEP_INDICATOR_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <vector>
#include "core/types.h"

// FOC算法步骤指示器
// 显示算法执行流程，高亮当前步骤
class step_indicator : public QWidget {
    Q_OBJECT
public:
    explicit step_indicator(QWidget* parent = nullptr);
    
public slots:
    // 设置当前高亮步骤
    void set_current_step(e_foc_step step);
    
    // 清除高亮（显示空闲状态）
    void clear_highlight();

private:
    void setup_ui();
    void update_highlight();
    
    e_foc_step m_current = e_foc_step::IDLE;
    std::vector<QLabel*> m_labels;
    
    static const char* step_names[];
};

#endif // UI_WIDGETS_STEP_INDICATOR_H
