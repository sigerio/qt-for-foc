#ifndef UI_WIDGETS_HALL_INDICATOR_H
#define UI_WIDGETS_HALL_INDICATOR_H

#include <QWidget>
#include <QLabel>
#include "core/types.h"

// 霍尔传感器状态指示器
// 显示三个霍尔传感器状态、换向扇区和霍尔编码
class hall_indicator : public QWidget {
    Q_OBJECT
public:
    explicit hall_indicator(QWidget* parent = nullptr);

public slots:
    // 更新霍尔状态
    void set_hall_state(const hall_state_t& state);
    
    // 清除状态
    void clear();

private:
    void setup_ui();
    void update_display();
    
    hall_state_t m_state;
    
    // LED指示灯
    QLabel* m_led_ha = nullptr;
    QLabel* m_led_hb = nullptr;
    QLabel* m_led_hc = nullptr;
    
    // 状态显示
    QLabel* m_label_sector = nullptr;
    QLabel* m_label_code = nullptr;
};

#endif // UI_WIDGETS_HALL_INDICATOR_H
