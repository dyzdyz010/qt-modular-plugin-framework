#include "mpf/http/http_client.h"

#include <QJsonDocument>
#include <QNetworkRequest>
#include <QTimer>

namespace mpf::http {

HttpClient::HttpClient(QObject* parent)
    : QObject(parent) {}

QNetworkReply* HttpClient::get(const QUrl& url, RequestOptions options) {
    QNetworkRequest request = buildRequest(url, options);
    QNetworkReply* reply = m_manager.get(request);
    applyTimeout(reply, options.timeoutMs);
    return reply;
}

QNetworkReply* HttpClient::post(const QUrl& url,
                                const QByteArray& body,
                                const QString& contentType,
                                RequestOptions options) {
    QNetworkRequest request = buildRequest(url, options, contentType);
    QNetworkReply* reply = m_manager.post(request, body);
    applyTimeout(reply, options.timeoutMs);
    return reply;
}

QNetworkReply* HttpClient::postJson(const QUrl& url,
                                    const QJsonObject& body,
                                    RequestOptions options) {
    const QJsonDocument document(body);
    const QByteArray payload = document.toJson(QJsonDocument::Compact);
    return post(url, payload, QStringLiteral("application/json"), options);
}

QNetworkRequest HttpClient::buildRequest(const QUrl& url,
                                         const RequestOptions& options,
                                         const QString& contentType) {
    QNetworkRequest request(url);

    for (auto it = options.headers.cbegin(); it != options.headers.cend(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    if (!contentType.isEmpty()) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    }

    return request;
}

void HttpClient::applyTimeout(QNetworkReply* reply, int timeoutMs) {
    if (timeoutMs <= 0 || !reply) {
        return;
    }

    auto* timer = new QTimer(reply);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, reply, &QNetworkReply::abort);
    QObject::connect(reply, &QNetworkReply::finished, timer, &QTimer::stop);
    timer->start(timeoutMs);
}

} // namespace mpf::http
