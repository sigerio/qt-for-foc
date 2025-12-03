# 任务清单

## M1. 窗口基础框架
- [x] 1.1 创建 algorithm_explanation_window.h/cpp 窗口类
- [x] 1.2 创建窗口布局（垂直滚动区域，四个主要区域）
- [x] 1.3 修改 main.cpp 添加第三窗口实例化和显示

## M2. 电机模型说明区域
- [x] 2.1 创建 motor_model_explanation_panel.h/cpp
- [x] 2.2 实现参数表格展示（QTableWidget）
- [x] 2.3 实现等效电路简图绘制（QPainter）
- [x] 2.4 添加PMSM/BLDC模型参数说明文字

## M3. 坐标变换说明区域
- [x] 3.1 创建 transform_explanation_panel.h/cpp
- [x] 3.2 实现Clark变换公式（HTML渲染）
- [x] 3.3 实现Park变换公式（HTML渲染）
- [x] 3.4 实现逆变换公式展示
- [x] 3.5 绘制abc/αβ/dq坐标系示意图

## M4. SVPWM说明区域
- [x] 4.1 创建 svpwm_explanation_panel.h/cpp
- [x] 4.2 实现扇区判断公式（HTML渲染）
- [x] 4.3 实现占空比计算公式（HTML渲染）
- [x] 4.4 绘制六边形矢量示意图

## M5. 三环控制说明区域
- [x] 5.1 创建 control_loop_explanation_panel.h/cpp
- [x] 5.2 实现PID公式展示（HTML渲染）
- [x] 5.3 绘制三环串联结构示意图
- [x] 5.4 实现环路使能状态指示

## M6. 集成与完善
- [x] 6.1 集成所有面板到主窗口
- [x] 6.2 更新CMakeLists.txt添加新源文件
- [x] 6.3 调整窗口默认尺寸和位置
