# 变更：控制框图配置联动与箭头组件增强

## 为什么
当前窗口3的控制框图编辑器（cascade_widget）支持手动搭建和保存布局，但存在以下问题：
1. 与窗口2的控制环使能状态没有联动
2. 箭头组件不支持命名标注
3. 箭头交叉时存在视觉重叠问题

## 变更内容
1. 定义三种预设框图配置文件：
   - `cascade_current.json` - 仅电流环控制框图
   - `cascade_current_velocity.json` - 电流+速度环控制框图
   - `cascade_full.json` - 电流+速度+位置环完整框图
2. 窗口3 的 cascade_widget 根据窗口2 使能的控制环自动加载对应配置
3. 配置文件存储在项目根目录的 `config/` 文件夹中
4. 增强所有框图组件的中文支持：
   - arrow_item：添加标签功能（双击编辑、显示在箭头旁边）
   - sumpoint_item：添加可选标签功能（双击编辑、显示在求和点旁边）
   - 所有组件统一使用 QInputDialog 支持中文输入
   - 优化箭头样式（线条透明度/虚线）避免交叉重叠

## 实现路径
1. 先完善箭头组件功能（命名、中文、样式优化）
2. 用户通过窗口3的 cascade_widget 手动搭建三种控制框图，分别保存为上述配置文件
3. 窗口2 根据使能的控制环发出信号，通知窗口3 加载对应配置
4. cascade_widget 提供 API 支持外部触发加载指定配置

## 影响
- 受影响的规格：foc-visualization
- 受影响的代码：
  - `src/ui/widgets/arrow_item.h/cpp` - 添加标签功能、样式优化
  - `src/ui/widgets/sumpoint_item.h/cpp` - 添加标签功能
  - `src/ui/widgets/cascade_widget.h/cpp` - 添加外部加载接口
  - `src/ui/control_loop_window.h/cpp` - 添加环路使能状态信号
  - `src/ui/algorithm_explanation_window.h/cpp` - 响应使能状态变化
  - `main.cpp` - 连接窗口间信号
