# foc-visualization Specification Delta

## ADDED Requirements

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
