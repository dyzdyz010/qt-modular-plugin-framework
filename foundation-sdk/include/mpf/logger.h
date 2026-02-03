#pragma once

#include <mpf/interfaces/ilogger.h>
#include <QDebug>

namespace mpf {

/**
 * @brief Global logger access (set by host at startup)
 */
class LoggerAccess
{
public:
    static ILogger* instance() { return s_logger; }
    static void setInstance(ILogger* logger) { s_logger = logger; }
private:
    static inline ILogger* s_logger = nullptr;
};

} // namespace mpf

// Convenience logging macros for plugins
#define MPF_LOG_TRACE(tag, msg) \
    if (auto* _l = mpf::LoggerAccess::instance()) _l->trace(tag, msg); else qDebug() << "[TRACE]" << tag << msg
#define MPF_LOG_DEBUG(tag, msg) \
    if (auto* _l = mpf::LoggerAccess::instance()) _l->debug(tag, msg); else qDebug() << "[DEBUG]" << tag << msg
#define MPF_LOG_INFO(tag, msg) \
    if (auto* _l = mpf::LoggerAccess::instance()) _l->info(tag, msg); else qDebug() << "[INFO]" << tag << msg
#define MPF_LOG_WARNING(tag, msg) \
    if (auto* _l = mpf::LoggerAccess::instance()) _l->warning(tag, msg); else qWarning() << "[WARN]" << tag << msg
#define MPF_LOG_ERROR(tag, msg) \
    if (auto* _l = mpf::LoggerAccess::instance()) _l->error(tag, msg); else qCritical() << "[ERROR]" << tag << msg
