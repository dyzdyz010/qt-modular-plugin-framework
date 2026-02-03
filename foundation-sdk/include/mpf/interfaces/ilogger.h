#pragma once

#include <QObject>
#include <QString>

namespace mpf {

/**
 * @brief Logging interface
 * 
 * Plugins should use this for logging instead of qDebug directly.
 * This allows centralized log management, filtering, and routing.
 */
class ILogger
{
public:
    virtual ~ILogger() = default;

    enum class Level {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    virtual void log(Level level, const QString& tag, const QString& message) = 0;

    // Convenience methods
    virtual void trace(const QString& tag, const QString& message) {
        log(Level::Trace, tag, message);
    }
    virtual void debug(const QString& tag, const QString& message) {
        log(Level::Debug, tag, message);
    }
    virtual void info(const QString& tag, const QString& message) {
        log(Level::Info, tag, message);
    }
    virtual void warning(const QString& tag, const QString& message) {
        log(Level::Warning, tag, message);
    }
    virtual void error(const QString& tag, const QString& message) {
        log(Level::Error, tag, message);
    }
    virtual void fatal(const QString& tag, const QString& message) {
        log(Level::Fatal, tag, message);
    }

    virtual void setMinLevel(Level level) = 0;
    virtual Level minLevel() const = 0;

    static constexpr int apiVersion() { return 1; }
};

} // namespace mpf

#define MPF_ILogger_iid "com.mpf.ILogger/1.0"
Q_DECLARE_INTERFACE(mpf::ILogger, MPF_ILogger_iid)
