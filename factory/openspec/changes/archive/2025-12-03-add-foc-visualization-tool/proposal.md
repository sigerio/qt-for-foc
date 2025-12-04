# Change: 添加FOC算法可视化学习工具

## Why
需要一个纯软件的FOC算法可视化学习工具，帮助理解和学习磁场定向控制（FOC）算法的工作原理。
通过可视化展示坐标变换、SVPWM、三环控制等核心算法，配合单步执行和参数调节功能，提供直观的学习体验。

## What Changes
- **新增** qt6_for_visualization_foc 项目
- **新增** PMSM电机数学模型仿真引擎
- **新增** 坐标变换可视化（Clark/Park变换）
- **新增** SVPWM算法可视化（六边形矢量图）
- **新增** 三环控制可视化（电流环/速度环/位置环）
- **新增** 电机状态可视化（转子动画、三相波形）
- **新增** 交互控制功能（单步执行、参数调节、负载扰动）

## Impact
- Affected specs: 新增 `foc-simulation`、`foc-visualization`、`control-loops` 能力
- Affected code: 全部为新增代码，位于 `qt6_for_visualization_foc/` 目录

## Design Decisions
- 双窗口设计：窗口1展示FOC算法流程，窗口2展示控制环
- 仿真步长：默认10μs，性能不足时可切换100μs
- 纯软件仿真：不依赖硬件，基于PMSM数学模型
- 单步执行：支持算法步骤高亮，便于学习理解
