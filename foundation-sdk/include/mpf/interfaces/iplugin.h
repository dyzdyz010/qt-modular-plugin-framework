#pragma once

#include <QtPlugin>
#include <QString>
#include <QJsonObject>

namespace mpf {

class ServiceRegistry;

/**
 * @brief Base interface for all plugins
 * 
 * Every plugin must implement this interface. The host will call these
 * methods during plugin lifecycle.
 */
class IPlugin
{
public:
    virtual ~IPlugin() = default;

    /**
     * @brief Called after plugin is loaded, before services are resolved
     * @param registry Service registry for registering provided services
     * @return true if initialization succeeded
     */
    virtual bool initialize(ServiceRegistry* registry) = 0;

    /**
     * @brief Called after all plugins are loaded and services resolved
     * @return true if startup succeeded
     */
    virtual bool start() = 0;

    /**
     * @brief Called when plugin is about to be unloaded
     */
    virtual void stop() = 0;

    /**
     * @brief Get plugin metadata
     * @return JSON object with plugin info (id, version, requires, provides, etc.)
     */
    virtual QJsonObject metadata() const = 0;

    /**
     * @brief Get plugin's QML module URI (if any)
     * @return QML module URI like "YourCo.Orders" or empty string
     */
    virtual QString qmlModuleUri() const { return QString(); }

    /**
     * @brief Get plugin's entry QML file (if any)
     * @return Path like "qrc:/YourCo/Orders/Main.qml" or empty string
     */
    virtual QString entryQml() const { return QString(); }
};

} // namespace mpf

#define MPF_IPlugin_iid "com.mpf.IPlugin/1.0"
Q_DECLARE_INTERFACE(mpf::IPlugin, MPF_IPlugin_iid)
