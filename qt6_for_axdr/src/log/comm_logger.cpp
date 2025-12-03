/**
 * @file comm_logger.cpp
 * @brief 通信日志记录类实现
 */

#include "comm_logger.h"
#include <QDir>

comm_logger_t* comm_logger_t::s_instance = nullptr;

comm_logger_t* comm_logger_t::instance()
{
    if (!s_instance) {
        s_instance = new comm_logger_t();
    }
    return s_instance;
}

comm_logger_t::comm_logger_t(QObject *parent)
    : QObject(parent)
    , m_file(nullptr)
    , m_stream(nullptr)
    , m_is_logging(false)
{
}

comm_logger_t::~comm_logger_t()
{
    stop_logging();
}

bool comm_logger_t::start_logging(const QString &file_path)
{
    stop_logging();

    m_file = new QFile(file_path);
    if (!m_file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        delete m_file;
        m_file = nullptr;
        return false;
    }

    m_stream = new QTextStream(m_file);
    m_stream->setEncoding(QStringConverter::Utf8);
    m_is_logging = true;

    log_info("========== 日志开始 ==========");
    return true;
}

void comm_logger_t::stop_logging()
{
    if (m_is_logging) {
        log_info("========== 日志结束 ==========");
    }

    if (m_stream) {
        m_stream->flush();
        delete m_stream;
        m_stream = nullptr;
    }

    if (m_file) {
        m_file->close();
        delete m_file;
        m_file = nullptr;
    }

    m_is_logging = false;
}

bool comm_logger_t::is_logging() const
{
    return m_is_logging;
}

void comm_logger_t::log_send(const QByteArray &data, const QString &desc)
{
    QString content = format_hex(data);
    if (!desc.isEmpty()) {
        content += " | " + desc;
    }
    write_log("发送", content);
}

void comm_logger_t::log_recv(const QByteArray &data, const QString &desc)
{
    QString content = format_hex(data);
    if (!desc.isEmpty()) {
        content += " | " + desc;
    }
    write_log("接收", content);
}

void comm_logger_t::log_info(const QString &msg)
{
    write_log("信息", msg);
}

void comm_logger_t::log_error(const QString &msg)
{
    write_log("错误", msg);
}

void comm_logger_t::write_log(const QString &prefix, const QString &content)
{
    if (!m_is_logging || !m_stream) {
        return;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString line = QString("[%1] [%2] %3").arg(timestamp, prefix, content);

    *m_stream << line << "\n";
    m_stream->flush();
}

QString comm_logger_t::format_hex(const QByteArray &data)
{
    QStringList hex_list;
    for (int i = 0; i < data.size(); ++i) {
        hex_list.append(QString("%1").arg(static_cast<quint8>(data[i]), 2, 16, QChar('0')).toUpper());
    }
    return hex_list.join(" ");
}
