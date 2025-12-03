#include "config_loader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

config_data_t config_loader::load(const QString& path) {
    config_data_t cfg;
    
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开配置文件:" << path;
        return cfg;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "配置文件JSON格式错误";
        return cfg;
    }
    
    QJsonObject root = doc.object();
    
    // 电机参数
    if (root.contains("motor")) {
        QJsonObject m = root["motor"].toObject();
        cfg.motor.rs = m.value("rs").toDouble(0.3);
        cfg.motor.ld = m.value("ld").toDouble(0.001);
        cfg.motor.lq = m.value("lq").toDouble(0.001);
        cfg.motor.psi_f = m.value("psi_f").toDouble(0.15);
        cfg.motor.j = m.value("j").toDouble(0.001);
        cfg.motor.b = m.value("b").toDouble(0.0001);
        cfg.motor.pole_pairs = m.value("pole_pairs").toInt(4);
    }
    
    // 仿真配置
    if (root.contains("sim")) {
        QJsonObject s = root["sim"].toObject();
        cfg.sim.dt = s.value("dt").toDouble(100e-6);
        cfg.sim.speed_ratio = s.value("speed_ratio").toDouble(1.0);
    }
    
    // PID参数
    auto load_pid = [](const QJsonObject& obj) -> pid_params_t {
        pid_params_t p;
        p.kp = obj.value("kp").toDouble(1.0);
        p.ki = obj.value("ki").toDouble(0.0);
        p.kd = obj.value("kd").toDouble(0.0);
        p.out_max = obj.value("out_max").toDouble(100.0);
        p.out_min = obj.value("out_min").toDouble(-100.0);
        p.integral_max = obj.value("integral_max").toDouble(50.0);
        return p;
    };
    
    if (root.contains("current_pid")) {
        cfg.current_pid = load_pid(root["current_pid"].toObject());
    }
    if (root.contains("velocity_pid")) {
        cfg.velocity_pid = load_pid(root["velocity_pid"].toObject());
    }
    if (root.contains("position_pid")) {
        cfg.position_pid = load_pid(root["position_pid"].toObject());
    }
    
    // 控制目标
    if (root.contains("target")) {
        QJsonObject t = root["target"].toObject();
        cfg.target.id_ref = t.value("id_ref").toDouble(0.0);
        cfg.target.iq_ref = t.value("iq_ref").toDouble(0.0);
        cfg.target.vel_ref = t.value("vel_ref").toDouble(100.0);
        cfg.target.pos_ref = t.value("pos_ref").toDouble(0.0);
    }
    
    // 六步换向参数
    if (root.contains("six_step")) {
        QJsonObject ss = root["six_step"].toObject();
        cfg.six_step.kp = ss.value("kp").toDouble(0.01);
        cfg.six_step.ki = ss.value("ki").toDouble(0.1);
        cfg.six_step.integral_max = ss.value("integral_max").toDouble(0.8);
        cfg.has_six_step = true;
    }
    
    cfg.valid = true;
    return cfg;
}

bool config_loader::save(const QString& path, const config_data_t& cfg) {
    QJsonObject root;
    
    // 电机参数
    QJsonObject motor;
    motor["rs"] = cfg.motor.rs;
    motor["ld"] = cfg.motor.ld;
    motor["lq"] = cfg.motor.lq;
    motor["psi_f"] = cfg.motor.psi_f;
    motor["j"] = cfg.motor.j;
    motor["b"] = cfg.motor.b;
    motor["pole_pairs"] = static_cast<int>(cfg.motor.pole_pairs);
    root["motor"] = motor;
    
    // 仿真配置
    QJsonObject sim;
    sim["dt"] = cfg.sim.dt;
    sim["speed_ratio"] = cfg.sim.speed_ratio;
    root["sim"] = sim;
    
    // PID参数
    auto save_pid = [](const pid_params_t& p) -> QJsonObject {
        QJsonObject obj;
        obj["kp"] = p.kp;
        obj["ki"] = p.ki;
        obj["kd"] = p.kd;
        obj["out_max"] = p.out_max;
        obj["out_min"] = p.out_min;
        obj["integral_max"] = p.integral_max;
        return obj;
    };
    
    root["current_pid"] = save_pid(cfg.current_pid);
    root["velocity_pid"] = save_pid(cfg.velocity_pid);
    root["position_pid"] = save_pid(cfg.position_pid);
    
    // 控制目标
    QJsonObject target;
    target["id_ref"] = cfg.target.id_ref;
    target["iq_ref"] = cfg.target.iq_ref;
    target["vel_ref"] = cfg.target.vel_ref;
    target["pos_ref"] = cfg.target.pos_ref;
    root["target"] = target;
    
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "无法写入配置文件:" << path;
        return false;
    }
    
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}
