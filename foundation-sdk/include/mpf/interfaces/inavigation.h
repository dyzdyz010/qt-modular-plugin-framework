#pragma once

#include <QString>
#include <QVariantMap>
#include <QObject>

namespace mpf {

/**
 * @brief Navigation/Router interface
 * 
 * Provides navigation capabilities for UI modules.
 * Plugins should use this instead of directly manipulating StackView.
 */
class INavigation : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;
    virtual ~INavigation() = default;

    /**
     * @brief Push a new page onto the navigation stack
     * @param route Route identifier (e.g., "orders/detail")
     * @param params Parameters to pass to the page
     * @return true if navigation succeeded
     */
    Q_INVOKABLE virtual bool push(const QString& route, const QVariantMap& params = {}) = 0;

    /**
     * @brief Pop the current page from the stack
     * @return true if pop succeeded
     */
    Q_INVOKABLE virtual bool pop() = 0;

    /**
     * @brief Pop to the root of the navigation stack
     */
    Q_INVOKABLE virtual void popToRoot() = 0;

    /**
     * @brief Replace the current page
     * @param route Route identifier
     * @param params Parameters to pass
     * @return true if replace succeeded
     */
    Q_INVOKABLE virtual bool replace(const QString& route, const QVariantMap& params = {}) = 0;

    /**
     * @brief Get current route
     * @return Current route string
     */
    Q_INVOKABLE virtual QString currentRoute() const = 0;

    /**
     * @brief Get navigation stack depth
     * @return Number of pages in stack
     */
    Q_INVOKABLE virtual int stackDepth() const = 0;

    /**
     * @brief Check if can go back
     * @return true if stack has more than one page
     */
    Q_INVOKABLE virtual bool canGoBack() const = 0;

    /**
     * @brief Register a route handler
     * @param route Route pattern (can include wildcards)
     * @param qmlComponent QML component URL
     */
    virtual void registerRoute(const QString& route, const QString& qmlComponent) = 0;

signals:
    void navigationChanged(const QString& route, const QVariantMap& params);
    void canGoBackChanged(bool canGoBack);

public:
    static constexpr int apiVersion() { return 2; }
};

} // namespace mpf

#define MPF_INavigation_iid "com.mpf.INavigation/2.0"
Q_DECLARE_INTERFACE(mpf::INavigation, MPF_INavigation_iid)
