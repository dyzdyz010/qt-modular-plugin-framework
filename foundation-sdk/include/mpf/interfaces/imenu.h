#pragma once

#include <QString>
#include <QStringList>
#include <QList>
#include <QVariantList>
#include <QVariantMap>

namespace mpf {

/**
 * @brief Menu item data
 */
struct MenuItem
{
    QString id;
    QString label;
    QString icon;
    QString route;
    QString group;
    int order = 0;
    bool enabled = true;
    QString badge;
    QString pluginId;
    
    QVariantMap toVariantMap() const {
        return {
            {"id", id},
            {"label", label},
            {"icon", icon},
            {"route", route},
            {"group", group},
            {"order", order},
            {"enabled", enabled},
            {"badge", badge},
            {"pluginId", pluginId}
        };
    }
};

/**
 * @brief Menu service interface (pure abstract)
 * 
 * Manages application menu items. Plugins can register their menu entries.
 */
class IMenu
{
public:
    virtual ~IMenu() = default;

    /**
     * @brief Register a menu item
     * @return true if registration succeeded
     */
    virtual bool registerItem(const MenuItem& item) = 0;

    /**
     * @brief Unregister a menu item
     */
    virtual void unregisterItem(const QString& id) = 0;

    /**
     * @brief Unregister all items from a plugin
     */
    virtual void unregisterPlugin(const QString& pluginId) = 0;

    /**
     * @brief Update menu item properties
     */
    virtual bool updateItem(const QString& id, const QVariantMap& updates) = 0;

    /**
     * @brief Update menu item badge
     */
    virtual void setBadge(const QString& id, const QString& badge) = 0;

    /**
     * @brief Enable/disable menu item
     */
    virtual void setEnabled(const QString& id, bool enabled) = 0;

    /**
     * @brief Get all menu items
     */
    virtual QList<MenuItem> items() const = 0;

    /**
     * @brief Get all menu items as QVariantList (for QML)
     */
    virtual QVariantList itemsAsVariant() const = 0;

    /**
     * @brief Get menu items in a specific group
     */
    virtual QVariantList itemsInGroup(const QString& group) const = 0;

    /**
     * @brief Get all group names
     */
    virtual QStringList groups() const = 0;

    /**
     * @brief Get total item count
     */
    virtual int count() const = 0;

    static constexpr int apiVersion() { return 1; }
};

} // namespace mpf
