#pragma once

#include <QString>
#include <typeinfo>

class QObject;

namespace mpf {

/**
 * @brief Forward declaration of ServiceRegistry
 * 
 * The actual implementation is in the host application.
 * Plugins receive a pointer to ServiceRegistry via IPlugin::initialize().
 */
class ServiceRegistry
{
public:
    virtual ~ServiceRegistry() = default;

    /**
     * @brief Get a service by interface type
     * @tparam T Interface type
     * @param minVersion Minimum required version (0 = any)
     * @return Service instance or nullptr if not found
     */
    template<typename T>
    T* get(int minVersion = 0)
    {
        QObject* obj = getService(typeid(T).name(), minVersion);
        return dynamic_cast<T*>(obj);
    }

    /**
     * @brief Register a service implementation
     * @tparam T Interface type
     * @param instance Service instance
     * @param version API version
     * @param providerId Plugin ID providing this service
     * @return true if registration succeeded
     */
    template<typename T>
    bool add(T* instance, int version = 1, const QString& providerId = {})
    {
        QObject* obj = dynamic_cast<QObject*>(instance);
        if (!obj) {
            obj = reinterpret_cast<QObject*>(instance);
        }
        return addService(typeid(T).name(), obj, version, providerId);
    }

    /**
     * @brief Check if a service is available
     */
    template<typename T>
    bool has(int minVersion = 0) const
    {
        return hasService(typeid(T).name(), minVersion);
    }

protected:
    virtual QObject* getService(const char* typeName, int minVersion) = 0;
    virtual bool addService(const char* typeName, QObject* instance, int version, const QString& providerId) = 0;
    virtual bool hasService(const char* typeName, int minVersion) const = 0;
};

} // namespace mpf
