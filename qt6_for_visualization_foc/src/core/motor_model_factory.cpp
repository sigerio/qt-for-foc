#include "motor_model_factory.h"
#include "pmsm_model.h"
#include "bldc_model.h"

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

std::vector<e_motor_type> motor_model_factory::get_supported_types() {
    return { e_motor_type::PMSM, e_motor_type::BLDC };
}

QString motor_model_factory::get_type_name(e_motor_type type) {
    switch (type) {
        case e_motor_type::PMSM: return QString::fromUtf8("PMSM永磁同步电机");
        case e_motor_type::BLDC: return QString::fromUtf8("BLDC无刷直流电机");
        case e_motor_type::ACIM: return QString::fromUtf8("ACIM异步感应电机");
        case e_motor_type::SRM:  return QString::fromUtf8("SRM开关磁阻电机");
        default: return QString::fromUtf8("未知");
    }
}
