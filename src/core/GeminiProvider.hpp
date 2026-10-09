#ifndef GEMINIPROVIDER_HPP
#define GEMINIPROVIDER_HPP

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class GeminiProvider : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isConfigured READ isConfigured NOTIFY configChanged)
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(QString lastResponse READ lastResponse NOTIFY responseReceived)
    Q_PROPERTY(QVariantList conversationHistory READ conversationHistory NOTIFY historyChanged)

public:
    explicit GeminiProvider(QObject *parent = nullptr);

    bool isConfigured() const { return !m_apiKey.isEmpty(); }
    bool isBusy() const { return m_isBusy; }
    QString lastResponse() const { return m_lastResponse; }
    QVariantList conversationHistory() const;

    Q_INVOKABLE void setApiKey(const QString &key);
    Q_INVOKABLE void sendPrompt(const QString &prompt);
    Q_INVOKABLE void cancelRequest();
    Q_INVOKABLE void clearHistory();

signals:
    void configChanged();
    void busyChanged();
    void responseReceived(const QString &response);
    void errorOccurred(const QString &errorMessage);
    void historyChanged();

private slots:
    void handleNetworkReply(QNetworkReply *reply);

private:
    QString m_apiKey;
    bool m_isBusy{false};
    QString m_lastResponse;
    QList<QVariantMap> m_history;
    QNetworkAccessManager *m_networkManager{nullptr};
    QNetworkReply *m_currentReply{nullptr};
};

#endif // GEMINIPROVIDER_HPP
