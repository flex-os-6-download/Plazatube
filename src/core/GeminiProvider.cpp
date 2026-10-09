#include "GeminiProvider.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QTimer>

GeminiProvider::GeminiProvider(QObject *parent)
    : QObject(parent), m_networkManager(new QNetworkAccessManager(this)) {
    QSettings settings("FlexOS", "Gemini");
    m_apiKey = settings.value("apiKey", "").toString();
}

QVariantList GeminiProvider::conversationHistory() const {
    QVariantList list;
    for (const auto &item : m_history) {
        list.append(item);
    }
    return list;
}

void GeminiProvider::setApiKey(const QString &key) {
    if (m_apiKey != key.trimmed()) {
        m_apiKey = key.trimmed();
        QSettings settings("FlexOS", "Gemini");
        settings.setValue("apiKey", m_apiKey);
        emit configChanged();
    }
}

void GeminiProvider::sendPrompt(const QString &prompt) {
    if (prompt.trimmed().isEmpty()) return;

    if (m_isBusy) {
        cancelRequest();
    }

    m_isBusy = true;
    emit busyChanged();

    // Record user prompt in conversation history
    QVariantMap userEntry;
    userEntry["role"] = "user";
    userEntry["text"] = prompt;
    m_history.append(userEntry);
    emit historyChanged();

    if (m_apiKey.isEmpty()) {
        // Safe offline simulated assistant response when credentials are not configured
        QTimer::singleShot(600, this, [this, prompt]() {
            m_isBusy = false;
            emit busyChanged();

            m_lastResponse = QString("FlexOS Assistant (Offline Mode): I received your query '%1'. "
                                     "Please configure your Google Gemini API key in FlexOS Settings to enable online AI queries.")
                                 .arg(prompt);

            QVariantMap assistantEntry;
            assistantEntry["role"] = "assistant";
            assistantEntry["text"] = m_lastResponse;
            m_history.append(assistantEntry);

            emit responseReceived(m_lastResponse);
            emit historyChanged();
        });
        return;
    }

    // Official Google Gemini REST API v1beta URL structure
    QString urlStr = QString("https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent?key=%1").arg(m_apiKey);
    QNetworkRequest request((QUrl(urlStr)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject textObj;
    textObj["text"] = prompt;
    QJsonObject partsObj;
    QJsonArray partsArray;
    partsArray.append(textObj);
    QJsonObject contentObj;
    contentObj["parts"] = partsArray;
    QJsonArray contentsArray;
    contentsArray.append(contentObj);
    QJsonObject bodyObj;
    bodyObj["contents"] = contentsArray;

    QByteArray bodyData = QJsonDocument(bodyObj).toJson();

    m_currentReply = m_networkManager->post(request, bodyData);
    connect(m_currentReply, &QNetworkReply::finished, this, [this]() {
        if (m_currentReply) {
            handleNetworkReply(m_currentReply);
            m_currentReply->deleteLater();
            m_currentReply = nullptr;
        }
    });
}

void GeminiProvider::handleNetworkReply(QNetworkReply *reply) {
    m_isBusy = false;
    emit busyChanged();

    if (reply->error() != QNetworkReply::NoError) {
        QString err = QString("Gemini API Error: %1").arg(reply->errorString());
        emit errorOccurred(err);
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject root = doc.object();

    QString resultText;
    if (root.contains("candidates") && root["candidates"].isArray()) {
        QJsonArray candidates = root["candidates"].toArray();
        if (!candidates.isEmpty()) {
            QJsonObject firstCand = candidates.first().toObject();
            if (firstCand.contains("content")) {
                QJsonObject content = firstCand["content"].toObject();
                if (content.contains("parts")) {
                    QJsonArray parts = content["parts"].toArray();
                    for (const auto &p : parts) {
                        resultText += p.toObject()["text"].toString();
                    }
                }
            }
        }
    }

    if (resultText.isEmpty()) {
        resultText = "Received empty response from Gemini API.";
    }

    m_lastResponse = resultText;

    QVariantMap assistantEntry;
    assistantEntry["role"] = "assistant";
    assistantEntry["text"] = m_lastResponse;
    m_history.append(assistantEntry);

    emit responseReceived(m_lastResponse);
    emit historyChanged();
}

void GeminiProvider::cancelRequest() {
    if (m_currentReply) {
        m_currentReply->abort();
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
    }
    if (m_isBusy) {
        m_isBusy = false;
        emit busyChanged();
    }
}

void GeminiProvider::clearHistory() {
    m_history.clear();
    m_lastResponse.clear();
    emit historyChanged();
}
