#ifndef CORE_MOTOR_MODEL_FACTORY_H
#define CORE_MOTOR_MODEL_FACTORY_H

#include "types.h"
#include "i_motor_model.h"
#include <memory>
#include <vector>
#include <QString>

// 电机模型工厂
class motor_model_factory {
public:
    // 根据电机类型创建模型实例
    static std::unique_ptr<i_motor_model> create(e_motor_type type);

    // 获取支持的电机类型列表
    static std::vector<e_motor_type> get_supported_types();

    // 获取电机类型名称
    static QString get_type_name(e_motor_type type);
};

#endif // CORE_MOTOR_MODEL_FACTORY_H
