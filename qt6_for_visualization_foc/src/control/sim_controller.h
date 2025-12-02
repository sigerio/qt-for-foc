#ifndef CONTROL_SIM_CONTROLLER_H
#define CONTROL_SIM_CONTROLLER_H

#include <QObject>
#include "core/types.h"

// 仿真控制器（占位）
class sim_controller : public QObject {
    Q_OBJECT

public:
    explicit sim_controller(QObject* parent = nullptr);
    ~sim_controller() override;

public slots:
    void start();
    void stop();
    void pause();
    void step();
    void reset();

    void set_config(const sim_config_t& cfg);
    sim_config_t get_config() const;

signals:
    void state_updated(const motor_state_t& state);
    void step_completed(int step_index, const QString& step_name);

private:
    sim_config_t m_config;
};

#endif // CONTROL_SIM_CONTROLLER_H
