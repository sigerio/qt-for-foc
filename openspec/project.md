# Project Context

本仓库包含两个独立的 Qt6 项目：

---

## 项目1: FOC算法可视化学习工具 (qt6_for_visualization_foc)

### Purpose
纯软件模拟的FOC算法可视化学习工具，脱离硬件平台，用于学习和理解FOC控制算法。

### Tech Stack
- Qt6 (Core, Gui, Widgets)
- C++17
- CMake 3.16+

### Technical Specifications
- 仿真步长: 10μs（可选100μs）
- 界面: 三窗口多面板
- 电机模型: PMSM / BLDC

### Interface Design

**窗口1 - FOC算法可视化 + 参数配置**
- 左侧：坐标变换面板、SVPWM面板、转子动画+步骤指示器
- 右侧：控制工具栏、电机参数、三环PID配置

**窗口2 - 波形显示**
- 可调整行高的多波形视图
- dq电压/电流、αβ电压/电流、三相电压/电流、PWM、速度、位置

**窗口3 - 算法说明**
- 电机模型、坐标变换、SVPWM说明面板（并排）
- 三环控制说明面板（含框图编辑器）

### Project File Structure
```
qt6_for_visualization_foc/
├── CMakeLists.txt
├── main.cpp
├── src/
│   ├── core/               # 核心仿真引擎
│   ├── control/            # 控制算法
│   └── ui/                 # 用户界面
│       ├── panels/         # 面板组件
│       └── widgets/        # 自定义控件
└── resources/
```

---

## 项目2: AxDr电机控制板上位机 (qt6_for_axdr)

### Purpose
基于Qt6的串口上位机软件，用于控制AxDr电机控制板（FOC驱动器）。
通过Modbus RTU协议与下位机通信，实现电机参数的实时监控和调节。

### Tech Stack
- Qt6 (Core, Gui, Widgets, SerialPort)
- C++17
- CMake 3.16+
- Modbus RTU协议（手动实现）

### Features
- Modbus RTU通信模块
- 参数管理（PID、电机、限制参数）
- 串口配置、PID配置、电机配置界面
- 从机模拟器
- 通信日志记录

### Project File Structure
```
qt6_for_axdr/
├── CMakeLists.txt
├── main.cpp
├── docs/protocol.md
├── src/
│   ├── serial/             # 串口通信
│   ├── params/             # 参数管理
│   ├── slave/              # 从机模拟器
│   ├── log/                # 日志
│   └── ui/                 # 界面
└── fonts/
```

---

## 通用约定

### Code Style
- 代码注释使用中文
- 错误信息使用中文
- **禁止使用驼峰命名法**
- 变量命名：snake_case
- 结构体类型：`_t` 结尾
- 宏定义：全大写
- 枚举类型：`_E` 结尾
- 枚举变量：`e_` 开头

### Architecture Patterns
- MVC架构模式
- 核心模块与UI解耦

### Git Workflow
- 主分支: main
- 功能分支: feature/xxx
- 提交信息使用中文描述

### Important Constraints
- 必须支持Windows和Linux平台
- UI响应不能被计算/通信阻塞

### External Dependencies
- Qt6 Core, Gui, Widgets
- Qt6 SerialPort (仅 axdr 项目)
