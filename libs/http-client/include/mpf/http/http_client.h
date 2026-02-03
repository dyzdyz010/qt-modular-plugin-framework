#pragma once

#include <QObject>
#include <QMap>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

namespace mpf::http {

class HttpClient : public QObject {
    Q_OBJECT

public:
    struct RequestOptions {
        QMap<QString, QString> headers;
        int timeoutMs;

        RequestOptions()
            : timeoutMs(0) {}
    };

    explicit HttpClient(QObject* parent = nullptr);

    QNetworkReply* get(const QUrl& url, RequestOptions options = RequestOptions());
    QNetworkReply* post(const QUrl& url,
                        const QByteArray& body,
                        const QString& contentType,
                        RequestOptions options = RequestOptions());
    QNetworkReply* postJson(const QUrl& url,
                            const QJsonObject& body,
                            RequestOptions options = RequestOptions());

private:
    QNetworkRequest buildRequest(const QUrl& url,
                                 const RequestOptions& options,
                                 const QString& contentType = QString());
    void applyTimeout(QNetworkReply* reply, int timeoutMs);

    QNetworkAccessManager m_manager;
};

} // namespace mpf::http
