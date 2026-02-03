#pragma once

#include <QString>
#include <QVariantMap>

namespace mpf {

/**
 * @brief Navigation/Router interface (pure abstract)
 * 
 * Provides navigation capabilities for UI modules.
 * Implementation provides Qt signals via a separate QObject.
 */
class INavigation
{
public:
    virtual ~INavigation() = default;

    /**
     * @brief Push a new page onto the navigation stack
     */
    virtual bool push(const QString& route, const QVariantMap& params = {}) = 0;

    /**
     * @brief Pop the current page from the stack
     */
    virtual bool pop() = 0;

    /**
     * @brief Pop to the root of the navigation stack
     */
    virtual void popToRoot() = 0;

    /**
     * @brief Replace the current page
     */
    virtual bool replace(const QString& route, const QVariantMap& params = {}) = 0;

    /**
     * @brief Get current route
     */
    virtual QString currentRoute() const = 0;

    /**
     * @brief Get navigation stack depth
     */
    virtual int stackDepth() const = 0;

    /**
     * @brief Check if can go back
     */
    virtual bool canGoBack() const = 0;

    /**
     * @brief Register a route handler
     */
    virtual void registerRoute(const QString& route, const QString& qmlComponent) = 0;

    static constexpr int apiVersion() { return 2; }
};

} // namespace mpf
