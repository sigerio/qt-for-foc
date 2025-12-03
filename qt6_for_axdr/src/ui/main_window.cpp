/**
 * @file main_window.cpp
 * @brief 主窗口类实现
 */

#include "main_window.h"
#include "serial_config_widget.h"
#include "pid_config_widget.h"
#include "motor_config_widget.h"
#include "slave/slave_window.h"

#include <QVBoxLayout>
#include <QMessageBox>

main_window_t::main_window_t(QWidget *parent)
    : QMainWindow(parent)
    , m_modbus_client(nullptr)
    , m_param_manager(nullptr)
    , m_tab_widget(nullptr)
    , m_poll_timer(nullptr)
    , m_slave_window(nullptr)
{
    /* 创建核心对象 */
    m_modbus_client = new modbus_client_t(this);
    m_param_manager = new param_manager_t(m_modbus_client, this);
    
    /* 创建定时器 */
    m_poll_timer = new QTimer(this);
    
    setup_ui();
    setup_menu();
    setup_connections();
    
    setWindowTitle("AxDr电机控制上位机");
    resize(900, 700);
}

main_window_t::~main_window_t()
{
    m_poll_timer->stop();
}

/**
 * @brief 初始化UI布局
 */
void main_window_t::setup_ui()
{
    /* 创建中央部件 */
    QWidget *central_widget = new QWidget(this);
    QVBoxLayout *main_layout = new QVBoxLayout(central_widget);
    
    /* 创建选项卡 */
    m_tab_widget = new QTabWidget(this);
    
    /* 串口配置页 */
    m_serial_config = new serial_config_widget_t(m_modbus_client, this);
    m_tab_widget->addTab(m_serial_config, "串口配置");
    
    /* PID配置页 */
    m_pid_config = new pid_config_widget_t(m_param_manager, this);
    m_tab_widget->addTab(m_pid_config, "PID参数");
    
    /* 电机配置页 */
    m_motor_config = new motor_config_widget_t(m_param_manager, this);
    m_tab_widget->addTab(m_motor_config, "电机参数");
    
    main_layout->addWidget(m_tab_widget);
    setCentralWidget(central_widget);
    
    /* 状态栏 */
    m_status_label = new QLabel("未连接");
    m_realtime_label = new QLabel("速度: -- | 位置: -- | 电流: --");
    statusBar()->addWidget(m_status_label, 1);
    statusBar()->addPermanentWidget(m_realtime_label);
}

/**
 * @brief 建立信号槽连接
 */
void main_window_t::setup_connections()
{
    /* 连接状态信号 */
    connect(m_modbus_client, &modbus_client_t::signal_connection_changed,
            this, &main_window_t::slot_on_connection_changed);
    connect(m_modbus_client, &modbus_client_t::signal_error_occurred,
            this, &main_window_t::slot_on_error_occurred);
    
    /* 实时数据信号 */
    connect(m_param_manager, &param_manager_t::signal_realtime_updated,
            this, &main_window_t::slot_on_realtime_updated);
    
    /* 定时器 */
    connect(m_poll_timer, &QTimer::timeout,
            this, &main_window_t::slot_poll_realtime_data);
}

/**
 * @brief 连接状态变化槽
 */
void main_window_t::slot_on_connection_changed(connection_state_E state)
{
    switch (state) {
    case e_disconnected:
        m_status_label->setText("已断开");
        m_status_label->setStyleSheet("color: gray;");
        m_poll_timer->stop();
        break;
    case e_connecting:
        m_status_label->setText("连接中...");
        m_status_label->setStyleSheet("color: orange;");
        break;
    case e_connected:
        m_status_label->setText("已连接");
        m_status_label->setStyleSheet("color: green;");
        /* 连接成功，不自动轮询，由用户手动控制 */
        break;
    case e_error:
        m_status_label->setText("连接错误");
        m_status_label->setStyleSheet("color: red;");
        m_poll_timer->stop();
        break;
    }
}

/**
 * @brief 错误发生槽
 */
void main_window_t::slot_on_error_occurred(const QString &msg)
{
    update_status_bar(QString("错误: %1").arg(msg));
}

/**
 * @brief 实时数据更新槽
 */
void main_window_t::slot_on_realtime_updated(const realtime_data_t &data)
{
    QString text = QString("速度: %1 rad/s | 位置: %2 rad | 电流: %3 A")
                       .arg(data.velocity, 0, 'f', 2)
                       .arg(data.position, 0, 'f', 2)
                       .arg(data.current_q, 0, 'f', 3);
    m_realtime_label->setText(text);
}

/**
 * @brief 定时轮询实时数据
 */
void main_window_t::slot_poll_realtime_data()
{
    if (m_modbus_client->get_connection_state() == e_connected) {
        m_param_manager->read_realtime_data();
    }
}

/**
 * @brief 更新状态栏消息
 */
void main_window_t::update_status_bar(const QString &msg)
{
    statusBar()->showMessage(msg, 3000);
}

/**
 * @brief 初始化菜单栏
 */
void main_window_t::setup_menu()
{
    QMenu *tools_menu = menuBar()->addMenu("工具(&T)");
    
    QAction *slave_action = new QAction("打开从机模拟器(&S)", this);
    slave_action->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(slave_action, &QAction::triggered, this, &main_window_t::slot_open_slave_window);
    tools_menu->addAction(slave_action);
}

/**
 * @brief 打开从机模拟器窗口
 */
void main_window_t::slot_open_slave_window()
{
    if (!m_slave_window) {
        m_slave_window = new slave_window_t(nullptr);
        /* 窗口关闭时不删除，只隐藏 */
        m_slave_window->setAttribute(Qt::WA_DeleteOnClose, false);
    }
    
    m_slave_window->show();
    m_slave_window->raise();
    m_slave_window->activateWindow();
}
