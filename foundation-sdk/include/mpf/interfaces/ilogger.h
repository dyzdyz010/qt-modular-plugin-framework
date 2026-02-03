#pragma once

#include <QString>

namespace mpf {

/**
 * @brief Logger interface (pure abstract)
 * 
 * Provides logging capabilities for plugins.
 */
class ILogger
{
public:
    enum class Level {
        Trace,
        Debug,
        Info,
        Warning,
        Error
    };

    virtual ~ILogger() = default;

    /**
     * @brief Log a message
     */
    virtual void log(Level level, const QString& tag, const QString& message) = 0;

    /**
     * @brief Set minimum log level
     */
    virtual void setMinLevel(Level level) = 0;

    /**
     * @brief Get minimum log level
     */
    virtual Level minLevel() const = 0;

    // Convenience methods
    void trace(const QString& tag, const QString& msg) { log(Level::Trace, tag, msg); }
    void debug(const QString& tag, const QString& msg) { log(Level::Debug, tag, msg); }
    void info(const QString& tag, const QString& msg) { log(Level::Info, tag, msg); }
    void warning(const QString& tag, const QString& msg) { log(Level::Warning, tag, msg); }
    void error(const QString& tag, const QString& msg) { log(Level::Error, tag, msg); }

    static constexpr int apiVersion() { return 1; }
};

} // namespace mpf
