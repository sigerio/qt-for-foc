# Change: 新增算法说明窗口

## Why
用户需要一个专门的窗口来展示FOC算法的详细说明，包括电机模型参数、坐标变换公式、SVPWM算法和三环控制原理，便于学习和理解整个闭环控制流程。

## What Changes
- **新增** 第三窗口：算法说明窗口（algorithm_explanation_window）
- **新增** 电机模型参数展示区域（含参数表格和等效电路图示）
- **新增** 坐标变换说明区域（Clark/Park公式，HTML渲染）
- **新增** SVPWM算法说明区域（扇区判断、占空比计算公式，六边形图示）
- **新增** 三环控制说明区域（PID公式、串联结构图示、使能状态指示）

## Impact
- Affected specs: 修改 `foc-visualization` 能力（新增窗口）
- Affected code: 
  - 新增 `src/ui/algorithm_explanation_window.h/cpp`
  - 新增 `src/ui/panels/` 下的说明面板
  - 修改 `main.cpp` 添加第三窗口

## Design Decisions
- 公式展示：使用 QTextBrowser + HTML 实体渲染
- 图示绘制：使用 QPainter 简单绘制，不使用外部图片资源
- 内容静态：初始版本为静态展示，预留后续动态扩展接口
- 窗口独立：与现有双窗口并列，可独立开关
