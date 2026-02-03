#pragma once

#include <QString>
#include <QVariant>
#include <QObject>

namespace mpf {

/**
 * @brief Settings/Configuration interface
 * 
 * Provides persistent settings storage for plugins.
 * Each plugin gets its own namespace to avoid conflicts.
 */
class ISettings : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;
    virtual ~ISettings() = default;

    /**
     * @brief Get a setting value
     * @param pluginId Plugin identifier for namespace isolation
     * @param key Setting key
     * @param defaultValue Default value if not set
     * @return Setting value
     */
    Q_INVOKABLE virtual QVariant value(const QString& pluginId, 
                                        const QString& key, 
                                        const QVariant& defaultValue = {}) const = 0;

    /**
     * @brief Set a setting value
     * @param pluginId Plugin identifier
     * @param key Setting key
     * @param value Value to set
     */
    Q_INVOKABLE virtual void setValue(const QString& pluginId, 
                                       const QString& key, 
                                       const QVariant& value) = 0;

    /**
     * @brief Remove a setting
     * @param pluginId Plugin identifier
     * @param key Setting key
     */
    Q_INVOKABLE virtual void remove(const QString& pluginId, const QString& key) = 0;

    /**
     * @brief Check if a setting exists
     * @param pluginId Plugin identifier
     * @param key Setting key
     * @return true if setting exists
     */
    Q_INVOKABLE virtual bool contains(const QString& pluginId, const QString& key) const = 0;

    /**
     * @brief Get all keys for a plugin
     * @param pluginId Plugin identifier
     * @return List of setting keys
     */
    Q_INVOKABLE virtual QStringList keys(const QString& pluginId) const = 0;

    /**
     * @brief Sync settings to persistent storage
     */
    Q_INVOKABLE virtual void sync() = 0;

signals:
    void settingChanged(const QString& pluginId, const QString& key, const QVariant& value);

public:
    static constexpr int apiVersion() { return 1; }
};

} // namespace mpf

#define MPF_ISettings_iid "com.mpf.ISettings/1.0"
Q_DECLARE_INTERFACE(mpf::ISettings, MPF_ISettings_iid)
