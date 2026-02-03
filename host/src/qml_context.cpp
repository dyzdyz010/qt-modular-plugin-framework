#include "qml_context.h"
#include "service_registry.h"
#include "navigation_service.h"
#include "settings_service.h"
#include "theme_service.h"
#include "menu_service.h"
#include <mpf/version.h>

#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace mpf {

QmlContext::QmlContext(ServiceRegistry* registry, QObject* parent)
    : QObject(parent)
    , m_registry(registry)
{
}

QmlContext::~QmlContext() = default;

void QmlContext::setup(QQmlApplicationEngine* engine)
{
    // Register this as "App" singleton
    engine->rootContext()->setContextProperty("App", this);
    
    // Expose services to QML (as QObject*)
    engine->rootContext()->setContextProperty("Navigation", navigation());
    engine->rootContext()->setContextProperty("Settings", settings());
    engine->rootContext()->setContextProperty("Theme", theme());
    engine->rootContext()->setContextProperty("Menu", menu());
}

QString QmlContext::version() const
{
    return MPF_VERSION_STRING;
}

QObject* QmlContext::navigation() const
{
    // Get interface pointer and cast to QObject via the known implementation
    auto* nav = m_registry->get<INavigation>();
    return dynamic_cast<QObject*>(nav);
}

QObject* QmlContext::settings() const
{
    auto* svc = m_registry->get<ISettings>();
    return dynamic_cast<QObject*>(svc);
}

QObject* QmlContext::theme() const
{
    auto* svc = m_registry->get<ITheme>();
    return dynamic_cast<QObject*>(svc);
}

QObject* QmlContext::menu() const
{
    auto* svc = m_registry->get<IMenu>();
    return dynamic_cast<QObject*>(svc);
}

} // namespace mpf
