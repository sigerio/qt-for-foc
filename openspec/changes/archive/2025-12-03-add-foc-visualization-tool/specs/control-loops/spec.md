# 控制环规格

## ADDED Requirements

### Requirement: 电流环控制
系统必须(SHALL)实现电流环PI控制。

#### Scenario: Id电流控制
- **WHEN** 设定Id目标值
- **THEN** PI控制器调节Id电流跟踪目标

#### Scenario: Iq电流控制
- **WHEN** 设定Iq目标值
- **THEN** PI控制器调节Iq电流跟踪目标

### Requirement: 速度环控制
系统必须(SHALL)实现速度环PI控制。

#### Scenario: 速度跟踪
- **WHEN** 设定目标速度
- **THEN** PI控制器输出Iq参考值使速度跟踪目标

### Requirement: 位置环控制
系统必须(SHALL)实现位置环PID控制。

#### Scenario: 位置跟踪
- **WHEN** 设定目标位置
- **THEN** PID控制器输出速度参考值使位置跟踪目标

### Requirement: 环路启用开关
系统必须(SHALL)支持各环路独立启用/禁用。

#### Scenario: 禁用速度环
- **WHEN** 用户禁用速度环
- **THEN** 速度环不参与控制，直接使用电流环

#### Scenario: 禁用位置环
- **WHEN** 用户禁用位置环
- **THEN** 位置环不参与控制，使用速度环控制

### Requirement: PID参数调节
系统必须(SHALL)支持实时调节PID参数。

#### Scenario: 调节Kp参数
- **WHEN** 用户拖动Kp滑块
- **THEN** 控制器Kp参数实时更新

### Requirement: 负载扰动
系统必须(SHALL)支持注入负载扰动。

#### Scenario: 阶跃负载
- **WHEN** 用户设定负载扰动值
- **THEN** 电机模型施加对应负载转矩

### Requirement: 波形显示
系统必须(SHALL)显示各环路波形。

#### Scenario: 电流波形
- **WHEN** 仿真运行中
- **THEN** 显示Id/Iq目标值与实际值波形

#### Scenario: 速度波形
- **WHEN** 仿真运行中
- **THEN** 显示速度目标值与实际值波形
