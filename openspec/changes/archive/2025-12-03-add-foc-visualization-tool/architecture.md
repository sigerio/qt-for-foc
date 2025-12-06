# FOC可视化工具软件架构设计

## 1. 整体架构

```
┌─────────────────────────────────────────────────────────────────┐
│                        Application Layer                         │
│  ┌─────────────────────────┐  ┌─────────────────────────────┐   │
│  │   FOC算法可视化窗口      │  │     三环控制窗口             │   │
│  │  (foc_visualization_wnd)│  │  (control_loop_window)      │   │
│  └─────────────────────────┘  └─────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                      Presentation Layer (ui/)                    │
│  ┌───────────────────┐ ┌──────────────┐ ┌──────────────────┐    │
│  │coord_transform_panel│ │ svpwm_panel │ │motor_state_panel │    │
│  └───────────────────┘ └──────────────┘ └──────────────────┘    │
│  ┌───────────────────┐ ┌──────────────────┐ ┌────────────────┐  │
│  │current_loop_panel │ │velocity_loop_panel│ │position_loop_panel│
│  └───────────────────┘ └──────────────────┘ └────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                      Business Layer (control/)                   │
│  ┌──────────────────┐  ┌──────────────────┐                     │
│  │  sim_controller  │  │  loop_controller │                     │
│  │  (仿真控制器)     │  │  (环路控制器)     │                     │
│  └──────────────────┘  └──────────────────┘                     │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                        Core Layer (core/)                        │
│  ┌────────────┐ ┌────────────┐ ┌────────────┐ ┌──────────────┐ │
│  │ pmsm_model │ │ transform  │ │   svpwm    │ │pid_controller│ │
│  │ (电机模型)  │ │ (坐标变换)  │ │  (SVPWM)   │ │  (PID控制)   │ │
│  └────────────┘ └────────────┘ └────────────┘ └──────────────┘ │
│  ┌────────────┐ ┌────────────┐                                  │
│  │ sim_engine │ │ data_buffer│                                  │
│  │ (仿真引擎)  │ │ (数据缓冲)  │                                  │
│  └────────────┘ └────────────┘                                  │
└─────────────────────────────────────────────────────────────────┘
```

## 2. 目录结构

```
qt6_for_visualization_foc/
├── CMakeLists.txt
├── Makefile
├── main.cpp
├── resources/
│   └── resources.qrc
└── src/
    ├── core/                         # 核心算法层
    │   ├── types.h                   # 公共数据结构
    │   ├── pmsm_model.h/cpp          # PMSM电机数学模型
    │   ├── transform.h/cpp           # 坐标变换(Clark/Park)
    │   ├── svpwm.h/cpp               # SVPWM算法
    │   ├── pid_controller.h/cpp      # PID控制器
    │   ├── sim_engine.h/cpp          # 仿真引擎
    │   └── data_buffer.h/cpp         # 环形数据缓冲
    ├── control/                      # 业务控制层
    │   ├── sim_controller.h/cpp      # 仿真流程控制
    │   └── loop_controller.h/cpp     # 三环控制器
    └── ui/                           # 界面层
        ├── foc_visualization_window.h/cpp  # FOC可视化主窗口
        ├── control_loop_window.h/cpp       # 三环控制窗口
        ├── panels/
        │   ├── coord_transform_panel.h/cpp # 坐标变换面板
        │   ├── svpwm_panel.h/cpp           # SVPWM面板
        │   ├── motor_state_panel.h/cpp     # 电机状态面板
        │   ├── current_loop_panel.h/cpp    # 电流环面板
        │   ├── velocity_loop_panel.h/cpp   # 速度环面板
        │   └── position_loop_panel.h/cpp   # 位置环面板
        └── widgets/
            ├── vector_scope.h/cpp    # 矢量示波器控件
            ├── waveform_view.h/cpp   # 波形显示控件
            └── rotor_animator.h/cpp  # 转子动画控件
```


## 3. 核心类设计

### 3.1 数据结构 (core/types.h)

```cpp
// 电机参数结构
struct motor_params_t {
    double Rs;      // 定子电阻 (Ω)
    double Ld;      // d轴电感 (H)
    double Lq;      // q轴电感 (H)
    double psi_f;   // 永磁体磁链 (Wb)
    double J;       // 转动惯量 (kg·m²)
    double B;       // 阻尼系数 (N·m·s/rad)
    uint32_t p;     // 极对数
};

// 电机状态结构
struct motor_state_t {
    double theta_e;     // 电角度 (rad)
    double omega_e;     // 电角速度 (rad/s)
    double omega_m;     // 机械角速度 (rad/s)
    double id, iq;      // dq轴电流 (A)
    double ud, uq;      // dq轴电压 (V)
    double ia, ib, ic;  // 三相电流 (A)
    double i_alpha, i_beta;  // αβ电流
    double Te;          // 电磁转矩 (N·m)
    double TL;          // 负载转矩 (N·m)
};

// 控制目标结构
struct control_target_t {
    double id_ref;      // d轴电流目标
    double iq_ref;      // q轴电流目标
    double vel_ref;     // 速度目标 (rad/s)
    double pos_ref;     // 位置目标 (rad)
};

// PID参数结构
struct pid_params_t {
    double kp, ki, kd;
    double out_max, out_min;
    double integral_max;
};

// 仿真配置
struct sim_config_t {
    double dt;          // 仿真步长 (s): 10e-6 或 100e-6
    double speed_ratio; // 仿真速度倍率: 1x/2x/5x/10x
    bool running;
    bool single_step;
};
```

### 3.2 核心接口定义

```cpp
// ===== core/i_motor_model.h =====
class i_motor_model {
public:
    virtual ~i_motor_model() = default;
    virtual void set_params(const motor_params_t& params) = 0;
    virtual void set_voltage(double ud, double uq) = 0;
    virtual void set_load_torque(double TL) = 0;
    virtual void step(double dt) = 0;
    virtual motor_state_t get_state() const = 0;
    virtual void reset() = 0;
};

// ===== core/i_transform.h =====
class i_transform {
public:
    virtual ~i_transform() = default;
    // Clark: abc -> αβ
    virtual void clark(double ia, double ib, double ic,
                       double& i_alpha, double& i_beta) = 0;
    // Park: αβ -> dq
    virtual void park(double i_alpha, double i_beta, double theta,
                      double& id, double& iq) = 0;
    // 逆变换
    virtual void inv_clark(double u_alpha, double u_beta,
                           double& ua, double& ub, double& uc) = 0;
    virtual void inv_park(double ud, double uq, double theta,
                          double& u_alpha, double& u_beta) = 0;
};

// ===== core/i_svpwm.h =====
class i_svpwm {
public:
    virtual ~i_svpwm() = default;
    virtual void calc(double u_alpha, double u_beta, double udc,
                      double& Ta, double& Tb, double& Tc) = 0;
    virtual int get_sector() const = 0;
    virtual void get_vector(double& mag, double& angle) const = 0;
};

// ===== core/i_pid_controller.h =====
class i_pid_controller {
public:
    virtual ~i_pid_controller() = default;
    virtual void set_params(const pid_params_t& params) = 0;
    virtual double calc(double target, double feedback, double dt) = 0;
    virtual void reset() = 0;
    virtual pid_params_t get_params() const = 0;
};
```


### 3.3 控制层接口

```cpp
// ===== control/i_sim_controller.h =====
class i_sim_controller : public QObject {
    Q_OBJECT
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void step() = 0;      // 单步执行
    virtual void reset() = 0;
    virtual void set_config(const sim_config_t& cfg) = 0;
    virtual sim_config_t get_config() const = 0;

signals:
    void state_updated(const motor_state_t& state);
    void step_completed(int step_index, const QString& step_name);
};

// ===== control/i_loop_controller.h =====
class i_loop_controller {
public:
    virtual void set_current_loop_enabled(bool en) = 0;
    virtual void set_velocity_loop_enabled(bool en) = 0;
    virtual void set_position_loop_enabled(bool en) = 0;
    
    virtual void set_current_pid(const pid_params_t& id_pid,
                                  const pid_params_t& iq_pid) = 0;
    virtual void set_velocity_pid(const pid_params_t& pid) = 0;
    virtual void set_position_pid(const pid_params_t& pid) = 0;
    
    virtual void set_target(const control_target_t& target) = 0;
    virtual control_target_t calc(const motor_state_t& state, double dt) = 0;
};
```

## 4. 数据流设计

```
┌─────────────────────────────────────────────────────────────────────┐
│                         仿真主循环 (sim_engine)                      │
└─────────────────────────────────────────────────────────────────────┘
                                   │
           ┌───────────────────────┼───────────────────────┐
           ▼                       ▼                       ▼
    ┌─────────────┐        ┌─────────────┐        ┌─────────────┐
    │ 位置环      │───────▶│ 速度环      │───────▶│ 电流环      │
    │ pos_ref     │ vel_ref│ vel_ref     │ iq_ref │ id/iq_ref   │
    │  → vel_ref  │        │  → iq_ref   │        │  → ud/uq    │
    └─────────────┘        └─────────────┘        └─────────────┘
                                                         │
                                                         ▼
                                                  ┌─────────────┐
                                                  │ 逆Park变换   │
                                                  │ ud,uq→uα,uβ │
                                                  └─────────────┘
                                                         │
                                                         ▼
                                                  ┌─────────────┐
                                                  │ SVPWM       │
                                                  │ uα,uβ→Ta,Tb,Tc│
                                                  └─────────────┘
                                                         │
                                                         ▼
                                                  ┌─────────────┐
                                                  │ pmsm_model  │
                                                  │ 电压方程求解 │
                                                  │ 运动方程求解 │
                                                  └─────────────┘
                                                         │
           ┌─────────────────────────────────────────────┘
           ▼
    ┌─────────────┐        ┌─────────────┐        ┌─────────────┐
    │ ia,ib,ic    │───────▶│ Clark变换   │───────▶│ Park变换    │
    │ (三相电流)   │        │ →iα,iβ      │        │ →id,iq      │
    └─────────────┘        └─────────────┘        └─────────────┘
                                                         │
                                                         ▼
                                                  ┌─────────────┐
                                                  │ 反馈到控制环 │
                                                  └─────────────┘
```


## 5. Qt信号槽连接

```
sim_controller
    │
    ├──[state_updated(motor_state_t)]──▶ foc_visualization_window
    │                                       ├── coord_transform_panel::update_display()
    │                                       ├── svpwm_panel::update_display()
    │                                       └── motor_state_panel::update_display()
    │
    ├──[state_updated(motor_state_t)]──▶ control_loop_window
    │                                       ├── current_loop_panel::update_waveform()
    │                                       ├── velocity_loop_panel::update_waveform()
    │                                       └── position_loop_panel::update_waveform()
    │
    └──[step_completed(int, QString)]──▶ foc_visualization_window
                                            └── highlight_current_step()

UI Controls
    │
    ├──[运行/暂停按钮]──▶ sim_controller::start()/pause()
    ├──[单步按钮]──────▶ sim_controller::step()
    ├──[复位按钮]──────▶ sim_controller::reset()
    ├──[PID滑块]───────▶ loop_controller::set_*_pid()
    ├──[目标设定]──────▶ loop_controller::set_target()
    └──[负载扰动]──────▶ pmsm_model::set_load_torque()
```

## 6. 线程模型

```
┌────────────────────────┐     ┌────────────────────┐
│    主线程(UI)           │     │    仿真线程         │
│                        │     │                    │
│ foc_visualization_window│◀────│ sim_engine         │
│ control_loop_window    │信号  │  └─ 10μs/100μs循环  │
│ 各panel                │     │                    │
└────────────────────────┘     └────────────────────┘
        │                           ▲
        │ 用户操作                   │ 控制命令
        └───────────────────────────┘
```

**线程安全策略:**
- 仿真数据通过Qt信号槽跨线程传递（自动排队）
- motor_state_t 为值类型，拷贝传递
- 控制命令通过线程安全队列发送

## 7. 设计原则

| 原则 | 应用 |
|------|------|
| **单一职责** | 每个类只负责一个功能（如pmsm_model只负责电机建模） |
| **开闭原则** | 通过接口扩展，核心算法可替换（如不同电机模型） |
| **依赖倒置** | 上层依赖接口(i_motor_model)，不依赖具体实现 |
| **接口隔离** | 细粒度接口（i_transform、i_svpwm分离） |

## 8. 文件依赖关系

```
main.cpp
  └── ui/foc_visualization_window.h
  └── ui/control_loop_window.h
        └── control/sim_controller.h
        └── control/loop_controller.h
              └── core/pmsm_model.h
              └── core/transform.h
              └── core/svpwm.h
              └── core/pid_controller.h
              └── core/sim_engine.h
                    └── core/types.h
```

## 9. 命名规范

| 类型 | 命名规则 | 示例 |
|------|---------|------|
| 文件名 | 小写下划线 | `pmsm_model.cpp` |
| 类名 | 小写下划线 | `pmsm_model` |
| 接口类 | i_前缀+小写下划线 | `i_motor_model` |
| 结构体 | 小写下划线+_t后缀 | `motor_state_t` |
| 成员变量 | 小写下划线+m_前缀 | `m_theta_e` |
| 方法名 | 小写下划线 | `get_state()` |
| 信号 | 小写下划线 | `state_updated` |
| 槽函数 | 小写下划线 | `on_btn_start_clicked` |


## 10. 电机模型扩展设计

为支持多种电机模型（PMSM、BLDC等），采用工厂模式+策略模式设计：

### 10.1 电机类型枚举

```cpp
// core/types.h
enum class e_motor_type {
    PMSM,       // 永磁同步电机
    BLDC,       // 无刷直流电机
    ACIM,       // 异步感应电机（预留）
    SRM         // 开关磁阻电机（预留）
};
```

### 10.2 电机模型工厂

```cpp
// core/motor_model_factory.h
class motor_model_factory {
public:
    static std::unique_ptr<i_motor_model> create(e_motor_type type);
    static std::vector<e_motor_type> get_supported_types();
    static QString get_type_name(e_motor_type type);
};

// core/motor_model_factory.cpp
std::unique_ptr<i_motor_model> motor_model_factory::create(e_motor_type type) {
    switch (type) {
        case e_motor_type::PMSM:
            return std::make_unique<pmsm_model>();
        case e_motor_type::BLDC:
            return std::make_unique<bldc_model>();
        default:
            return nullptr;
    }
}
```

### 10.3 BLDC模型扩展参数

```cpp
// core/bldc_model.h
struct bldc_params_t : motor_params_t {
    double ke;          // 反电动势常数 (V/rad/s)
    double kt;          // 转矩常数 (N·m/A)
    double hall_offset; // 霍尔传感器偏移角 (rad)
};

class bldc_model : public i_motor_model {
public:
    void set_params(const motor_params_t& params) override;
    void set_bldc_params(const bldc_params_t& params);
    // ... 其他接口实现
private:
    bldc_params_t m_params;
    int m_hall_state;       // 霍尔状态(1-6)
    double m_bemf[3];       // 三相反电动势
};
```

### 10.4 目录结构扩展

```
src/core/
├── types.h
├── i_motor_model.h           # 电机模型接口
├── motor_model_factory.h/cpp # 电机模型工厂
├── pmsm_model.h/cpp          # PMSM实现
├── bldc_model.h/cpp          # BLDC实现（扩展）
└── ...
```

### 10.5 UI层电机选择

```cpp
// ui/motor_type_selector.h
class motor_type_selector : public QWidget {
    Q_OBJECT
public:
    void populate_types();      // 填充支持的电机类型
    e_motor_type get_selected_type() const;

signals:
    void type_changed(e_motor_type type);

private:
    QComboBox* m_combo_type;
};
```

### 10.6 电机模型切换流程

```
用户选择电机类型
       │
       ▼
motor_type_selector::type_changed(e_motor_type)
       │
       ▼
sim_controller::set_motor_type(e_motor_type type)
       │
       ▼
motor_model_factory::create(type)
       │
       ▼
更新UI参数面板（根据电机类型显示不同参数）
```
