#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include "ui/foc_visualization_window.h"
#include "ui/waveform_window.h"
#include "ui/algorithm_explanation_window.h"
#include "ui/panels/control_loop_explanation_panel.h"
#include "ui/panels/pid_config_panel.h"
#include "ui/panels/control_toolbar_panel.h"
#include "ui/widgets/cascade_widget.h"
#include "ui/panels/coord_transform_panel.h"
#include "ui/panels/svpwm_panel.h"
#include "ui/panels/motor_state_panel.h"
#include "ui/widgets/step_indicator.h"
#include "ui/widgets/hall_indicator.h"
#include "ui/panels/motor_params_panel.h"
#include "core/sim_engine.h"
#include "core/motor_model_factory.h"
#include "core/config_loader.h"
#include "control/loop_controller.h"
#include "control/six_step_controller.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("FOC可视化学习工具");

    // 从文件系统加载阿里普惠体
    QString font_path = QApplication::applicationDirPath() + "/../fonts/AlibabaPuHuiTi-Regular.ttf";
    int font_id = QFontDatabase::addApplicationFont(font_path);
    if (font_id != -1) {
        QStringList font_families = QFontDatabase::applicationFontFamilies(font_id);
        if (!font_families.isEmpty()) {
            QFont font(font_families.first(), 10);
            app.setFont(font);
        }
    }

    // 创建三窗口
    foc_visualization_window foc_win;      // 窗口1：可视化+参数配置
    waveform_window wave_win;              // 窗口2：波形显示
    algorithm_explanation_window algo_win; // 窗口3：算法说明（选项卡形式）
    
    // 设置算法说明窗口和波形窗口引用（用于开关控制）
    foc_win.set_algo_window(&algo_win);
    foc_win.set_wave_window(&wave_win);

    // 创建仿真引擎
    sim_engine engine;
    
    // 创建PMSM电机模型并设置参数（中型驱动电机500W级）
    motor_params_t params;
    params.rs = 0.3;          // 定子电阻 0.3Ω
    params.ld = 0.001;        // d轴电感 1mH
    params.lq = 0.001;        // q轴电感 1mH
    params.psi_f = 0.15;      // 永磁磁链 0.15Wb
    params.j = 0.001;         // 转动惯量 0.001 kg·m²
    params.b = 0.0001;        // 阻尼系数
    params.pole_pairs = 4;    // 4极对
    auto motor = motor_model_factory::create(e_motor_type::PMSM);
    if (motor) {
        motor->set_params(params);
    }
    engine.set_motor_model(std::move(motor));
    
    // 创建FOC控制器并设置默认目标
    loop_controller ctrl;
    control_target_t target;
    target.vel_ref = 100.0;  // 默认速度目标
    ctrl.set_target(target);
    engine.set_loop_controller(&ctrl);
    
    // 创建六步换向控制器
    six_step_controller six_step_ctrl;
    engine.set_six_step_controller(&six_step_ctrl);
    
    // 配置仿真
    sim_config_t cfg;
    cfg.dt = 100e-6;  // 100us步长
    cfg.speed_ratio = 1.0;
    engine.set_config(cfg);
    
    // 连接仿真引擎信号到窗口更新
    QObject::connect(&engine, &sim_engine::state_updated, [&](const motor_state_t& state) {
        // ========== 更新窗口1（可视化面板）==========
        foc_win.coord_panel()->update_alpha_beta(state.i_alpha, state.i_beta);
        foc_win.coord_panel()->update_dq(state.id, state.iq);
        foc_win.coord_panel()->update_abc(state.ia, state.ib, state.ic);
        
        auto svpwm_out = engine.get_svpwm_output();
        foc_win.svpwm_panel_ptr()->update_voltage(state.u_alpha, state.u_beta);
        foc_win.svpwm_panel_ptr()->update_sector(svpwm_out.sector);
        foc_win.svpwm_panel_ptr()->update_duty(svpwm_out.ta, svpwm_out.tb, svpwm_out.tc);
        
        foc_win.motor_panel()->update_angle(state.theta_e);
        foc_win.motor_panel()->update_motor_state(state.omega_m, state.te, state.theta_e);
        
        // ========== 更新窗口2（波形显示）==========
        // 电流波形
        wave_win.update_currents(state.ia, state.ib, state.ic,
                                 state.i_alpha, state.i_beta,
                                 state.id, state.iq,
                                 ctrl.get_id_ref(), ctrl.get_iq_ref());
        // 电压波形（使用控制器输出的dq电压和坐标变换后的电压）
        wave_win.update_voltages(state.ud, state.uq,
                                 state.u_alpha, state.u_beta,
                                 state.ua, state.ub, state.uc);
        // PWM占空比
        wave_win.update_pwm(svpwm_out.ta, svpwm_out.tb, svpwm_out.tc);
        // 速度波形
        wave_win.update_velocity(ctrl.get_vel_ref(), state.omega_m);
        // 位置波形
        wave_win.update_position(ctrl.get_target().pos_ref, state.theta_m);
    });
    
    // ========== 控制工具栏信号连接（从窗口1发出）==========
    auto* toolbar = foc_win.toolbar_panel();
    
    // 运行/暂停/复位控制
    QObject::connect(toolbar, &control_toolbar_panel::run_state_changed, [&](bool running) {
        if (running) {
            engine.start();
        } else {
            engine.pause();
        }
    });
    
    QObject::connect(toolbar, &control_toolbar_panel::reset_requested, [&]() {
        engine.reset();
    });
    
    QObject::connect(toolbar, &control_toolbar_panel::step_requested, [&]() {
        engine.step();
    });
    
    // 连接步骤高亮信号
    QObject::connect(&engine, &sim_engine::foc_step_changed, 
                     foc_win.step_ind(), &step_indicator::set_current_step);
    
    // 电机类型切换
    QObject::connect(toolbar, &control_toolbar_panel::motor_type_changed, [&](int type) {
        engine.pause();
        motor_params_t new_params = params;
        e_motor_type motor_type = (type == 0) ? e_motor_type::PMSM : e_motor_type::BLDC;
        auto new_motor = motor_model_factory::create(motor_type);
        if (new_motor) {
            new_motor->set_params(new_params);
            engine.set_motor_model(std::move(new_motor));
            engine.reset();
        }
    });
    
    // 控制模式切换（FOC/六步换向）
    QObject::connect(toolbar, &control_toolbar_panel::control_mode_changed, [&](int mode) {
        engine.pause();
        e_control_mode ctrl_mode = (mode == 0) ? e_control_mode::FOC : e_control_mode::SIX_STEP;
        engine.set_control_mode(ctrl_mode);
        foc_win.set_control_mode(ctrl_mode);
        engine.reset();
    });
    
    // 连接霍尔状态变化信号
    QObject::connect(&engine, &sim_engine::hall_state_changed, 
                     foc_win.hall_ind(), &hall_indicator::set_hall_state);
    
    // 仿真速度调节
    QObject::connect(toolbar, &control_toolbar_panel::speed_ratio_changed, [&](double ratio) {
        sim_config_t cfg_new = engine.get_config();
        cfg_new.speed_ratio = ratio;
        engine.set_config(cfg_new);
    });
    
    // 配置文件加载
    QObject::connect(toolbar, &control_toolbar_panel::config_load_requested, [&](const QString& path) {
        config_data_t cfg = config_loader::load(path);
        if (cfg.valid) {
            engine.pause();
            params = cfg.motor;
            if (auto* m = engine.get_motor_model()) {
                m->set_params(params);
            }
            foc_win.params_panel()->set_params(params);
            engine.set_config(cfg.sim);
            ctrl.set_current_pid(cfg.current_pid, cfg.current_pid);
            ctrl.set_velocity_pid(cfg.velocity_pid);
            ctrl.set_position_pid(cfg.position_pid);
            if (cfg.has_six_step) {
                six_step_ctrl.set_speed_pid(cfg.six_step.kp, cfg.six_step.ki);
            }
            ctrl.set_target(cfg.target);
            engine.reset();
        }
    });
    
    // 恢复默认配置
    QObject::connect(toolbar, &control_toolbar_panel::config_reset_requested, [&]() {
        engine.pause();
        params.rs = 0.3;
        params.ld = 0.001;
        params.lq = 0.001;
        params.psi_f = 0.15;
        params.j = 0.001;
        params.b = 0.0001;
        params.pole_pairs = 4;
        if (auto* m = engine.get_motor_model()) {
            m->set_params(params);
        }
        foc_win.params_panel()->set_params(params);
        sim_config_t default_cfg;
        default_cfg.dt = 100e-6;
        default_cfg.speed_ratio = 1.0;
        engine.set_config(default_cfg);
        ctrl.reset_to_default();
        engine.reset();
    });
    
    // ========== PID参数信号连接（从窗口1发出）==========
    auto* pid_panel = foc_win.pid_panel();
    
    // 电流环PID
    QObject::connect(pid_panel, &pid_config_panel::current_pid_changed, [&](double kp, double ki) {
        ctrl.set_current_pid({kp, ki, 0}, {kp, ki, 0});
    });
    
    // 速度环PID
    QObject::connect(pid_panel, &pid_config_panel::velocity_pid_changed, [&](double kp, double ki) {
        ctrl.set_velocity_pid({kp, ki, 0});
    });
    
    // 速度目标
    QObject::connect(pid_panel, &pid_config_panel::velocity_target_changed, [&](double vel) {
        control_target_t t = ctrl.get_target();
        t.vel_ref = vel;
        ctrl.set_target(t);
    });
    
    // 位置环PID
    QObject::connect(pid_panel, &pid_config_panel::position_pid_changed, [&](double kp, double ki, double kd) {
        ctrl.set_position_pid({kp, ki, kd});
    });
    
    // 位置目标
    QObject::connect(pid_panel, &pid_config_panel::position_target_changed, [&](double pos) {
        control_target_t t = ctrl.get_target();
        t.pos_ref = pos;
        ctrl.set_target(t);
    });
    
    // 环路使能变化触发框图配置加载
    if (auto* cascade = algo_win.control_panel()->get_cascade_widget()) {
        QObject::connect(pid_panel, &pid_config_panel::loop_preset_changed,
                         cascade, &cascade_widget::load_preset);
    }
    
    // ========== 其他信号连接 ==========
    // 负载扰动注入
    QObject::connect(foc_win.motor_panel(), &motor_state_panel::load_torque_changed, [&](double tl) {
        engine.set_load_torque(tl);
    });
    
    // 电机参数变化
    QObject::connect(foc_win.params_panel(), &motor_params_panel::params_changed, [&](const motor_params_t& p) {
        params = p;
        if (auto* m = engine.get_motor_model()) {
            m->set_params(params);
        }
    });

    // 设置窗口位置
    foc_win.move(50, 50);
    wave_win.move(1300, 50);
    algo_win.move(50, 800);

    // 显示窗口
    foc_win.show();
    wave_win.show();
    // 窗口3默认隐藏，通过按钮开启
    // algo_win.show();

    return app.exec();
}
