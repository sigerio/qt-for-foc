# Change: 添加串口电机控制上位机功能

## Why
需要一个上位机软件来控制AxDr电机控制板（FOC驱动器）的参数，包括PID参数调节、电机参数配置、控制模式切换等功能。
通过Modbus RTU协议与下位机通信，实现参数的实时读写和监控。

## What Changes
- **新增** 串口通信模块（Modbus RTU协议）
- **新增** 参数管理模块（PID、电机参数、限制参数等）
- **新增** 主界面UI（参数显示、调节控件）
- **新增** 实时数据监控功能
- **新增** 参数保存/加载功能

## Impact
- Affected specs: 新增 `serial-communication`、`parameter-management`、`motor-control-ui` 能力
- Affected code: 
  - `qt6_for_axdr/src/serial/` - 串口通信模块
  - `qt6_for_axdr/src/params/` - 参数管理模块
  - `qt6_for_axdr/src/ui/` - 用户界面模块
  - `qt6_for_axdr/main.cpp` - 主程序入口

## Design Principles
- 通信速度和解析速度优先
- UI响应不被串口通信阻塞（异步通信）
- 模块化设计，串口通信与UI解耦
