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

### Requirement: 算法说明窗口
系统必须(SHALL)提供独立的算法说明窗口，展示FOC算法的详细原理说明。

#### Scenario: 启动时显示第三窗口
- **WHEN** 应用启动
- **THEN** 除原有双窗口外，显示算法说明窗口

#### Scenario: 窗口可独立开关
- **WHEN** 用户关闭算法说明窗口
- **THEN** 窗口隐藏，不影响其他窗口运行

### Requirement: 电机模型参数展示
系统必须(SHALL)在算法说明窗口展示当前电机模型的关键参数及其含义。

#### Scenario: 显示PMSM参数
- **WHEN** 选择PMSM电机类型
- **THEN** 显示Rs、Ld、Lq、ψf、J、B、极对数等参数及其物理含义

#### Scenario: 显示等效电路
- **WHEN** 查看电机模型区域
- **THEN** 显示电机等效电路简图

### Requirement: 坐标变换公式展示
系统必须(SHALL)在算法说明窗口展示Clark/Park变换的数学公式和坐标系示意图。

#### Scenario: 显示Clark变换公式
- **WHEN** 查看坐标变换区域
- **THEN** 以HTML格式显示Clark变换矩阵公式

#### Scenario: 显示Park变换公式
- **WHEN** 查看坐标变换区域
- **THEN** 以HTML格式显示Park变换矩阵公式

#### Scenario: 显示坐标系图示
- **WHEN** 查看坐标变换区域
- **THEN** 显示abc、αβ、dq三个坐标系的关系示意图

### Requirement: SVPWM算法展示
系统必须(SHALL)在算法说明窗口展示SVPWM算法的公式和六边形矢量图。

#### Scenario: 显示扇区判断公式
- **WHEN** 查看SVPWM区域
- **THEN** 显示扇区判断的数学公式

#### Scenario: 显示六边形示意
- **WHEN** 查看SVPWM区域
- **THEN** 显示六边形矢量分布示意图

### Requirement: 三环控制说明
系统必须(SHALL)在算法说明窗口展示三环控制的PID公式和串联结构。

#### Scenario: 显示PID公式
- **WHEN** 查看三环控制区域
- **THEN** 以HTML格式显示PID控制公式

#### Scenario: 显示串联结构
- **WHEN** 查看三环控制区域
- **THEN** 显示位置环→速度环→电流环的串联结构示意图

