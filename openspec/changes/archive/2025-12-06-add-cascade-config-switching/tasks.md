## 1. 框图组件中文标签增强（优先实施）

### 1.1 arrow_item 组件增强
- [x] 1.1.1 添加 `m_label` 成员变量存储箭头名称
- [x] 1.1.2 添加 `set_label(const QString&)` / `label()` 接口
- [x] 1.1.3 修改双击事件：弹出 QInputDialog 编辑名称（替代切换方向）
- [x] 1.1.4 在 `paint()` 中绘制标签文字（水平箭头上方/垂直箭头左侧）
- [x] 1.1.5 修改箭头样式：使用半透明线条避免交叉重叠
- [x] 1.1.6 更新 `to_json()` / `from_json()` 序列化标签

### 1.2 sumpoint_item 组件增强
- [x] 1.2.1 添加 `m_label` 成员变量存储求和点名称
- [x] 1.2.2 添加 `set_label(const QString&)` / `label()` 接口
- [x] 1.2.3 添加双击事件：弹出 QInputDialog 编辑名称
- [x] 1.2.4 在 `paint()` 中绘制标签文字（求和点下方）
- [x] 1.2.5 更新 `to_json()` / `from_json()` 序列化标签

## 2. 用户手动创建框图配置（用户操作）
- [ ] 2.1 使用 cascade_widget 搭建仅电流环框图，保存为 `config/cascade_current.json`
- [ ] 2.2 使用 cascade_widget 搭建电流+速度环框图，保存为 `config/cascade_current_velocity.json`
- [ ] 2.3 使用 cascade_widget 搭建完整三环框图，保存为 `config/cascade_full.json`

## 3. cascade_widget 外部加载接口
- [x] 3.1 在 cascade_widget.h 添加公共槽函数 `load_preset(const QString& preset_name)`
- [x] 3.2 实现 `load_preset` 从 config/ 目录加载对应配置文件

## 4. control_loop_window 环路使能信号
- [x] 4.1 添加信号 `loop_preset_changed(const QString& preset)` - "current"/"current_velocity"/"full"
- [x] 4.2 添加槽函数 `on_loop_enable_changed()` 连接各环面板使能信号
- [x] 4.3 为各环路面板添加 `is_enabled()` 方法

## 5. 窗口间信号连接
- [x] 5.1 control_loop_explanation_panel 添加 `get_cascade_widget()` 获取器
- [x] 5.2 algorithm_explanation_window 添加 `control_panel()` 获取器
- [x] 5.3 main.cpp 中连接 control_loop_window::loop_preset_changed 到 cascade_widget::load_preset

## 6. 验证
- [x] 6.1 箭头命名功能：双击输入中文名称，保存/加载正确
- [x] 6.2 箭头样式：交叉时视觉清晰，无重叠遮挡
- [x] 6.3 切换控制环使能状态，确认框图自动切换
