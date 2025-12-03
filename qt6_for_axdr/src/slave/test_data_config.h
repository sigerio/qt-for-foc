/**
 * @file test_data_config.h
 * @brief 测试数据配置管理类声明
 * @note 管理从机模拟器的寄存器配置，支持JSON文件读写
 */

#ifndef TEST_DATA_CONFIG_H
#define TEST_DATA_CONFIG_H

#include <QObject>
#include <QString>
#include <QMap>
#include "modbus_slave.h"

/**
 * @brief 测试数据配置管理类
 * @note 负责寄存器配置的加载、保存和默认值初始化
 */
class test_data_config_t : public QObject
{
    Q_OBJECT

public:
    explicit test_data_config_t(QObject *parent = nullptr);
    ~test_data_config_t();

    /* 配置文件操作 */
    bool load_config(const QString &file_path);
    bool save_config(const QString &file_path);

    /* 默认配置 */
    void load_default_config();

    /* 获取寄存器数据 */
    QMap<quint16, register_info_t> get_registers() const;

    /* 应用配置到从机 */
    void apply_to_slave(modbus_slave_t *slave);

    /* 从从机获取当前配置 */
    void load_from_slave(modbus_slave_t *slave);

    /* 获取默认配置文件路径 */
    static QString get_default_config_path();

signals:
    void signal_config_loaded(const QString &file_path);
    void signal_config_saved(const QString &file_path);
    void signal_error_occurred(const QString &error_msg);

private:
    QMap<quint16, register_info_t> m_registers;
};

#endif /* TEST_DATA_CONFIG_H */
