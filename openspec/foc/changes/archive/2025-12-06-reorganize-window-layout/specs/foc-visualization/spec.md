## MODIFIED Requirements

### Requirement: 双窗口界面
~~系统必须(SHALL)提供双窗口界面布局。~~

系统必须(SHALL)提供三窗口界面布局，职责分离。

#### Scenario: 窗口1显示（参数配置中心）
- **WHEN** 启动应用
- **THEN** 显示FOC算法可视化窗口，包含：
  - 左侧：坐标变换面板、SVPWM面板、电机状态面板
  - 右侧：电机参数配置、三环PID配置、控制工具栏

#### Scenario: 窗口2显示（波形显示中心）
- **WHEN** 启动应用
- **THEN** 显示波形窗口，包含12组数据波形

#### Scenario: 窗口3显示（算法说明选项卡）
- **WHEN** 启动应用
- **THEN** 算法说明窗口默认隐藏，可通过按钮开启

## ADDED Requirements

### Requirement: 参数配置集中管理
系统必须(SHALL)将所有参数配置集中在窗口1。

#### Scenario: 三环PID参数配置
- **WHEN** 用户调节PID参数
- **THEN** 在窗口1右侧的PID配置面板中进行调节

#### Scenario: 控制工具栏
- **WHEN** 用户操作仿真控制
- **THEN** 在窗口1的控制工具栏中进行（运行/暂停/单步/复位）

#### Scenario: 环路使能开关
- **WHEN** 用户切换环路使能状态
- **THEN** 在窗口1的PID配置面板中切换

### Requirement: 完整波形显示
系统必须(SHALL)在窗口2显示完整的FOC数据波形，采用9行布局（第1行两列，其余单列）。

#### Scenario: dq坐标系电压电流波形（第1行两列）
- **WHEN** 仿真运行中
- **THEN** 第1行左侧显示 Ud、Uq 电压波形，右侧显示 Id、Iq 电流波形

#### Scenario: αβ坐标系电压波形（第2行）
- **WHEN** 仿真运行中
- **THEN** 显示 Uα、Uβ 电压波形

#### Scenario: αβ坐标系电流波形（第3行）
- **WHEN** 仿真运行中
- **THEN** 显示 Iα、Iβ 电流波形

#### Scenario: 三相电压波形（第4行）
- **WHEN** 仿真运行中
- **THEN** 显示 Ua、Ub、Uc 三相电压波形

#### Scenario: 三相电流波形（第5行）
- **WHEN** 仿真运行中
- **THEN** 显示 Ia、Ib、Ic 三相电流波形

#### Scenario: PWM占空比波形（第6行）
- **WHEN** 仿真运行中
- **THEN** 显示 Ta、Tb、Tc PWM占空比波形

#### Scenario: 电流参考值波形（第7行）
- **WHEN** 仿真运行中
- **THEN** 显示 id_ref、iq_ref 电流参考值波形

#### Scenario: 速度波形（第8行）
- **WHEN** 仿真运行中
- **THEN** 显示 vel_ref 速度参考值和 velocity 实际速度波形

#### Scenario: 位置波形（第9行）
- **WHEN** 仿真运行中
- **THEN** 显示 pos_ref 位置参考值和 position 实际位置波形

### Requirement: 算法说明窗口选项卡化
系统必须(SHALL)将算法说明窗口改为选项卡形式，支持开关控制。

#### Scenario: 选项卡组织
- **WHEN** 打开算法说明窗口
- **THEN** 内容以选项卡形式组织：电机模型 | 坐标变换 | SVPWM | 三环控制

#### Scenario: 窗口开关
- **WHEN** 用户点击窗口1的"算法说明"按钮
- **THEN** 切换算法说明窗口的显示/隐藏状态

#### Scenario: 默认隐藏
- **WHEN** 应用启动
- **THEN** 算法说明窗口默认不显示
