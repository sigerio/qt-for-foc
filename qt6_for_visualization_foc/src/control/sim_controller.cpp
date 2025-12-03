#include "sim_controller.h"

sim_controller::sim_controller(QObject* parent) : QObject(parent) {}
sim_controller::~sim_controller() = default;

void sim_controller::start() { m_config.running = true; }
void sim_controller::stop() { m_config.running = false; }
void sim_controller::pause() { m_config.running = false; }
void sim_controller::step() { m_config.single_step = true; }
void sim_controller::reset() { m_config = sim_config_t{}; }
void sim_controller::set_config(const sim_config_t& cfg) { m_config = cfg; }
sim_config_t sim_controller::get_config() const { return m_config; }
