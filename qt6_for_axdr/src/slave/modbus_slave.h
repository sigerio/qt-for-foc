/**
 * @file modbus_slave.h
 * @brief Modbus RTU从机模拟器类声明
 * @note 用于测试模式，模拟AxDr控制板响应
 */

#ifndef MODBUS_SLAVE_H
#define MODBUS_SLAVE_H

#include <QObject>
#include <QSerialPort>
#include <QMap>
#include <QByteArray>
#include <QTimer>

/* 从机状态枚举 */
typedef enum {
    e_slave_stopped = 0,    /* 已停止 */
    e_slave_running,        /* 运行中 */
    e_slave_error           /* 错误状态 */
} slave_state_E;

/* 寄存器信息结构体 */
typedef struct {
    QString name;           /* 寄存器名称 */
    quint16 value;          /* 寄存器值 */
} register_info_t;

/* Modbus异常码 */
#define MODBUS_EX_ILLEGAL_FUNCTION      0x01
#define MODBUS_EX_ILLEGAL_DATA_ADDRESS  0x02
#define MODBUS_EX_ILLEGAL_DATA_VALUE    0x03

/* Modbus功能码 */
#define MODBUS_FC_READ_HOLDING_REGISTERS    0x03
#define MODBUS_FC_WRITE_SINGLE_REGISTER     0x06
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS  0x10

/**
 * @brief Modbus RTU从机模拟器类
 * @note 监听串口请求并返回模拟响应
 */
class modbus_slave_t : public QObject
{
    Q_OBJECT

public:
    explicit modbus_slave_t(QObject *parent = nullptr);
    ~modbus_slave_t();

    /* 从机控制 */
    bool start_listening(const QString &port_name, qint32 baud_rate,
                         QSerialPort::DataBits data_bits = QSerialPort::Data8,
                         QSerialPort::Parity parity = QSerialPort::NoParity,
                         QSerialPort::StopBits stop_bits = QSerialPort::OneStop);
    void stop_listening();
    slave_state_E get_state() const;

    /* 从机地址设置 */
    void set_slave_address(quint8 address);
    quint8 get_slave_address() const;

    /* 寄存器管理 */
    void set_register(quint16 address, quint16 value);
    void set_register_with_name(quint16 address, const QString &name, quint16 value);
    quint16 get_register(quint16 address) const;
    QString get_register_name(quint16 address) const;
    QMap<quint16, register_info_t> get_all_registers() const;
    void clear_registers();

signals:
    /* 状态信号 */
    void signal_state_changed(slave_state_E state);
    void signal_error_occurred(const QString &error_msg);

    /* 通信日志信号 */
    void signal_request_received(const QByteArray &data);
    void signal_response_sent(const QByteArray &data);

    /* 寄存器变更信号 */
    void signal_register_changed(quint16 address, quint16 value);

private slots:
    void slot_on_ready_read();
    void slot_on_serial_error(QSerialPort::SerialPortError error);
    void slot_process_frame();

private:
    /* 协议处理 */
    void process_request(const QByteArray &request);
    QByteArray handle_read_holding_registers(quint16 start_addr, quint16 count);
    QByteArray handle_write_single_register(quint16 addr, quint16 value);
    QByteArray handle_write_multiple_registers(quint16 start_addr, const QByteArray &data);
    QByteArray build_exception_response(quint8 function_code, quint8 exception_code);
    quint16 calc_crc16(const QByteArray &data);
    bool verify_crc(const QByteArray &data);

private:
    QSerialPort *m_serial_port;             /* 串口对象 */
    quint8 m_slave_address;                 /* 从机地址 */
    slave_state_E m_state;                  /* 从机状态 */
    QByteArray m_recv_buffer;               /* 接收缓冲区 */
    QTimer *m_frame_timer;                  /* 帧间隔定时器 */
    QMap<quint16, register_info_t> m_registers;  /* 寄存器映射表 */

    static const int FRAME_TIMEOUT_MS = 20; /* 帧间隔超时(ms) */
};

#endif /* MODBUS_SLAVE_H */
