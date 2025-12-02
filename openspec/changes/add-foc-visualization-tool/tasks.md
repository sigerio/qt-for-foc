# 任务清单

## M1. 项目基础架构
- [x] 1.1 创建CMakeLists.txt项目配置
- [x] 1.2 创建Makefile构建脚本
- [x] 1.3 创建双窗口应用框架
- [x] 1.4 创建src目录结构（core/control/ui）
- [x] 1.5 创建types.h公共数据结构

## M2. 电机模型框架
- [x] 2.1 定义i_motor_model接口
- [x] 2.2 实现motor_model_factory工厂类
- [x] 2.3 定义e_motor_type枚举（PMSM/BLDC/ACIM/SRM）
- [x] 2.4 实现PMSM电机dq轴数学模型
- [x] 2.5 实现仿真引擎sim_engine（支持10μs/100μs步长）
- [x] 2.6 实现电机参数配置（Rs, Ld, Lq, ψf, J, B, p）
- [x] 2.7 实现运动方程求解器

## M3. 坐标变换模块
- [x] 3.1 定义i_transform接口
- [x] 3.2 实现Clark变换（abc→αβ）
- [x] 3.3 实现Park变换（αβ→dq）
- [x] 3.4 实现逆Clark变换
- [x] 3.5 实现逆Park变换
- [ ] 3.6 实现Iα-Iβ矢量圆动画绘制（vector_scope控件）
- [ ] 3.7 实现坐标变换面板coord_transform_panel

## M4. SVPWM模块
- [x] 4.1 定义i_svpwm接口
- [x] 4.2 实现SVPWM算法核心
- [x] 4.3 实现扇区判断
- [x] 4.4 实现占空比计算（Ta, Tb, Tc）
- [ ] 4.5 实现六边形矢量图绘制
- [ ] 4.6 实现svpwm_panel面板UI

## M5. 三环控制模块
- [x] 5.1 定义i_pid_controller接口
- [x] 5.2 实现pid_controller通用PID控制器
- [x] 5.3 实现i_loop_controller接口
- [x] 5.4 实现loop_controller三环控制器
- [x] 5.5 实现电流环（Id/Iq PI控制）
- [x] 5.6 实现速度环（PI控制）
- [x] 5.7 实现位置环（PID控制）
- [x] 5.8 实现环路启用/禁用开关
- [ ] 5.9 实现waveform_view波形显示控件
- [ ] 5.10 实现current_loop_panel/velocity_loop_panel/position_loop_panel

## M6. 电机状态可视化
- [ ] 6.1 实现rotor_animator转子动画控件
- [ ] 6.2 实现三相电流波形绘制
- [ ] 6.3 实现实时数据显示面板
- [ ] 6.4 实现motor_state_panel面板UI

## M7. 交互与调试功能
- [ ] 7.1 定义i_sim_controller接口
- [ ] 7.2 实现sim_controller仿真控制器
- [ ] 7.3 实现运行/暂停/复位按钮
- [ ] 7.4 实现单步执行功能
- [ ] 7.5 实现算法步骤高亮显示
- [ ] 7.6 实现PID参数滑块调节
- [ ] 7.7 实现负载扰动注入
- [ ] 7.8 实现目标速度/位置设定
- [ ] 7.9 实现仿真速度调节（1x/2x/5x/10x）
- [ ] 7.10 实现motor_type_selector电机类型选择器

## M8. 集成与优化
- [ ] 8.1 集成所有模块到双窗口框架
- [ ] 8.2 实现foc_visualization_window主窗口
- [ ] 8.3 实现control_loop_window控制窗口
- [ ] 8.4 仿真性能优化
- [ ] 8.5 内存使用优化
- [ ] 8.6 界面响应优化

## M9. BLDC模型扩展（可选）
- [ ] 9.1 定义bldc_params_t扩展参数结构
- [ ] 9.2 实现bldc_model电机模型
- [ ] 9.3 实现六步换相算法
- [ ] 9.4 实现霍尔传感器状态模拟
- [ ] 9.5 在motor_model_factory注册BLDC类型
- [ ] 9.6 扩展UI支持BLDC参数配置
