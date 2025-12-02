#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QDebug>
#include "ui/foc_visualization_window.h"
#include "ui/control_loop_window.h"
#include "ui/panels/coord_transform_panel.h"
#include "ui/panels/svpwm_panel.h"
#include "ui/panels/motor_state_panel.h"
#include "ui/panels/current_loop_panel.h"
#include "ui/panels/velocity_loop_panel.h"
#include "ui/panels/position_loop_panel.h"
#include "ui/widgets/step_indicator.h"
#include "core/sim_engine.h"
#include "core/motor_model_factory.h"
#include "control/loop_controller.h"

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
    } else {
        qDebug() << "字体加载失败，使用系统默认字体";
    }

    // 创建双窗口
    foc_visualization_window foc_win;
    control_loop_window ctrl_win;

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
    
    // 创建控制器并设置默认目标
    loop_controller ctrl;
    control_target_t target;
    target.vel_ref = 100.0;  // 默认速度目标
    ctrl.set_target(target);
    engine.set_loop_controller(&ctrl);
    
    // 配置仿真
    sim_config_t cfg;
    cfg.dt = 100e-6;  // 100us步长
    cfg.speed_ratio = 1.0;
    engine.set_config(cfg);
    
    // 连接仿真引擎信号到FOC可视化窗口面板
    QObject::connect(&engine, &sim_engine::state_updated, [&](const motor_state_t& state) {
        // 更新坐标变换面板
        foc_win.coord_panel()->update_alpha_beta(state.i_alpha, state.i_beta);
        foc_win.coord_panel()->update_dq(state.id, state.iq);
        foc_win.coord_panel()->update_abc(state.ia, state.ib, state.ic);
        
        // 更新SVPWM面板
        auto svpwm_out = engine.get_svpwm_output();
        foc_win.svpwm_panel_ptr()->update_voltage(state.ud, state.uq);
        foc_win.svpwm_panel_ptr()->update_sector(svpwm_out.sector);
        foc_win.svpwm_panel_ptr()->update_duty(svpwm_out.ta, svpwm_out.tb, svpwm_out.tc);
        
        // 更新电机状态面板
        foc_win.motor_panel()->update_angle(state.theta_e);
        foc_win.motor_panel()->update_phase_currents(state.ia, state.ib, state.ic);
        foc_win.motor_panel()->update_motor_state(state.omega_m, state.te, state.theta_e);
        
        // 更新控制环窗口面板（使用内部计算的参考值）
        ctrl_win.current_panel()->update_currents(ctrl.get_id_ref(), state.id, ctrl.get_iq_ref(), state.iq);
        ctrl_win.velocity_panel()->update_velocity(ctrl.get_vel_ref(), state.omega_m);
        ctrl_win.position_panel()->update_position(ctrl.get_target().pos_ref, state.theta_m);
    });
    
    // 连接运行/暂停/复位控制
    QObject::connect(&ctrl_win, &control_loop_window::run_state_changed, [&](bool running) {
        if (running) {
            engine.start();
        } else {
            engine.pause();
        }
    });
    
    QObject::connect(&ctrl_win, &control_loop_window::reset_requested, [&]() {
        engine.reset();
    });
    
    QObject::connect(&ctrl_win, &control_loop_window::step_requested, [&]() {
        engine.step();
    });
    
    // 连接步骤高亮信号
    QObject::connect(&engine, &sim_engine::foc_step_changed, 
                     foc_win.step_ind(), &step_indicator::set_current_step);
    
    // 电机类型切换
    QObject::connect(&ctrl_win, &control_loop_window::motor_type_changed, [&](int type) {
        engine.pause();
        motor_params_t new_params = params;  // 使用当前参数
        e_motor_type motor_type = (type == 0) ? e_motor_type::PMSM : e_motor_type::BLDC;
        auto new_motor = motor_model_factory::create(motor_type);
        if (new_motor) {
            new_motor->set_params(new_params);
            engine.set_motor_model(std::move(new_motor));
            engine.reset();
        }
    });

    // 设置窗口位置
    foc_win.move(100, 100);
    ctrl_win.move(920, 100);

    // 显示窗口
    foc_win.show();
    ctrl_win.show();

    return app.exec();
}
