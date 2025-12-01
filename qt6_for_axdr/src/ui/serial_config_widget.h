/**
 * @file serial_config_widget.h
 * @brief 串口配置控件声明
 */

#ifndef SERIAL_CONFIG_WIDGET_H
#define SERIAL_CONFIG_WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>

#include "serial/modbus_client.h"

/**
 * @brief 串口配置控件
 */
class serial_config_widget_t : public QWidget
{
    Q_OBJECT

public:
    explicit serial_config_widget_t(modbus_client_t *client, QWidget *parent = nullptr);
    ~serial_config_widget_t();

private slots:
    void slot_refresh_ports();
    void slot_connect_clicked();
    void slot_disconnect_clicked();
    void slot_on_connection_changed(connection_state_E state);

private:
    void setup_ui();
    serial_config_t get_config_from_ui();

private:
    modbus_client_t *m_client;
    
    /* UI控件 */
    QComboBox *m_port_combo;
    QComboBox *m_baud_combo;
    QComboBox *m_data_bits_combo;
    QComboBox *m_parity_combo;
    QComboBox *m_stop_bits_combo;
    QSpinBox *m_slave_addr_spin;
    QSpinBox *m_timeout_spin;
    QPushButton *m_refresh_btn;
    QPushButton *m_connect_btn;
    QPushButton *m_disconnect_btn;
    QLabel *m_status_label;
};

#endif /* SERIAL_CONFIG_WIDGET_H */
