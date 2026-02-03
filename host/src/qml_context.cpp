#include "qml_context.h"
#include <mpf/service_registry.h>
#include <mpf/version.h>
#include <mpf/interfaces/inavigation.h>
#include <mpf/interfaces/isettings.h>
#include <mpf/interfaces/itheme.h>
#include <mpf/interfaces/imenu.h>

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
    
    // Also expose individual services directly for convenience
    if (auto* nav = m_registry->get<INavigation>()) {
        engine->rootContext()->setContextProperty("Navigation", nav);
    }
    if (auto* settings = m_registry->get<ISettings>()) {
        engine->rootContext()->setContextProperty("Settings", settings);
    }
    if (auto* theme = m_registry->get<ITheme>()) {
        engine->rootContext()->setContextProperty("Theme", theme);
    }
    if (auto* menu = m_registry->get<IMenu>()) {
        engine->rootContext()->setContextProperty("Menu", menu);
    }
}

QString QmlContext::version() const
{
    return MPF_VERSION_STRING;
}

QObject* QmlContext::navigation() const
{
    return m_registry->get<INavigation>();
}

QObject* QmlContext::settings() const
{
    return m_registry->get<ISettings>();
}

QObject* QmlContext::theme() const
{
    return m_registry->get<ITheme>();
}

QObject* QmlContext::menu() const
{
    return m_registry->get<IMenu>();
}

} // namespace mpf
