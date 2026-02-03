#pragma once

#include <mpf/interfaces/iplugin.h>
#include <QObject>

namespace orders {

class OrdersService;

/**
 * @brief Orders plugin implementation
 * 
 * Demonstrates a complete business plugin with:
 * - Service registration
 * - QML module
 * - Route registration
 * - Settings usage
 */
class OrdersPlugin : public QObject, public mpf::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID MPF_IPlugin_iid FILE "../orders_plugin.json")
    Q_INTERFACES(mpf::IPlugin)

public:
    explicit OrdersPlugin(QObject* parent = nullptr);
    ~OrdersPlugin() override;

    // IPlugin interface
    bool initialize(mpf::ServiceRegistry* registry) override;
    bool start() override;
    void stop() override;
    QJsonObject metadata() const override;
    QString qmlModuleUri() const override { return "YourCo.Orders"; }
    QString entryQml() const override { return QString(); }

private:
    void registerRoutes();
    void registerQmlTypes();

    mpf::ServiceRegistry* m_registry = nullptr;
    std::unique_ptr<OrdersService> m_ordersService;
};

} // namespace orders
