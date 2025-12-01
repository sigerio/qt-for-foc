/**
 * @file modbus_client.h
 * @brief Modbus RTU客户端类声明
 * @note 使用QSerialPort直接实现Modbus RTU协议
 * @note 通信速度和解析速度优先设计
 */

#ifndef MODBUS_CLIENT_H
#define MODBUS_CLIENT_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QVector>
#include <QByteArray>
#include <QMutex>

/* 连接状态枚举 */
typedef enum {
    e_disconnected = 0,   /* 已断开 */
    e_connecting,         /* 连接中 */
    e_connected,          /* 已连接 */
    e_error               /* 错误状态 */
} connection_state_E;

/* 串口配置结构体 */
typedef struct {
    QString port_name;              /* 串口名称 */
    qint32 baud_rate;               /* 波特率 */
    QSerialPort::DataBits data_bits;/* 数据位 */
    QSerialPort::Parity parity;     /* 校验位 */
    QSerialPort::StopBits stop_bits;/* 停止位 */
    int server_address;             /* 从站地址 */
    int response_timeout;           /* 响应超时(ms) */
    int retry_count;                /* 重试次数 */
} serial_config_t;

/* Modbus功能码 */
#define MODBUS_FC_READ_HOLDING_REGISTERS    0x03
#define MODBUS_FC_WRITE_SINGLE_REGISTER     0x06
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS  0x10

/**
 * @brief Modbus RTU客户端类
 * @note 使用QSerialPort实现，异步通信设计
 */
class modbus_client_t : public QObject
{
    Q_OBJECT

public:
    explicit modbus_client_t(QObject *parent = nullptr);
    ~modbus_client_t();

    /* 连接管理 */
    bool connect_device(const serial_config_t &config);
    void disconnect_device();
    connection_state_E get_connection_state() const;

    /* 寄存器读写 - 异步操作 */
    void read_holding_registers(int start_addr, quint16 count);
    void write_holding_register(int addr, quint16 value);
    void write_holding_registers(int start_addr, const QVector<quint16> &values);

    /* 配置获取 */
    serial_config_t get_current_config() const;

signals:
    /* 连接状态信号 */
    void signal_connection_changed(connection_state_E state);
    void signal_error_occurred(const QString &error_msg);

    /* 数据信号 */
    void signal_read_completed(int start_addr, const QVector<quint16> &values);
    void signal_write_completed(int addr, bool success);

private slots:
    void slot_on_ready_read();
    void slot_on_error_occurred(QSerialPort::SerialPortError error);
    void slot_on_timeout();
    void slot_reconnect_timer();

private:
    /* 协议处理 */
    QByteArray build_read_request(quint8 slave_addr, quint16 start_addr, quint16 count);
    QByteArray build_write_single_request(quint8 slave_addr, quint16 addr, quint16 value);
    QByteArray build_write_multiple_request(quint8 slave_addr, quint16 start_addr, 
                                             const QVector<quint16> &values);
    quint16 calc_crc16(const QByteArray &data);
    bool parse_read_response(const QByteArray &response, QVector<quint16> &values);
    bool parse_write_response(const QByteArray &response);
    void handle_reconnect();

private:
    QSerialPort *m_serial_port;       /* 串口对象 */
    serial_config_t m_config;          /* 当前配置 */
    connection_state_E m_state;        /* 连接状态 */
    
    QTimer *m_timeout_timer;           /* 超时定时器 */
    QTimer *m_reconnect_timer;         /* 重连定时器 */
    int m_reconnect_attempts;          /* 重连尝试次数 */
    
    QByteArray m_recv_buffer;          /* 接收缓冲区 */
    int m_expected_response_len;       /* 期望响应长度 */
    int m_current_start_addr;          /* 当前请求起始地址 */
    quint8 m_current_function_code;    /* 当前功能码 */
    
    QMutex m_mutex;                    /* 线程安全锁 */

    static const int MAX_RECONNECT_ATTEMPTS = 3;  /* 最大重连次数 */
    static const int RECONNECT_INTERVAL_MS = 2000;/* 重连间隔(ms) */
};

#endif /* MODBUS_CLIENT_H */
