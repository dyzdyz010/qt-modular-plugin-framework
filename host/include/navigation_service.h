#pragma once

#include "mpf/interfaces/inavigation.h"
#include <QHash>
#include <QStack>

class QQmlApplicationEngine;

namespace mpf {

/**
 * @brief Default navigation service implementation
 */
class NavigationService : public INavigation
{
    Q_OBJECT

public:
    explicit NavigationService(QQmlApplicationEngine* engine, QObject* parent = nullptr);
    ~NavigationService() override;

    // INavigation interface
    bool push(const QString& route, const QVariantMap& params = {}) override;
    bool pop() override;
    void popToRoot() override;
    bool replace(const QString& route, const QVariantMap& params = {}) override;
    QString currentRoute() const override;
    int stackDepth() const override;
    bool canGoBack() const override;
    void registerRoute(const QString& route, const QString& qmlComponent) override;

    /**
     * @brief Set the StackView object ID in QML
     */
    void setStackViewId(const QString& id) { m_stackViewId = id; }

private:
    QObject* stackView() const;
    QVariant resolveComponent(const QString& route);

    QQmlApplicationEngine* m_engine;
    QString m_stackViewId = "mainStackView";
    
    struct RouteEntry {
        QString pattern;
        QString component;
    };
    QList<RouteEntry> m_routes;
    
    struct StackEntry {
        QString route;
        QVariantMap params;
    };
    QStack<StackEntry> m_stack;
};

} // namespace mpf
