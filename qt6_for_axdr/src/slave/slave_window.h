/**
 * @file slave_window.h
 * @brief 从机模拟器窗口类声明
 * @note 独立窗口，用于Modbus从机测试
 */

#ifndef SLAVE_WINDOW_H
#define SLAVE_WINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>
#include "modbus_slave.h"
#include "test_data_config.h"

/**
 * @brief 从机模拟器窗口类
 * @note 提供串口配置、寄存器管理、通信日志功能
 */
class slave_window_t : public QMainWindow
{
    Q_OBJECT

public:
    explicit slave_window_t(QWidget *parent = nullptr);
    ~slave_window_t();

private slots:
    /* 串口控制 */
    void slot_on_start_stop_clicked();
    void slot_on_refresh_ports_clicked();

    /* 配置文件 */
    void slot_on_load_config_clicked();
    void slot_on_save_config_clicked();
    void slot_on_load_default_clicked();

    /* 从机信号处理 */
    void slot_on_slave_state_changed(slave_state_E state);
    void slot_on_slave_error(const QString &error_msg);
    void slot_on_request_received(const QByteArray &data);
    void slot_on_response_sent(const QByteArray &data);
    void slot_on_register_changed(quint16 address, quint16 value);

    /* 日志控制 */
    void slot_on_clear_log_clicked();

private:
    void setup_ui();
    void setup_connections();
    void refresh_serial_ports();
    void refresh_register_table();
    void update_ui_state();
    void append_log(const QString &prefix, const QByteArray &data);
    QString format_hex_data(const QByteArray &data);
    float registers_to_float(quint16 reg_low, quint16 reg_high);

private:
    /* 从机相关 */
    modbus_slave_t *m_slave;
    test_data_config_t *m_config;

    /* 串口配置控件 */
    QComboBox *m_port_combo;
    QComboBox *m_baud_combo;
    QComboBox *m_data_bits_combo;
    QComboBox *m_parity_combo;
    QComboBox *m_stop_bits_combo;
    QSpinBox *m_slave_addr_spin;
    QPushButton *m_refresh_btn;
    QPushButton *m_start_stop_btn;
    QLabel *m_status_label;

    /* 配置文件控件 */
    QPushButton *m_load_config_btn;
    QPushButton *m_save_config_btn;
    QPushButton *m_load_default_btn;

    /* 寄存器表格 */
    QTableWidget *m_register_table;

    /* 通信日志 */
    QTextEdit *m_log_text;
    QPushButton *m_clear_log_btn;

    /* 表格编辑标志 */
    bool m_updating_table;
};

#endif /* SLAVE_WINDOW_H */
