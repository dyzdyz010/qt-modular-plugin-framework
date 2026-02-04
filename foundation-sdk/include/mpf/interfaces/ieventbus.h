#pragma once

#include <QString>
#include <QStringList>
#include <QVariantMap>

namespace mpf {

/**
 * @brief Event data payload
 */
struct Event
{
    QString topic;              ///< Topic/channel name (e.g., "orders/created")
    QString senderId;           ///< Plugin ID of sender
    QVariantMap data;           ///< Event payload
    qint64 timestamp = 0;       ///< Unix timestamp in milliseconds
    QString correlationId;      ///< Optional: for request/response patterns

    QVariantMap toVariantMap() const
    {
        return {
            {"topic", topic},
            {"senderId", senderId},
            {"data", data},
            {"timestamp", timestamp},
            {"correlationId", correlationId}
        };
    }

    static Event fromVariantMap(const QVariantMap& map)
    {
        Event e;
        e.topic = map.value("topic").toString();
        e.senderId = map.value("senderId").toString();
        e.data = map.value("data").toMap();
        e.timestamp = map.value("timestamp").toLongLong();
        e.correlationId = map.value("correlationId").toString();
        return e;
    }
};

/**
 * @brief Subscription options
 */
struct SubscriptionOptions
{
    bool async = true;              ///< Async delivery (default) vs synchronous
    int priority = 0;               ///< Higher priority = called first
    bool receiveOwnEvents = false;  ///< Receive events from same sender

    QVariantMap toVariantMap() const
    {
        return {
            {"async", async},
            {"priority", priority},
            {"receiveOwnEvents", receiveOwnEvents}
        };
    }
};

/**
 * @brief Topic statistics
 */
struct TopicStats
{
    QString topic;
    int subscriberCount = 0;
    qint64 eventCount = 0;      ///< Total events published
    qint64 lastEventTime = 0;   ///< Last event timestamp

    QVariantMap toVariantMap() const
    {
        return {
            {"topic", topic},
            {"subscriberCount", subscriberCount},
            {"eventCount", eventCount},
            {"lastEventTime", lastEventTime}
        };
    }
};

/**
 * @brief Event bus interface for publish/subscribe messaging
 *
 * Provides loose-coupled communication between plugins.
 * Supports wildcard topic matching:
 *   - "*" matches a single level (e.g., "orders/*" matches "orders/created")
 *   - "**" matches multiple levels (e.g., "orders/**" matches "orders/items/added")
 */
class IEventBus
{
public:
    virtual ~IEventBus() = default;

    // ===== Publishing =====

    /**
     * @brief Publish an event to a topic (async delivery)
     * @param topic Topic name (e.g., "orders/created")
     * @param data Event payload
     * @param senderId Publisher plugin ID
     * @return Number of subscribers notified
     */
    virtual int publish(const QString& topic,
                        const QVariantMap& data,
                        const QString& senderId = {}) = 0;

    /**
     * @brief Publish an event synchronously (blocks until all handlers complete)
     * @param topic Topic name
     * @param data Event payload
     * @param senderId Publisher plugin ID
     * @return Number of subscribers notified
     */
    virtual int publishSync(const QString& topic,
                            const QVariantMap& data,
                            const QString& senderId = {}) = 0;

    // ===== Subscribing =====

    /**
     * @brief Subscribe to a topic pattern
     * @param pattern Topic pattern (supports wildcards: "*" single level, "**" multi-level)
     * @param subscriberId Subscriber plugin ID
     * @param options Subscription options
     * @return Subscription ID (used for unsubscribe)
     */
    virtual QString subscribe(const QString& pattern,
                              const QString& subscriberId,
                              const SubscriptionOptions& options = {}) = 0;

    /**
     * @brief Unsubscribe by subscription ID
     * @param subscriptionId ID returned from subscribe()
     * @return true if subscription was found and removed
     */
    virtual bool unsubscribe(const QString& subscriptionId) = 0;

    /**
     * @brief Unsubscribe all subscriptions for a plugin
     * @param subscriberId Plugin ID to unsubscribe
     */
    virtual void unsubscribeAll(const QString& subscriberId) = 0;

    // ===== Query Methods =====

    /**
     * @brief Get subscriber count for a topic (counts matching patterns)
     * @param topic Exact topic name
     * @return Number of subscribers that would receive events on this topic
     */
    virtual int subscriberCount(const QString& topic) const = 0;

    /**
     * @brief Get all active subscription patterns
     * @return List of patterns with active subscriptions
     */
    virtual QStringList activeTopics() const = 0;

    /**
     * @brief Get statistics for a topic
     * @param topic Topic name
     * @return Topic statistics
     */
    virtual TopicStats topicStats(const QString& topic) const = 0;

    /**
     * @brief Get all subscription IDs for a plugin
     * @param subscriberId Plugin ID
     * @return List of subscription IDs
     */
    virtual QStringList subscriptionsFor(const QString& subscriberId) const = 0;

    /**
     * @brief Check if a topic matches a pattern
     * @param topic Actual topic (e.g., "orders/created")
     * @param pattern Pattern to match (e.g., "orders/*")
     * @return true if topic matches the pattern
     */
    virtual bool matchesTopic(const QString& topic, const QString& pattern) const = 0;

    /**
     * @brief API version for compatibility checking
     */
    static constexpr int apiVersion() { return 1; }
};

} // namespace mpf
