/**
 * @file main_window.h
 * @brief 主窗口类声明
 */

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QStatusBar>
#include <QLabel>
#include <QTimer>

#include "serial/modbus_client.h"
#include "params/param_manager.h"

/* 前向声明 */
class serial_config_widget_t;
class pid_config_widget_t;
class motor_config_widget_t;

/**
 * @brief 主窗口类
 */
class main_window_t : public QMainWindow
{
    Q_OBJECT

public:
    explicit main_window_t(QWidget *parent = nullptr);
    ~main_window_t();

private slots:
    /* 连接状态槽 */
    void slot_on_connection_changed(connection_state_E state);
    void slot_on_error_occurred(const QString &msg);
    
    /* 数据更新槽 */
    void slot_on_realtime_updated(const realtime_data_t &data);
    
    /* 定时器槽 */
    void slot_poll_realtime_data();

private:
    void setup_ui();
    void setup_connections();
    void update_status_bar(const QString &msg);

private:
    /* 核心对象 */
    modbus_client_t *m_modbus_client;
    param_manager_t *m_param_manager;
    
    /* UI控件 */
    QTabWidget *m_tab_widget;
    serial_config_widget_t *m_serial_config;
    pid_config_widget_t *m_pid_config;
    motor_config_widget_t *m_motor_config;
    
    /* 状态栏 */
    QLabel *m_status_label;
    QLabel *m_realtime_label;
    
    /* 定时器 */
    QTimer *m_poll_timer;
    
    static const int POLL_INTERVAL_MS = 100;  /* 实时数据轮询间隔 */
};

#endif /* MAIN_WINDOW_H */
