#pragma once

#include <QAbstractListModel>

namespace orders {

class OrdersService;

/**
 * @brief List model for orders
 * 
 * Exposes orders to QML ListView/Repeater.
 */
class OrderModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(QString filterStatus READ filterStatus WRITE setFilterStatus NOTIFY filterStatusChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        CustomerNameRole,
        ProductNameRole,
        QuantityRole,
        PriceRole,
        StatusRole,
        CreatedAtRole,
        UpdatedAtRole,
        TotalRole
    };

    explicit OrderModel(OrdersService* service, QObject* parent = nullptr);
    ~OrderModel() override;

    // QAbstractListModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Filter
    QString filterStatus() const { return m_filterStatus; }
    void setFilterStatus(const QString& status);

    // Actions
    Q_INVOKABLE void refresh();
    Q_INVOKABLE QVariantMap get(int index) const;

signals:
    void countChanged();
    void filterStatusChanged();

private slots:
    void onOrdersChanged();

private:
    void updateFilteredOrders();

    OrdersService* m_service;
    QVariantList m_filteredOrders;
    QString m_filterStatus;
};

} // namespace orders
