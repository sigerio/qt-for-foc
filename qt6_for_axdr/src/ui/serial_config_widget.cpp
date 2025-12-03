/**
 * @file serial_config_widget.cpp
 * @brief 串口配置控件实现
 */

#include "serial_config_widget.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QSerialPortInfo>

serial_config_widget_t::serial_config_widget_t(modbus_client_t *client, QWidget *parent)
    : QWidget(parent)
    , m_client(client)
{
    setup_ui();
    slot_refresh_ports();
    
    /* 连接信号 */
    connect(m_client, &modbus_client_t::signal_connection_changed,
            this, &serial_config_widget_t::slot_on_connection_changed);
}

serial_config_widget_t::~serial_config_widget_t()
{
}

/**
 * @brief 初始化UI
 */
void serial_config_widget_t::setup_ui()
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    
    /* 串口参数组 */
    QGroupBox *port_group = new QGroupBox("串口参数", this);
    QGridLayout *port_layout = new QGridLayout(port_group);
    
    /* 串口选择 */
    port_layout->addWidget(new QLabel("串口:"), 0, 0);
    m_port_combo = new QComboBox(this);
    port_layout->addWidget(m_port_combo, 0, 1);
    m_refresh_btn = new QPushButton("刷新", this);
    port_layout->addWidget(m_refresh_btn, 0, 2);
    
    /* 波特率 */
    port_layout->addWidget(new QLabel("波特率:"), 1, 0);
    m_baud_combo = new QComboBox(this);
    m_baud_combo->addItems({"9600", "19200", "38400", "57600", "115200", "230400", "460800"});
    m_baud_combo->setCurrentText("115200");
    port_layout->addWidget(m_baud_combo, 1, 1);
    
    /* 数据位 */
    port_layout->addWidget(new QLabel("数据位:"), 2, 0);
    m_data_bits_combo = new QComboBox(this);
    m_data_bits_combo->addItems({"5", "6", "7", "8"});
    m_data_bits_combo->setCurrentText("8");
    port_layout->addWidget(m_data_bits_combo, 2, 1);

    /* 校验位 */
    port_layout->addWidget(new QLabel("校验位:"), 3, 0);
    m_parity_combo = new QComboBox(this);
    m_parity_combo->addItems({"无", "奇校验", "偶校验"});
    port_layout->addWidget(m_parity_combo, 3, 1);
    
    /* 停止位 */
    port_layout->addWidget(new QLabel("停止位:"), 4, 0);
    m_stop_bits_combo = new QComboBox(this);
    m_stop_bits_combo->addItems({"1", "1.5", "2"});
    port_layout->addWidget(m_stop_bits_combo, 4, 1);
    
    main_layout->addWidget(port_group);
    
    /* Modbus参数组 */
    QGroupBox *modbus_group = new QGroupBox("Modbus参数", this);
    QGridLayout *modbus_layout = new QGridLayout(modbus_group);
    
    /* 从站地址 */
    modbus_layout->addWidget(new QLabel("从站地址:"), 0, 0);
    m_slave_addr_spin = new QSpinBox(this);
    m_slave_addr_spin->setRange(1, 247);
    m_slave_addr_spin->setValue(1);
    modbus_layout->addWidget(m_slave_addr_spin, 0, 1);
    
    /* 超时时间 */
    modbus_layout->addWidget(new QLabel("超时(ms):"), 1, 0);
    m_timeout_spin = new QSpinBox(this);
    m_timeout_spin->setRange(100, 10000);
    m_timeout_spin->setValue(1000);
    modbus_layout->addWidget(m_timeout_spin, 1, 1);
    
    main_layout->addWidget(modbus_group);
    
    /* 连接按钮 */
    QHBoxLayout *btn_layout = new QHBoxLayout();
    m_connect_btn = new QPushButton("连接", this);
    m_disconnect_btn = new QPushButton("断开", this);
    m_disconnect_btn->setEnabled(false);
    btn_layout->addWidget(m_connect_btn);
    btn_layout->addWidget(m_disconnect_btn);
    btn_layout->addStretch();
    main_layout->addLayout(btn_layout);
    
    /* 状态显示 */
    m_status_label = new QLabel("状态: 未连接", this);
    main_layout->addWidget(m_status_label);
    
    main_layout->addStretch();
    
    /* 连接信号 */
    connect(m_refresh_btn, &QPushButton::clicked, this, &serial_config_widget_t::slot_refresh_ports);
    connect(m_connect_btn, &QPushButton::clicked, this, &serial_config_widget_t::slot_connect_clicked);
    connect(m_disconnect_btn, &QPushButton::clicked, this, &serial_config_widget_t::slot_disconnect_clicked);
}

/**
 * @brief 刷新可用串口列表
 */
void serial_config_widget_t::slot_refresh_ports()
{
    m_port_combo->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        m_port_combo->addItem(info.portName());
    }
}

/**
 * @brief 连接按钮点击
 */
void serial_config_widget_t::slot_connect_clicked()
{
    qDebug() << "[serial_config] 连接按钮被点击";
    serial_config_t config = get_config_from_ui();
    qDebug() << "[serial_config] 配置端口:" << config.port_name << "波特率:" << config.baud_rate;
    m_client->connect_device(config);
}

/**
 * @brief 断开按钮点击
 */
void serial_config_widget_t::slot_disconnect_clicked()
{
    m_client->disconnect_device();
}

/**
 * @brief 连接状态变化处理
 */
void serial_config_widget_t::slot_on_connection_changed(connection_state_E state)
{
    switch (state) {
    case e_disconnected:
        m_status_label->setText("状态: 已断开");
        m_connect_btn->setEnabled(true);
        m_disconnect_btn->setEnabled(false);
        break;
    case e_connecting:
        m_status_label->setText("状态: 连接中...");
        m_connect_btn->setEnabled(false);
        m_disconnect_btn->setEnabled(false);
        break;
    case e_connected:
        m_status_label->setText("状态: 已连接");
        m_connect_btn->setEnabled(false);
        m_disconnect_btn->setEnabled(true);
        break;
    case e_error:
        m_status_label->setText("状态: 连接错误");
        m_connect_btn->setEnabled(true);
        m_disconnect_btn->setEnabled(false);
        break;
    }
}

/**
 * @brief 从UI控件获取配置
 */
serial_config_t serial_config_widget_t::get_config_from_ui()
{
    serial_config_t config;
    config.port_name = m_port_combo->currentText();
    config.baud_rate = m_baud_combo->currentText().toInt();
    
    /* 数据位 */
    int data_bits = m_data_bits_combo->currentText().toInt();
    config.data_bits = static_cast<QSerialPort::DataBits>(data_bits);
    
    /* 校验位 */
    int parity_idx = m_parity_combo->currentIndex();
    QSerialPort::Parity parity_map[] = {QSerialPort::NoParity, QSerialPort::OddParity, QSerialPort::EvenParity};
    config.parity = parity_map[parity_idx];
    
    /* 停止位 */
    int stop_idx = m_stop_bits_combo->currentIndex();
    QSerialPort::StopBits stop_map[] = {QSerialPort::OneStop, QSerialPort::OneAndHalfStop, QSerialPort::TwoStop};
    config.stop_bits = stop_map[stop_idx];
    
    config.server_address = m_slave_addr_spin->value();
    config.response_timeout = m_timeout_spin->value();
    config.retry_count = 3;
    
    return config;
}
