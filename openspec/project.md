# Project Context

## Purpose
开发一个基于Qt6的串口上位机软件，用于控制AxDr电机控制板（FOC驱动器）的相关参数。
通过Modbus RTU协议与下位机通信，实现电机参数的实时监控和调节。

## Tech Stack
- Qt6 (Core, Gui, Widgets, SerialPort)
- C++17
- CMake 3.16+
- Modbus RTU协议（手动实现，不依赖SerialBus）

## Current Progress (2025-12-02)

### 已完成功能
- [x] 项目基础架构搭建
- [x] Modbus RTU通信模块（使用QSerialPort手动实现）
- [x] 参数管理模块（PID、电机、限制参数）
- [x] 用户界面模块（串口配置、PID配置、电机配置）
- [x] 配置文件保存/加载（JSON格式）
- [x] 实时数据显示
- [x] 从机模拟器（独立窗口，支持物理串口测试）
- [x] 通信日志记录（收发数据包hex dump）
- [x] 协议文档（docs/protocol.md）

### 已修复问题
- [x] 死锁问题：QMutex → QRecursiveMutex
- [x] 请求冲突：添加m_is_busy标志防止并发请求
- [x] 小端序float转换（union方式）
- [x] 从机寄存器映射与主机一致（每个float占2个寄存器）
- [x] 从机界面按数据类型显示（float/整型）

### 项目文件结构
```
qt6_for_axdr/
├── CMakeLists.txt          # CMake构建配置
├── Makefile                # 构建辅助脚本
├── main.cpp                # 主程序入口
├── resources.qrc           # Qt资源文件
├── fonts/                  # 字体文件（运行时加载）
│   └── AlibabaPuHuiTi-Regular.ttf
├── docs/                   # 文档
│   └── protocol.md         # Modbus RTU协议文档
└── src/
    ├── serial/             # 串口通信模块
    │   ├── modbus_client.h/cpp
    ├── params/             # 参数管理模块
    │   ├── motor_params.h/cpp
    │   ├── param_manager.h/cpp
    ├── slave/              # 从机模拟器模块
    │   ├── modbus_slave.h/cpp      # Modbus从机实现
    │   ├── slave_window.h/cpp      # 从机独立窗口
    │   ├── test_data_config.h/cpp  # 测试数据配置
    ├── log/                # 日志模块
    │   └── comm_logger.h/cpp       # 通信日志记录
    └── ui/                 # 用户界面模块
        ├── main_window.h/cpp
        ├── serial_config_widget.h/cpp
        ├── pid_config_widget.h/cpp
        └── motor_config_widget.h/cpp
```

### 待完成功能
- [ ] 实际串口通信测试（需连接AxDr控制板）
- [ ] 实时数据波形显示
- [ ] 参数曲线调节
- [ ] 多语言支持

## Project Conventions

### Code Style
- 代码注释使用中文
- 错误信息使用中文
- **禁止使用驼峰命名法**
- 变量命名使用下划线分隔（snake_case）
- 结构体类型以 `_t` 结尾，如 `pid_param_t`
- 声明的变量以 `s_` 开头，如 `s_motor_config`
- 宏定义全大写，如 `MAX_CURRENT_LIMIT`
- 枚举类型以 `_E` 结尾，如 `control_mode_E`
- 枚举变量以 `e_` 开头，如 `e_velocity_mode`
- 函数命名使用下划线分隔，如 `modbus_read_register()`

### Architecture Patterns
- MVC架构模式
- 以通信速度和解析速度优先进行设计
- 串口通信模块独立封装
- 参数管理模块与UI解耦

### Testing Strategy
- 单元测试覆盖核心通信模块
- 模拟串口测试

### Git Workflow
- 主分支: main
- 功能分支: feature/xxx
- 提交信息使用中文描述

## Domain Context

### AxDr电机控制板可控参数

#### 1. PID控制参数
| 参数类别 | 具体参数 | 说明 |
|---------|---------|------|
| 电流环PID | Kp_current, Ki_current, Kd_current | 电流闭环控制 |
| 速度环PID | Kp_velocity, Ki_velocity, Kd_velocity | 速度闭环控制 |
| 位置环PID | Kp_position, Ki_position, Kd_position | 位置闭环控制 |

#### 2. 电机参数
| 参数 | 说明 |
|------|------|
| pole_pairs | 电机极对数 |
| phase_resistance | 相电阻 (Ω) |
| phase_inductance | 相电感 (H) |
| torque_constant | 转矩常数 (Nm/A) |

#### 3. 限制参数
| 参数 | 说明 |
|------|------|
| current_limit | 电流限制 (A) |
| velocity_limit | 速度限制 (rad/s) |
| voltage_limit | 电压限制 (V) |

#### 4. 控制模式
| 模式 | 说明 |
|------|------|
| torque_mode | 力矩控制模式 |
| velocity_mode | 速度控制模式 |
| position_mode | 位置控制模式 |

#### 5. 编码器参数
| 参数 | 说明 |
|------|------|
| encoder_cpr | 编码器每转脉冲数 |
| encoder_offset | 编码器零点偏移 |

#### 6. 保护参数
| 参数 | 说明 |
|------|------|
| over_voltage_trip | 过压保护阈值 |
| under_voltage_trip | 欠压保护阈值 |
| over_temp_trip | 过温保护阈值 |

### Modbus寄存器地址映射
定义在 `src/params/param_manager.h` 中，主要地址段：
- 0x0000-0x001F: PID参数
- 0x0020-0x002F: 电机物理参数
- 0x0030-0x003F: 限制参数
- 0x0040-0x004F: 编码器参数
- 0x0050-0x005F: 保护参数
- 0x0060: 控制模式
- 0x0100-0x010F: 实时数据（只读）

### 通信协议
- 协议类型: Modbus RTU（手动实现）
- 设计原则: 通信速度和解析速度优先
- CRC校验: CRC-16/MODBUS

## Important Constraints
- 必须支持Windows和Linux平台
- 串口通信延迟需控制在合理范围内
- UI响应不能被串口通信阻塞
- 大文件（如字体）需运行时加载，避免编译内存不足

## External Dependencies
- Qt6 SerialPort模块


---

## FOC可视化学习工具 (qt6_for_visualization_foc)

### 项目概述
纯软件模拟的FOC算法可视化学习工具，脱离硬件平台，用于学习和理解FOC控制算法。

### 技术规格
- 仿真步长: 10μs（可选100μs）
- 界面: 双窗口多面板
- 电机模型: PMSM数学模型

### 界面设计

**窗口1 - FOC算法可视化**
- 坐标变换面板（Clark/Park矢量圆动画）
- SVPWM面板（六边形矢量图、扇区判断）
- 电机状态面板（转子动画、三相波形）

**窗口2 - 三环控制**
- 电流环（Id/Iq波形、PI参数调节）
- 速度环（速度波形、PI参数调节）
- 位置环（位置波形、PID参数调节）
- 各环路启用/禁用开关

### 交互功能
- 运行/暂停/单步执行
- 算法步骤高亮显示
- PID参数实时调节
- 负载扰动注入
- 目标值设定

### 里程碑计划

#### M1: 项目基础架构 [已完成] ✓
- [x] CMake项目配置
- [x] 双窗口框架搭建（垂直分割布局）
- [x] 基础面板布局（6个面板占位）
- [x] 类型定义（motor_state_t, motor_params_t等）

#### M2: 电机数学模型 [已完成] ✓
- [x] PMSM电机dq轴模型（欧拉法求解）
- [x] 仿真引擎（支持10μs/100μs步长）
- [x] 电磁转矩计算
- [x] 机械动态方程
- [x] 电机模型工厂（支持扩展）
- [ ] 电机参数配置界面

#### M3: 坐标变换模块 [已完成] ✓
- [x] Clark变换实现（abc→αβ）
- [x] Park变换实现（αβ→dq）
- [x] 逆Clark变换
- [x] 逆Park变换
- [x] Iα-Iβ矢量圆动画（vector_scope控件）
- [x] Id-Iq矢量显示

#### M4: SVPWM模块 [已完成] ✓
- [x] SVPWM算法实现（七段式）
- [x] 扇区判断算法
- [x] 基本矢量时间计算
- [x] 占空比计算
- [x] 六边形矢量图绘制（svpwm_hexagon控件）
- [x] PWM占空比进度条显示

#### M5: 三环控制模块 [已完成] ✓
- [x] PID控制器基类
- [x] 电流环PI控制器
- [x] 速度环PI控制器
- [x] 位置环PID控制器
- [x] 串级控制架构
- [x] 环路启用/禁用开关
- [x] 波形实时显示（waveform_view控件）
- [x] PID参数调节滑块

#### M6: 电机状态可视化 [已完成] ✓
- [x] 转子角度动画（rotor_animator控件）
- [x] 三相电流波形
- [x] 实时数据面板

#### M7: 交互与调试功能 [已完成] ✓
- [x] 运行/暂停/复位控制
- [x] 单步执行模式
- [x] 算法步骤高亮（step_indicator控件）
- [x] PID参数滑块调节
- [x] 电机类型选择器
- [x] 负载扰动注入（滑块+SpinBox）
- [x] 目标速度/位置设定UI控件
- [x] 仿真速度调节（1x/2x/5x/10x）

#### M8: 集成与优化 [已完成] ✓
- [x] 模块集成（仿真引擎连接UI面板）
- [x] 运行/暂停/复位控制
- [x] 单步执行功能
- [x] 电机类型切换（PMSM/BLDC）
- [x] 控制模式选择（FOC/六步换向）
- [x] 仿真性能优化（批量执行+60Hz刷新）
- [x] 内存使用优化（deque循环缓冲）
- [x] 界面响应优化（异步更新机制）

#### M9: BLDC模型与六步换向 [已完成] ✓
- [x] BLDC电机模型实现（梯形波反电动势）
- [x] 六步换向控制器框架
- [x] 电机类型选择UI
- [x] 控制模式选择UI
- [x] 六步换向仿真集成
- [x] 霍尔传感器状态模拟
- [x] 换向时序可视化（hall_indicator控件）

#### M10: 控制算法调优 [已完成] ✓
- [x] PMSM FOC控制参数整定（IMC内模控制法）
- [x] BLDC FOC控制参数整定（bldc_foc.json配置）
- [x] 六步换向控制参数整定（bldc_six_step.json配置）
- [x] 配置文件导入/导出功能
- [x] 电机参数配置面板
- [x] 恢复默认配置功能
- [ ] 参数自整定功能（可选，暂不实现）

#### M11: 代码质量优化 [已完成] ✓
- [x] calc_hall_state查表法优化（复杂度19→5）
- [x] waveform_view拆分为5个辅助函数
- [x] rotor_animator拆分为3个辅助函数
- [x] step_indicator查表法优化
- [x] 核心算法文件添加注释
- [x] 波形欠采样修复（每帧8次采样）

### 项目文件结构（规划）
```
qt6_for_visualization_foc/
├── CMakeLists.txt
├── Makefile
├── main.cpp
└── src/
    ├── core/               # 核心仿真引擎
    │   ├── foc_engine.h/cpp        # FOC仿真引擎
    │   ├── pmsm_model.h/cpp        # PMSM电机模型
    │   └── transforms.h/cpp        # 坐标变换
    ├── control/            # 控制算法
    │   ├── pid_controller.h/cpp    # PID控制器
    │   ├── current_loop.h/cpp      # 电流环
    │   ├── velocity_loop.h/cpp     # 速度环
    │   ├── position_loop.h/cpp     # 位置环
    │   └── svpwm.h/cpp             # SVPWM算法
    └── ui/                 # 用户界面
        ├── window_foc.h/cpp        # 窗口1-FOC可视化
        ├── window_control.h/cpp    # 窗口2-三环控制
        ├── panel_transform.h/cpp   # 坐标变换面板
        ├── panel_svpwm.h/cpp       # SVPWM面板
        ├── panel_motor.h/cpp       # 电机状态面板
        └── panel_loop.h/cpp        # 控制环面板
```
