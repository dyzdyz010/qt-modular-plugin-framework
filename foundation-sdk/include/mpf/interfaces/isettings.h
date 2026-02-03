#pragma once

#include <QString>
#include <QStringList>
#include <QVariant>

namespace mpf {

/**
 * @brief Settings service interface (pure abstract)
 * 
 * Provides typed access to application settings with plugin namespace isolation.
 */
class ISettings
{
public:
    virtual ~ISettings() = default;

    /**
     * @brief Get a setting value
     */
    virtual QVariant value(const QString& pluginId, 
                          const QString& key, 
                          const QVariant& defaultValue = {}) const = 0;

    /**
     * @brief Set a setting value
     */
    virtual void setValue(const QString& pluginId, 
                         const QString& key, 
                         const QVariant& value) = 0;

    /**
     * @brief Remove a setting
     */
    virtual void remove(const QString& pluginId, const QString& key) = 0;

    /**
     * @brief Check if a key exists
     */
    virtual bool contains(const QString& pluginId, const QString& key) const = 0;

    /**
     * @brief Get all keys for a plugin
     */
    virtual QStringList keys(const QString& pluginId) const = 0;

    /**
     * @brief Sync settings to storage
     */
    virtual void sync() = 0;

    static constexpr int apiVersion() { return 1; }
};

} // namespace mpf
