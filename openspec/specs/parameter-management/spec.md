# 参数管理模块规格

## Purpose
提供电机控制系统的参数管理功能，包括PID参数、电机物理参数、限制参数的读写，以及控制模式切换和参数本地存储功能。

## Requirements

### Requirement: PID参数管理
系统必须(SHALL)支持读写电流环、速度环、位置环的PID参数。

#### Scenario: 读取PID参数
- **WHEN** 用户请求读取PID参数
- **THEN** 系统从控制板读取Kp、Ki、Kd值并显示

#### Scenario: 写入PID参数
- **WHEN** 用户修改PID参数并确认
- **THEN** 系统将新参数写入控制板并验证

### Requirement: 电机参数管理
系统必须(SHALL)支持读写电机物理参数（极对数、相电阻、相电感、转矩常数）。

#### Scenario: 配置电机参数
- **WHEN** 用户修改电机参数
- **THEN** 系统将参数写入控制板

### Requirement: 限制参数管理
系统必须(SHALL)支持读写限制参数（电流限制、速度限制、电压限制）。

#### Scenario: 设置限制参数
- **WHEN** 用户设置限制值
- **THEN** 系统将限制参数写入控制板

### Requirement: 控制模式切换
系统必须(SHALL)支持切换控制模式（力矩、速度、位置）。

#### Scenario: 切换控制模式
- **WHEN** 用户选择新的控制模式
- **THEN** 系统向控制板发送模式切换命令

### Requirement: 参数本地存储
系统必须(SHALL)支持将参数配置保存到本地文件和从文件加载。

#### Scenario: 保存参数到文件
- **WHEN** 用户点击保存按钮
- **THEN** 系统将当前参数保存到JSON文件

#### Scenario: 从文件加载参数
- **WHEN** 用户点击加载按钮并选择文件
- **THEN** 系统从文件读取参数并更新界面
