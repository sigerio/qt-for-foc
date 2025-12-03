/**
 * @file comm_logger.h
 * @brief 通信日志记录类声明
 */

#ifndef COMM_LOGGER_H
#define COMM_LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>

/**
 * @brief 通信日志记录类
 * @note 单例模式，记录Modbus通信数据
 */
class comm_logger_t : public QObject
{
    Q_OBJECT

public:
    static comm_logger_t* instance();

    /* 日志控制 */
    bool start_logging(const QString &file_path);
    void stop_logging();
    bool is_logging() const;

    /* 日志记录 */
    void log_send(const QByteArray &data, const QString &desc = QString());
    void log_recv(const QByteArray &data, const QString &desc = QString());
    void log_info(const QString &msg);
    void log_error(const QString &msg);

private:
    explicit comm_logger_t(QObject *parent = nullptr);
    ~comm_logger_t();

    void write_log(const QString &prefix, const QString &content);
    QString format_hex(const QByteArray &data);

private:
    static comm_logger_t *s_instance;
    QFile *m_file;
    QTextStream *m_stream;
    bool m_is_logging;
};

#endif /* COMM_LOGGER_H */
