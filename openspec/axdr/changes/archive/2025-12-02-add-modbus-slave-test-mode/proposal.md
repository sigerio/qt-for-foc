# Change: 添加Modbus从机模拟器和测试模式

## Why
当前系统仅实现了Modbus RTU主机（客户端）功能，无法在没有实际AxDr控制板的情况下进行通信测试。
需要实现从机模拟器，通过物理串口连接进行自发自收测试，便于开发调试和功能验证。

## What Changes
- **ADDED** Modbus RTU从机模拟器模块（通过物理串口通信）
- **ADDED** 从机模拟器独立窗口（数据监测、动态编辑寄存器）
- **ADDED** 可配置的测试数据管理（JSON配置文件）
- **ADDED** 通信日志显示功能

## Impact
- 受影响规范: `serial-communication`
- 受影响代码: `src/serial/`, `src/ui/` 目录
- 新增文件: 
  - `src/serial/modbus_slave.h/cpp` - 从机协议实现
  - `src/serial/test_data_config.h/cpp` - 测试数据配置
  - `src/ui/slave_window.h/cpp` - 从机模拟器窗口
