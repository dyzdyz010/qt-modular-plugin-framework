#pragma once

#include <QObject>
#include <QString>
#include <QList>
#include <QVariantMap>

namespace mpf {

/**
 * @brief Menu item data structure
 */
struct MenuItem
{
    QString id;           // Unique identifier
    QString title;        // Display text
    QString icon;         // Icon name or path (emoji also works)
    QString route;        // Navigation route when clicked
    QString pluginId;     // Plugin that registered this item
    int order = 0;        // Sort order (lower = higher priority)
    bool enabled = true;  // Whether the item is enabled
    QString badge;        // Optional badge text (e.g., count)
    QString group;        // Optional group name for categorization
    
    QVariantMap toVariantMap() const {
        return {
            {"id", id},
            {"title", title},
            {"icon", icon},
            {"route", route},
            {"pluginId", pluginId},
            {"order", order},
            {"enabled", enabled},
            {"badge", badge},
            {"group", group}
        };
    }
    
    static MenuItem fromVariantMap(const QVariantMap& map) {
        MenuItem item;
        item.id = map.value("id").toString();
        item.title = map.value("title").toString();
        item.icon = map.value("icon").toString();
        item.route = map.value("route").toString();
        item.pluginId = map.value("pluginId").toString();
        item.order = map.value("order").toInt();
        item.enabled = map.value("enabled", true).toBool();
        item.badge = map.value("badge").toString();
        item.group = map.value("group").toString();
        return item;
    }
};

/**
 * @brief Menu service interface
 * 
 * Allows plugins to register themselves in the application menu.
 * The host displays these items in a sidebar or navigation drawer.
 */
class IMenu : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QVariantList items READ itemsAsVariant NOTIFY menuChanged)
    Q_PROPERTY(int count READ count NOTIFY menuChanged)

public:
    using QObject::QObject;
    virtual ~IMenu() = default;

    /**
     * @brief Register a menu item
     * @param item Menu item to register
     * @return true if registration succeeded
     */
    virtual bool registerItem(const MenuItem& item) = 0;
    
    /**
     * @brief Convenience method to register from QVariantMap (for QML)
     */
    Q_INVOKABLE virtual bool registerItem(const QVariantMap& item) {
        return registerItem(MenuItem::fromVariantMap(item));
    }

    /**
     * @brief Unregister a menu item
     * @param id Item ID to remove
     */
    Q_INVOKABLE virtual void unregisterItem(const QString& id) = 0;

    /**
     * @brief Unregister all items from a plugin
     * @param pluginId Plugin ID
     */
    Q_INVOKABLE virtual void unregisterPlugin(const QString& pluginId) = 0;

    /**
     * @brief Update a menu item's properties
     * @param id Item ID
     * @param updates Properties to update
     * @return true if item was found and updated
     */
    Q_INVOKABLE virtual bool updateItem(const QString& id, const QVariantMap& updates) = 0;

    /**
     * @brief Update badge for an item
     * @param id Item ID
     * @param badge New badge text (empty to clear)
     */
    Q_INVOKABLE virtual void setBadge(const QString& id, const QString& badge) = 0;

    /**
     * @brief Enable/disable a menu item
     * @param id Item ID
     * @param enabled Whether the item is enabled
     */
    Q_INVOKABLE virtual void setEnabled(const QString& id, bool enabled) = 0;

    /**
     * @brief Get all menu items (sorted by order)
     */
    virtual QList<MenuItem> items() const = 0;
    
    /**
     * @brief Get items as QVariantList for QML
     */
    Q_INVOKABLE virtual QVariantList itemsAsVariant() const = 0;

    /**
     * @brief Get items in a specific group
     */
    Q_INVOKABLE virtual QVariantList itemsInGroup(const QString& group) const = 0;

    /**
     * @brief Get all group names
     */
    Q_INVOKABLE virtual QStringList groups() const = 0;

    /**
     * @brief Get item count
     */
    virtual int count() const = 0;

signals:
    void menuChanged();
    void itemClicked(const QString& id, const QString& route);

public:
    static constexpr int apiVersion() { return 1; }
};

} // namespace mpf

#define MPF_IMenu_iid "com.mpf.IMenu/1.0"
Q_DECLARE_INTERFACE(mpf::IMenu, MPF_IMenu_iid)
