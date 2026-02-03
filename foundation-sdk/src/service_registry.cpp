#include "mpf/service_registry.h"
#include <QDebug>

namespace mpf {

ServiceRegistry::ServiceRegistry(QObject* parent)
    : QObject(parent)
{
}

ServiceRegistry::~ServiceRegistry()
{
    QMutexLocker locker(&m_mutex);
    m_services.clear();
}

bool ServiceRegistry::addService(const char* typeName, QObject* instance, 
                                  int version, const QString& providerId)
{
    if (!instance) {
        qWarning() << "ServiceRegistry: Cannot register null service for" << typeName;
        return false;
    }

    QString name = QString::fromLatin1(typeName);
    
    QMutexLocker locker(&m_mutex);
    
    if (m_services.contains(name)) {
        qWarning() << "ServiceRegistry: Service already registered:" << name;
        return false;
    }

    ServiceEntry entry;
    entry.interfaceName = name;
    entry.version = version;
    entry.instance = instance;
    entry.providerId = providerId;

    m_services.insert(name, entry);
    
    locker.unlock();
    emit serviceAdded(name);
    
    qDebug() << "ServiceRegistry: Registered" << name << "v" << version 
             << "from" << providerId;
    return true;
}

QObject* ServiceRegistry::getService(const char* typeName, int minVersion)
{
    QString name = QString::fromLatin1(typeName);
    
    QMutexLocker locker(&m_mutex);
    
    auto it = m_services.find(name);
    if (it == m_services.end()) {
        return nullptr;
    }

    if (minVersion > 0 && it->version < minVersion) {
        qWarning() << "ServiceRegistry: Service" << name 
                   << "version" << it->version 
                   << "is below required" << minVersion;
        return nullptr;
    }

    return it->instance;
}

bool ServiceRegistry::hasService(const char* typeName, int minVersion) const
{
    QString name = QString::fromLatin1(typeName);
    
    QMutexLocker locker(&m_mutex);
    
    auto it = m_services.find(name);
    if (it == m_services.end()) {
        return false;
    }

    if (minVersion > 0 && it->version < minVersion) {
        return false;
    }

    return true;
}

int ServiceRegistry::serviceVersion(const char* typeName) const
{
    QString name = QString::fromLatin1(typeName);
    
    QMutexLocker locker(&m_mutex);
    
    auto it = m_services.find(name);
    if (it == m_services.end()) {
        return -1;
    }

    return it->version;
}

void ServiceRegistry::removeService(const char* typeName)
{
    QString name = QString::fromLatin1(typeName);
    
    QMutexLocker locker(&m_mutex);
    
    if (m_services.remove(name) > 0) {
        locker.unlock();
        emit serviceRemoved(name);
        qDebug() << "ServiceRegistry: Removed" << name;
    }
}

QStringList ServiceRegistry::registeredServices() const
{
    QMutexLocker locker(&m_mutex);
    return m_services.keys();
}

const ServiceEntry* ServiceRegistry::entry(const QString& interfaceName) const
{
    QMutexLocker locker(&m_mutex);
    
    auto it = m_services.find(interfaceName);
    if (it == m_services.end()) {
        return nullptr;
    }
    
    return &it.value();
}

} // namespace mpf
