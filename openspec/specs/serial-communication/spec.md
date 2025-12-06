# 串口通信模块规格

## Purpose
定义AxDr电机控制上位机与下位机之间的Modbus RTU串口通信规范，包括连接管理、数据读写、错误处理等功能。
## Requirements
### Requirement: Modbus RTU通信
系统必须(SHALL)支持通过Modbus RTU协议与AxDr电机控制板通信。

#### Scenario: 建立串口连接
- **WHEN** 用户选择串口并点击连接
- **THEN** 系统建立Modbus RTU连接并显示连接状态

#### Scenario: 读取寄存器
- **WHEN** 系统需要读取参数
- **THEN** 发送Modbus读取命令并解析响应数据

#### Scenario: 写入寄存器
- **WHEN** 用户修改参数并确认
- **THEN** 发送Modbus写入命令并验证写入结果

### Requirement: 异步通信
系统必须(SHALL)使用异步方式进行串口通信，不阻塞UI线程。

#### Scenario: 异步读取不阻塞UI
- **WHEN** 系统正在进行串口通信
- **THEN** UI界面保持响应，用户可以进行其他操作

### Requirement: 通信错误处理
系统必须(SHALL)处理通信超时和错误，并提供重连机制。

#### Scenario: 通信超时处理
- **WHEN** 串口通信超时
- **THEN** 系统显示错误提示并尝试重连

### Requirement: Modbus从机模拟器
系统必须(SHALL)提供Modbus RTU从机模拟器，用于通过物理串口进行通信测试。

#### Scenario: 从机监听串口
- **WHEN** 用户在从机模拟器窗口配置串口并启动监听
- **THEN** 从机模拟器开始监听指定串口的Modbus请求

#### Scenario: 从机响应读取请求
- **WHEN** 主机发送读保持寄存器请求(0x03)
- **THEN** 从机模拟器根据当前寄存器数据返回对应值

#### Scenario: 从机响应写入请求
- **WHEN** 主机发送写寄存器请求(0x06/0x10)
- **THEN** 从机模拟器更新内部寄存器并返回确认响应

#### Scenario: 从机异常响应
- **WHEN** 主机请求的寄存器地址超出范围
- **THEN** 从机模拟器返回Modbus异常响应码

### Requirement: 从机模拟器独立窗口
系统必须(SHALL)提供独立的从机模拟器窗口，支持数据监测和动态修改。

#### Scenario: 打开从机模拟器窗口
- **WHEN** 用户从主窗口菜单选择"打开从机模拟器"
- **THEN** 系统显示独立的从机模拟器窗口

#### Scenario: 显示寄存器表格
- **WHEN** 从机模拟器窗口打开
- **THEN** 显示寄存器地址、名称、当前值的表格视图

#### Scenario: 动态编辑寄存器值
- **WHEN** 用户双击寄存器表格中的值
- **THEN** 可以直接编辑修改寄存器值，修改立即生效

#### Scenario: 显示通信日志
- **WHEN** 从机收发Modbus数据包
- **THEN** 在通信日志区域显示收发的原始数据和解析结果

### Requirement: 测试数据配置
系统必须(SHALL)支持通过JSON文件配置和保存测试数据。

#### Scenario: 加载测试数据配置
- **WHEN** 用户点击"加载配置"按钮
- **THEN** 从JSON文件读取寄存器名称和初始值

#### Scenario: 保存测试数据配置
- **WHEN** 用户点击"保存配置"按钮
- **THEN** 当前寄存器值和名称保存到JSON文件

