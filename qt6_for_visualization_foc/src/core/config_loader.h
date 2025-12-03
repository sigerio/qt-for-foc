#ifndef CORE_CONFIG_LOADER_H
#define CORE_CONFIG_LOADER_H

#include "types.h"
#include <QString>

// 六步换向参数
struct six_step_params_t {
    double kp = 0.01;
    double ki = 0.1;
    double integral_max = 0.8;
};

// 配置加载器 - 从JSON文件加载电机和控制参数
struct config_data_t {
    motor_params_t motor;
    sim_config_t sim;
    pid_params_t current_pid;
    pid_params_t velocity_pid;
    pid_params_t position_pid;
    six_step_params_t six_step;
    control_target_t target;
    bool valid = false;
    bool has_six_step = false;
};

class config_loader {
public:
    static config_data_t load(const QString& path);
    static bool save(const QString& path, const config_data_t& cfg);
};

#endif // CORE_CONFIG_LOADER_H
