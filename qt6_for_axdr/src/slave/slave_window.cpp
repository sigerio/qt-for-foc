/**
 * @file slave_window.cpp
 * @brief 从机模拟器窗口类实现
 */

#include "slave_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QSplitter>

slave_window_t::slave_window_t(QWidget *parent)
    : QMainWindow(parent)
    , m_slave(nullptr)
    , m_config(nullptr)
    , m_updating_table(false)
{
    m_slave = new modbus_slave_t(this);
    m_config = new test_data_config_t(this);

    setup_ui();
    setup_connections();

    /* 加载默认配置 */
    m_config->load_default_config();
    m_config->apply_to_slave(m_slave);
    refresh_register_table();
    refresh_serial_ports();
}

slave_window_t::~slave_window_t()
{
}

void slave_window_t::setup_ui()
{
    setWindowTitle("从机模拟器");
    resize(900, 700);

    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QVBoxLayout *main_layout = new QVBoxLayout(central_widget);

    /* 串口配置区域 */
    QGroupBox *serial_group = new QGroupBox("串口配置", this);
    QHBoxLayout *serial_layout = new QHBoxLayout(serial_group);

    serial_layout->addWidget(new QLabel("端口:", this));
    m_port_combo = new QComboBox(this);
    m_port_combo->setMinimumWidth(100);
    serial_layout->addWidget(m_port_combo);

    m_refresh_btn = new QPushButton("刷新", this);
    serial_layout->addWidget(m_refresh_btn);

    serial_layout->addWidget(new QLabel("波特率:", this));
    m_baud_combo = new QComboBox(this);
    m_baud_combo->addItems({"9600", "19200", "38400", "57600", "115200"});
    m_baud_combo->setCurrentText("115200");
    serial_layout->addWidget(m_baud_combo);

    serial_layout->addWidget(new QLabel("数据位:", this));
    m_data_bits_combo = new QComboBox(this);
    m_data_bits_combo->addItems({"8", "7", "6", "5"});
    serial_layout->addWidget(m_data_bits_combo);

    serial_layout->addWidget(new QLabel("校验:", this));
    m_parity_combo = new QComboBox(this);
    m_parity_combo->addItems({"无", "奇校验", "偶校验"});
    serial_layout->addWidget(m_parity_combo);

    serial_layout->addWidget(new QLabel("停止位:", this));
    m_stop_bits_combo = new QComboBox(this);
    m_stop_bits_combo->addItems({"1", "1.5", "2"});
    serial_layout->addWidget(m_stop_bits_combo);

    serial_layout->addWidget(new QLabel("从机地址:", this));
    m_slave_addr_spin = new QSpinBox(this);
    m_slave_addr_spin->setRange(1, 247);
    m_slave_addr_spin->setValue(1);
    serial_layout->addWidget(m_slave_addr_spin);

    m_start_stop_btn = new QPushButton("启动监听", this);
    serial_layout->addWidget(m_start_stop_btn);

    m_status_label = new QLabel("状态: 已停止", this);
    m_status_label->setStyleSheet("color: gray;");
    serial_layout->addWidget(m_status_label);

    serial_layout->addStretch();
    main_layout->addWidget(serial_group);

    /* 配置文件区域 */
    QGroupBox *config_group = new QGroupBox("配置管理", this);
    QHBoxLayout *config_layout = new QHBoxLayout(config_group);

    m_load_config_btn = new QPushButton("加载配置", this);
    config_layout->addWidget(m_load_config_btn);

    m_save_config_btn = new QPushButton("保存配置", this);
    config_layout->addWidget(m_save_config_btn);

    m_load_default_btn = new QPushButton("加载默认", this);
    config_layout->addWidget(m_load_default_btn);

    config_layout->addStretch();
    main_layout->addWidget(config_group);

    /* 主内容区域 - 使用分割器 */
    QSplitter *splitter = new QSplitter(Qt::Vertical, this);

    /* 寄存器表格 */
    QGroupBox *table_group = new QGroupBox("寄存器列表", this);
    QVBoxLayout *table_layout = new QVBoxLayout(table_group);

    m_register_table = new QTableWidget(this);
    m_register_table->setColumnCount(3);
    m_register_table->setHorizontalHeaderLabels({"地址", "名称", "值"});
    m_register_table->horizontalHeader()->setStretchLastSection(true);
    m_register_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_register_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_register_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_register_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_layout->addWidget(m_register_table);

    splitter->addWidget(table_group);

    /* 通信日志 */
    QGroupBox *log_group = new QGroupBox("通信日志", this);
    QVBoxLayout *log_layout = new QVBoxLayout(log_group);

    m_log_text = new QTextEdit(this);
    m_log_text->setReadOnly(true);
    m_log_text->setFont(QFont("Consolas", 10));
    log_layout->addWidget(m_log_text);

    QHBoxLayout *log_btn_layout = new QHBoxLayout();
    m_clear_log_btn = new QPushButton("清除日志", this);
    log_btn_layout->addWidget(m_clear_log_btn);
    log_btn_layout->addStretch();
    log_layout->addLayout(log_btn_layout);

    splitter->addWidget(log_group);

    splitter->setSizes({400, 200});
    main_layout->addWidget(splitter, 1);
}

void slave_window_t::setup_connections()
{
    /* 串口控制 */
    connect(m_start_stop_btn, &QPushButton::clicked,
            this, &slave_window_t::slot_on_start_stop_clicked);
    connect(m_refresh_btn, &QPushButton::clicked,
            this, &slave_window_t::slot_on_refresh_ports_clicked);

    /* 配置文件 */
    connect(m_load_config_btn, &QPushButton::clicked,
            this, &slave_window_t::slot_on_load_config_clicked);
    connect(m_save_config_btn, &QPushButton::clicked,
            this, &slave_window_t::slot_on_save_config_clicked);
    connect(m_load_default_btn, &QPushButton::clicked,
            this, &slave_window_t::slot_on_load_default_clicked);

    /* 从机信号 */
    connect(m_slave, &modbus_slave_t::signal_state_changed,
            this, &slave_window_t::slot_on_slave_state_changed);
    connect(m_slave, &modbus_slave_t::signal_error_occurred,
            this, &slave_window_t::slot_on_slave_error);
    connect(m_slave, &modbus_slave_t::signal_request_received,
            this, &slave_window_t::slot_on_request_received);
    connect(m_slave, &modbus_slave_t::signal_response_sent,
            this, &slave_window_t::slot_on_response_sent);
    connect(m_slave, &modbus_slave_t::signal_register_changed,
            this, &slave_window_t::slot_on_register_changed);

    /* 日志控制 */
    connect(m_clear_log_btn, &QPushButton::clicked,
            this, &slave_window_t::slot_on_clear_log_clicked);

    /* 从机地址变更 */
    connect(m_slave_addr_spin, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int value) {
                m_slave->set_slave_address(static_cast<quint8>(value));
            });
}

void slave_window_t::refresh_serial_ports()
{
    m_port_combo->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        m_port_combo->addItem(info.portName());
    }
}

void slave_window_t::refresh_register_table()
{
    m_updating_table = true;

    QMap<quint16, register_info_t> registers = m_slave->get_all_registers();

    /* 计算显示行数（跳过_H结尾的寄存器） */
    int visible_count = 0;
    for (auto it = registers.begin(); it != registers.end(); ++it) {
        if (!it.value().name.endsWith("_H")) {
            ++visible_count;
        }
    }
    m_register_table->setRowCount(visible_count);

    int row = 0;
    for (auto it = registers.begin(); it != registers.end(); ++it) {
        quint16 addr = it.key();
        QString name = it.value().name;
        quint16 value = it.value().value;

        /* 跳过_H结尾的寄存器（高16位） */
        if (name.endsWith("_H")) {
            continue;
        }

        /* 地址列 */
        QTableWidgetItem *addr_item = new QTableWidgetItem(
            QString("0x%1").arg(addr, 4, 16, QChar('0')).toUpper());
        addr_item->setFlags(addr_item->flags() & ~Qt::ItemIsEditable);
        addr_item->setData(Qt::UserRole, addr);
        m_register_table->setItem(row, 0, addr_item);

        /* 名称列 - 去掉_L后缀 */
        QString display_name = name;
        if (name.endsWith("_L")) {
            display_name = name.left(name.length() - 2);
        }
        QTableWidgetItem *name_item = new QTableWidgetItem(display_name);
        name_item->setFlags(name_item->flags() & ~Qt::ItemIsEditable);
        m_register_table->setItem(row, 1, name_item);

        /* 值列 - 根据类型显示 */
        QString value_str;
        if (name.endsWith("_L")) {
            /* float类型：组合_L和_H显示 */
            quint16 high_addr = addr + 1;
            if (registers.contains(high_addr)) {
                quint16 low_val = value;
                quint16 high_val = registers[high_addr].value;
                float f = registers_to_float(low_val, high_val);
                value_str = QString::number(f, 'g', 6);
            }
        } else {
            /* 整型：直接显示 */
            value_str = QString::number(value);
        }
        QTableWidgetItem *value_item = new QTableWidgetItem(value_str);
        value_item->setFlags(value_item->flags() & ~Qt::ItemIsEditable);
        m_register_table->setItem(row, 2, value_item);

        ++row;
    }

    m_updating_table = false;
}

void slave_window_t::update_ui_state()
{
    bool is_running = (m_slave->get_state() == e_slave_running);

    m_port_combo->setEnabled(!is_running);
    m_baud_combo->setEnabled(!is_running);
    m_data_bits_combo->setEnabled(!is_running);
    m_parity_combo->setEnabled(!is_running);
    m_stop_bits_combo->setEnabled(!is_running);
    m_slave_addr_spin->setEnabled(!is_running);
    m_refresh_btn->setEnabled(!is_running);

    m_start_stop_btn->setText(is_running ? "停止监听" : "启动监听");
}

void slave_window_t::append_log(const QString &prefix, const QByteArray &data)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString hex_str = format_hex_data(data);
    QString log_line = QString("[%1] %2: %3").arg(timestamp, prefix, hex_str);

    m_log_text->append(log_line);

    /* 滚动到底部 */
    QTextCursor cursor = m_log_text->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_log_text->setTextCursor(cursor);
}

QString slave_window_t::format_hex_data(const QByteArray &data)
{
    QStringList hex_list;
    for (int i = 0; i < data.size(); ++i) {
        hex_list.append(QString("%1").arg(static_cast<quint8>(data[i]), 2, 16, QChar('0')).toUpper());
    }
    return hex_list.join(" ");
}

float slave_window_t::registers_to_float(quint16 reg_low, quint16 reg_high)
{
    /* 小端序: reg_low=低16位, reg_high=高16位 */
    union {
        quint32 u32;
        float f;
    } converter;
    converter.u32 = (static_cast<quint32>(reg_high) << 16) | reg_low;
    return converter.f;
}

void slave_window_t::slot_on_start_stop_clicked()
{
    if (m_slave->get_state() == e_slave_running) {
        m_slave->stop_listening();
    } else {
        if (m_port_combo->currentText().isEmpty()) {
            QMessageBox::warning(this, "警告", "请选择串口");
            return;
        }

        /* 获取串口参数 */
        QString port_name = m_port_combo->currentText();
        qint32 baud_rate = m_baud_combo->currentText().toInt();

        QSerialPort::DataBits data_bits = QSerialPort::Data8;
        switch (m_data_bits_combo->currentIndex()) {
        case 0: data_bits = QSerialPort::Data8; break;
        case 1: data_bits = QSerialPort::Data7; break;
        case 2: data_bits = QSerialPort::Data6; break;
        case 3: data_bits = QSerialPort::Data5; break;
        }

        QSerialPort::Parity parity = QSerialPort::NoParity;
        switch (m_parity_combo->currentIndex()) {
        case 0: parity = QSerialPort::NoParity; break;
        case 1: parity = QSerialPort::OddParity; break;
        case 2: parity = QSerialPort::EvenParity; break;
        }

        QSerialPort::StopBits stop_bits = QSerialPort::OneStop;
        switch (m_stop_bits_combo->currentIndex()) {
        case 0: stop_bits = QSerialPort::OneStop; break;
        case 1: stop_bits = QSerialPort::OneAndHalfStop; break;
        case 2: stop_bits = QSerialPort::TwoStop; break;
        }

        m_slave->set_slave_address(static_cast<quint8>(m_slave_addr_spin->value()));
        m_slave->start_listening(port_name, baud_rate, data_bits, parity, stop_bits);
    }
}

void slave_window_t::slot_on_refresh_ports_clicked()
{
    refresh_serial_ports();
}

void slave_window_t::slot_on_load_config_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(
        this, "加载配置文件", QString(), "JSON文件 (*.json);;所有文件 (*)");

    if (file_path.isEmpty()) {
        return;
    }

    if (m_config->load_config(file_path)) {
        m_config->apply_to_slave(m_slave);
        refresh_register_table();
        QMessageBox::information(this, "提示", "配置加载成功");
    }
}

void slave_window_t::slot_on_save_config_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(
        this, "保存配置文件", QString(), "JSON文件 (*.json);;所有文件 (*)");

    if (file_path.isEmpty()) {
        return;
    }

    m_config->load_from_slave(m_slave);
    if (m_config->save_config(file_path)) {
        QMessageBox::information(this, "提示", "配置保存成功");
    }
}

void slave_window_t::slot_on_load_default_clicked()
{
    m_config->load_default_config();
    m_config->apply_to_slave(m_slave);
    refresh_register_table();
    QMessageBox::information(this, "提示", "已加载默认配置");
}

void slave_window_t::slot_on_slave_state_changed(slave_state_E state)
{
    update_ui_state();

    switch (state) {
    case e_slave_stopped:
        m_status_label->setText("状态: 已停止");
        m_status_label->setStyleSheet("color: gray;");
        break;
    case e_slave_running:
        m_status_label->setText("状态: 运行中");
        m_status_label->setStyleSheet("color: green;");
        break;
    case e_slave_error:
        m_status_label->setText("状态: 错误");
        m_status_label->setStyleSheet("color: red;");
        break;
    }
}

void slave_window_t::slot_on_slave_error(const QString &error_msg)
{
    QMessageBox::critical(this, "错误", error_msg);
    m_log_text->append(QString("<span style='color:red;'>[错误] %1</span>").arg(error_msg));
}

void slave_window_t::slot_on_request_received(const QByteArray &data)
{
    append_log("收到请求", data);
}

void slave_window_t::slot_on_response_sent(const QByteArray &data)
{
    append_log("发送响应", data);
}

void slave_window_t::slot_on_register_changed(quint16 address, quint16 value)
{
    Q_UNUSED(value);
    m_updating_table = true;

    QMap<quint16, register_info_t> registers = m_slave->get_all_registers();
    QString changed_name = registers.contains(address) ? registers[address].name : "";

    for (int row = 0; row < m_register_table->rowCount(); ++row) {
        QTableWidgetItem *addr_item = m_register_table->item(row, 0);
        if (!addr_item) continue;

        quint16 row_addr = static_cast<quint16>(addr_item->data(Qt::UserRole).toUInt());
        QString row_name = registers.contains(row_addr) ? registers[row_addr].name : "";

        /* 判断是否需要更新此行 */
        bool need_update = false;
        if (row_addr == address) {
            need_update = true;
        } else if (row_name.endsWith("_L") && (row_addr + 1) == address) {
            /* _H被改变时，更新对应的_L行 */
            need_update = true;
        }

        if (need_update) {
            QTableWidgetItem *value_item = m_register_table->item(row, 2);
            if (value_item) {
                QString value_str;
                if (row_name.endsWith("_L")) {
                    /* float类型 */
                    quint16 high_addr = row_addr + 1;
                    if (registers.contains(row_addr) && registers.contains(high_addr)) {
                        float f = registers_to_float(registers[row_addr].value, 
                                                     registers[high_addr].value);
                        value_str = QString::number(f, 'g', 6);
                    }
                } else {
                    /* 整型 */
                    value_str = QString::number(registers[row_addr].value);
                }
                value_item->setText(value_str);
            }
        }
    }

    m_updating_table = false;
}

void slave_window_t::slot_on_clear_log_clicked()
{
    m_log_text->clear();
}
