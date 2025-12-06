# Project Context

## Purpose
纯软件模拟的FOC算法可视化学习工具，脱离硬件平台，用于学习和理解FOC控制算法。

## Tech Stack
- Qt6 (Core, Gui, Widgets)
- C++17
- CMake 3.16+

## Technical Specifications
- 仿真步长: 10μs（可选100μs）
- 界面: 三窗口多面板
- 电机模型: PMSM / BLDC

## Interface Design

**窗口1 - FOC算法可视化**
- 坐标变换面板（Clark/Park矢量圆动画）
- SVPWM面板（六边形矢量图、扇区判断）
- 电机状态面板（转子动画、三相波形）

**窗口2 - 三环控制**
- 电流环（Id/Iq波形、PI参数调节）
- 速度环（速度波形、PI参数调节）
- 位置环（位置波形、PID参数调节）
- 各环路启用/禁用开关

**窗口3 - 算法说明**
- 电机模型说明面板
- 坐标变换说明面板
- SVPWM说明面板
- 三环控制说明面板
- 可交互控制框图编辑器

## Interactive Features
- 运行/暂停/单步执行
- 算法步骤高亮显示
- PID参数实时调节
- 负载扰动注入
- 目标值设定
- 仿真速度调节（1x/2x/5x/10x）
- 配置文件导入/导出



## Project Conventions

### Code Style
- 代码注释使用中文
- 错误信息使用中文
- **禁止使用驼峰命名法**
- 变量命名：snake_case
- 结构体类型：`_t` 结尾（如 `motor_state_t`）
- 声明变量：`s_` 开头
- 宏定义：全大写
- 枚举类型：`_E` 结尾
- 枚举变量：`e_` 开头
- 函数命名：下划线分隔

### Architecture Patterns
- MVC架构模式
- 仿真引擎与UI解耦
- 控制算法模块化设计

### Git Workflow
- 主分支: main
- 功能分支: feature/xxx
- 提交信息使用中文描述

## Project File Structure
```
qt6_for_visualization_foc/
├── CMakeLists.txt
├── Makefile
├── main.cpp
├── config/                 # 配置文件
├── fonts/                  # 字体资源
├── resources/              # 其他资源
└── src/
    ├── core/               # 核心仿真引擎
    │   ├── types.h                 # 类型定义
    │   ├── sim_engine.*            # 仿真引擎
    │   ├── pmsm_model.*            # PMSM电机模型
    │   ├── bldc_model.*            # BLDC电机模型
    │   ├── motor_model_factory.*   # 电机模型工厂
    │   ├── transform.*             # 坐标变换
    │   ├── svpwm.*                 # SVPWM算法
    │   ├── pid_controller.*        # PID控制器
    │   ├── config_loader.*         # 配置加载
    │   └── data_buffer.*           # 数据缓冲
    ├── control/            # 控制算法
    │   ├── loop_controller.*       # 环路控制器
    │   ├── sim_controller.*        # 仿真控制器
    │   └── six_step_controller.*   # 六步换向控制器
    └── ui/                 # 用户界面
        ├── foc_visualization_window.*    # 窗口1-FOC可视化
        ├── control_loop_window.*         # 窗口2-三环控制
        ├── algorithm_explanation_window.* # 窗口3-算法说明
        ├── panels/         # 面板组件
        │   ├── coord_transform_panel.*
        │   ├── svpwm_panel.*
        │   ├── motor_state_panel.*
        │   ├── motor_params_panel.*
        │   ├── current_loop_panel.*
        │   ├── velocity_loop_panel.*
        │   ├── position_loop_panel.*
        │   └── *_explanation_panel.*     # 说明面板
        └── widgets/        # 自定义控件
            ├── vector_scope.*      # 矢量圆
            ├── waveform_view.*     # 波形显示
            ├── rotor_animator.*    # 转子动画
            ├── step_indicator.*    # 步骤指示器
            ├── hall_indicator.*    # 霍尔指示器
            ├── cascade_widget.*    # 框图编辑器
            └── *_item.*            # 框图元素
```

## Important Constraints
- 必须支持Windows和Linux平台
- 仿真性能需满足实时性要求
- UI响应不能被仿真计算阻塞

## External Dependencies
- Qt6 Core, Gui, Widgets
