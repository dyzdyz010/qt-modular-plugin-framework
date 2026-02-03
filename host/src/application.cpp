#include "application.h"
#include "plugin_manager.h"
#include "plugin_loader.h"
#include "plugin_metadata.h"
#include "navigation_service.h"
#include "settings_service.h"
#include "theme_service.h"
#include "menu_service.h"
#include "qml_context.h"

#include "service_registry.h"
#include "logger.h"
#include <mpf/interfaces/inavigation.h>
#include <mpf/interfaces/isettings.h>
#include <mpf/interfaces/itheme.h>
#include <mpf/interfaces/imenu.h>

#include <QQmlContext>
#include <QDir>
#include <QFile>
#include <QUrl>
#include <QStandardPaths>
#include <QDebug>

namespace mpf {

Application* Application::s_instance = nullptr;

Application::Application(int& argc, char** argv)
{
    s_instance = this;
    
    m_app = std::make_unique<QGuiApplication>(argc, argv);
    m_app->setOrganizationName("MPF");
    m_app->setApplicationName("QtModularPluginFramework");
    m_app->setApplicationVersion("1.0.0");
}

Application::~Application()
{
    if (m_pluginManager) {
        m_pluginManager->stopAll();
        m_pluginManager->unloadAll();
    }
    
    if (s_instance == this) {
        s_instance = nullptr;
    }
}

bool Application::initialize()
{
    setupPaths();
    setupLogging();
    
    // Create service registry
    m_registry = std::make_unique<ServiceRegistryImpl>(this);
    
    // Create and register core services
    auto* navigation = new NavigationService(nullptr, this);
    auto* settings = new SettingsService(m_configPath, this);
    auto* theme = new ThemeService(this);
    auto* menu = new MenuService(this);
    
    m_registry->add<INavigation>(navigation, INavigation::apiVersion(), "host");
    m_registry->add<ISettings>(settings, ISettings::apiVersion(), "host");
    m_registry->add<ITheme>(theme, ITheme::apiVersion(), "host");
    m_registry->add<IMenu>(menu, IMenu::apiVersion(), "host");
    m_registry->add<ILogger>(m_logger.get(), ILogger::apiVersion(), "host");
    
    // Create QML engine
    m_engine = std::make_unique<QQmlApplicationEngine>();
    
    // Update navigation with engine reference
    static_cast<NavigationService*>(navigation)->~NavigationService();
    new (navigation) NavigationService(m_engine.get(), this);
    
    setupQmlContext();
    loadPlugins();
    
    if (!loadMainQml()) {
        return false;
    }
    
    emit initialized();
    return true;
}

int Application::run()
{
    connect(m_app.get(), &QCoreApplication::aboutToQuit, this, [this]() {
        emit aboutToQuit();
    });
    
    return m_app->exec();
}

QStringList Application::arguments() const
{
    return m_app->arguments();
}

void Application::setupPaths()
{
    QString appDir = QCoreApplication::applicationDirPath();
    
    // Determine paths relative to executable
    m_pluginPath = appDir + "/../plugins";
    m_qmlPath = appDir + "/../qml";
    m_configPath = appDir + "/../config";
    
    // Normalize paths
    m_pluginPath = QDir(m_pluginPath).absolutePath();
    m_qmlPath = QDir(m_qmlPath).absolutePath();
    m_configPath = QDir(m_configPath).absolutePath();
    
    // Create config directory if needed
    QDir().mkpath(m_configPath);
    
    qDebug() << "Plugin path:" << m_pluginPath;
    qDebug() << "QML path:" << m_qmlPath;
    qDebug() << "Config path:" << m_configPath;
    
    // Add library path for plugins
    m_app->addLibraryPath(m_pluginPath);
}

void Application::setupLogging()
{
    m_logger = std::make_unique<Logger>(this);
    m_logger->setFormat("[%time%] [%level%] [%tag%] %message%");
    m_logger->setMinLevel(ILogger::Level::Debug);
}

void Application::setupQmlContext()
{
    // Add QML import paths
    m_engine->addImportPath(m_qmlPath);
    m_engine->addImportPath("qrc:/");
    
    // Add host QML directory for component discovery
    QString hostQmlDir = QCoreApplication::applicationDirPath() + "/../host/MPF/Host/qml";
    if (QDir(hostQmlDir).exists()) {
        m_engine->addImportPath(hostQmlDir);
    }
    
    // Create and setup QML context helper
    auto* qmlContext = new QmlContext(m_registry.get(), this);
    qmlContext->setup(m_engine.get());
    
    qDebug() << "QML import paths:" << m_engine->importPathList();
}

void Application::loadPlugins()
{
    m_pluginManager = std::make_unique<PluginManager>(m_registry.get(), this);
    
    // Connect signals for logging
    connect(m_pluginManager.get(), &PluginManager::pluginDiscovered,
            [](const QString& id) { qDebug() << "Discovered plugin:" << id; });
    connect(m_pluginManager.get(), &PluginManager::pluginLoaded,
            [](const QString& id) { qDebug() << "Loaded plugin:" << id; });
    connect(m_pluginManager.get(), &PluginManager::pluginError,
            [](const QString& id, const QString& err) { 
                qWarning() << "Plugin error:" << id << "-" << err; 
            });
    
    // Discover plugins
    int count = m_pluginManager->discover(m_pluginPath);
    qDebug() << "Discovered" << count << "plugins";
    
    // Load, initialize, and start
    if (m_pluginManager->loadAll()) {
        if (m_pluginManager->initializeAll()) {
            m_pluginManager->startAll();
        }
    }
    
    // Register plugin QML modules
    for (const QString& uri : m_pluginManager->qmlModuleUris()) {
        qDebug() << "Plugin QML module:" << uri;
    }
}

bool Application::loadMainQml()
{
    // Try to find entry QML from plugins first
    QString entryQml;
    for (auto* loader : m_pluginManager->plugins()) {
        QString pluginEntry = m_pluginManager->entryQml(loader->metadata().id());
        if (!pluginEntry.isEmpty()) {
            entryQml = pluginEntry;
            break;
        }
    }
    
    // Fall back to host's Main.qml
    if (entryQml.isEmpty()) {
        // Try filesystem path first (development), then qrc (release)
        QString fsPath = QCoreApplication::applicationDirPath() + "/../host/MPF/Host/qml/Main.qml";
        if (QFile::exists(fsPath)) {
            entryQml = QUrl::fromLocalFile(fsPath).toString();
        } else {
            entryQml = "qrc:/qt/qml/MPF/Host/Main.qml";
        }
    }
    
    qDebug() << "Loading main QML:" << entryQml;
    
    m_engine->load(QUrl(entryQml));
    
    if (m_engine->rootObjects().isEmpty()) {
        qCritical() << "Failed to load main QML";
        return false;
    }
    
    return true;
}

} // namespace mpf
