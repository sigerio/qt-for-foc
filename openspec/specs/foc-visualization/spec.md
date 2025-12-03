# foc-visualization Specification

## Purpose
提供FOC算法的可视化学习界面，采用双窗口设计，展示坐标变换矢量圆、SVPWM六边形矢量图、转子旋转动画、三相波形等可视化效果，并支持单步执行和算法步骤高亮功能。
## Requirements
### Requirement: 双窗口界面
系统必须(SHALL)提供双窗口界面布局。

#### Scenario: 窗口1显示
- **WHEN** 启动应用
- **THEN** 显示FOC算法可视化窗口（坐标变换、SVPWM、电机状态）

#### Scenario: 窗口2显示
- **WHEN** 启动应用
- **THEN** 显示三环控制窗口（电流环、速度环、位置环）

### Requirement: 坐标变换可视化
系统必须(SHALL)可视化展示坐标变换过程。

#### Scenario: 矢量圆动画
- **WHEN** 仿真运行中
- **THEN** 显示Iα-Iβ矢量圆动画和实时数值

### Requirement: SVPWM可视化
系统必须(SHALL)可视化展示SVPWM算法。

#### Scenario: 六边形矢量图
- **WHEN** 仿真运行中
- **THEN** 显示电压矢量六边形和当前矢量位置

#### Scenario: 扇区高亮
- **WHEN** 矢量在某扇区
- **THEN** 高亮显示对应扇区

### Requirement: 电机状态可视化
系统必须(SHALL)可视化展示电机运行状态。

#### Scenario: 转子动画
- **WHEN** 仿真运行中
- **THEN** 显示转子旋转动画

#### Scenario: 三相波形
- **WHEN** 仿真运行中
- **THEN** 显示三相电流正弦波形

### Requirement: 单步执行模式
系统必须(SHALL)支持单步执行和算法高亮。

#### Scenario: 单步执行
- **WHEN** 用户点击单步按钮
- **THEN** 执行一个仿真步骤并暂停

#### Scenario: 算法高亮
- **WHEN** 单步执行时
- **THEN** 高亮显示当前执行的算法步骤

