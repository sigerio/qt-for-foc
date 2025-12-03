#ifndef CORE_SIM_ENGINE_H
#define CORE_SIM_ENGINE_H

#include "types.h"
#include "i_motor_model.h"
#include "transform.h"
#include "svpwm.h"
#include "control/six_step_controller.h"
#include <QObject>
#include <QTimer>
#include <memory>

class loop_controller;

// 仿真引擎 - 负责运行仿真主循环
class sim_engine : public QObject {
    Q_OBJECT

public:
    explicit sim_engine(QObject* parent = nullptr);
    ~sim_engine() override;

    // 配置
    void set_config(const sim_config_t& cfg);
    sim_config_t get_config() const { return m_config; }

    // 设置电机模型
    void set_motor_model(std::unique_ptr<i_motor_model> model);
    i_motor_model* get_motor_model() { return m_motor.get(); }

    // 设置控制器
    void set_loop_controller(loop_controller* ctrl) { m_loop_ctrl = ctrl; }
    void set_six_step_controller(six_step_controller* ctrl) { m_six_step_ctrl = ctrl; }

    // 设置控制模式
    void set_control_mode(e_control_mode mode) { m_control_mode = mode; }
    e_control_mode get_control_mode() const { return m_control_mode; }

    // 获取SVPWM输出
    svpwm_output_t get_svpwm_output() const { return m_svpwm_out; }

    // 获取霍尔状态
    hall_state_t get_hall_state() const { return m_hall_state; }
    
    // 设置负载转矩
    void set_load_torque(double tl) { m_load_torque = tl; }
    double get_load_torque() const { return m_load_torque; }

public slots:
    void start();
    void stop();
    void pause();
    void step();
    void reset();

signals:
    void state_updated(const motor_state_t& state);
    void step_completed(int step_index, const QString& step_name);
    // FOC算法步骤变化信号（用于步骤高亮）
    void foc_step_changed(e_foc_step step);
    // 霍尔状态变化信号
    void hall_state_changed(const hall_state_t& hall);

private slots:
    void run_loop();

private:
    void execute_one_step();
    void execute_foc_step(motor_state_t& state, bool detailed);
    void execute_six_step(motor_state_t& state, bool detailed);
    void calc_hall_state(double theta_e);

    sim_config_t m_config;
    std::unique_ptr<i_motor_model> m_motor;
    loop_controller* m_loop_ctrl = nullptr;
    six_step_controller* m_six_step_ctrl = nullptr;
    transform m_transform;
    svpwm m_svpwm;
    svpwm_output_t m_svpwm_out;
    hall_state_t m_hall_state;
    e_control_mode m_control_mode = e_control_mode::FOC;

    QTimer* m_timer = nullptr;
    int m_step_index = 0;
    double m_udc = 24.0;        // 直流母线电压
    double m_load_torque = 0.2; // 负载转矩
};

#endif // CORE_SIM_ENGINE_H
