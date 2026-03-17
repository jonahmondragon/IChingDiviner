#include "aimanager.h"

#include <QNetworkRequest>
#include <QUrlQuery>
#include <QDebug>
#include "constants.h"
#include<QMessageBox>
#include<QFile>
#include<QDir>

MistralAPI::MistralAPI(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &MistralAPI::handleNetworkReply);

    activeModelLoaded = loadActiveModel();

}

MistralAPI::~MistralAPI()
{
    // Clean up is handled by Qt's parent-child system
}

void MistralAPI::generateReading(const QString& prompt)
{
    if (!activeModelLoaded) {
        loadActiveModel();
        if (!activeModelLoaded) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("AI Model Not Configured");
            msgBox.setText("No active AI model or API key found.");
            msgBox.setInformativeText("Please configure an AI model in Settings → Configure AI Models.\n\n"
                                      "Compatible providers: Mistral, OpenAI, Groq, Ollama (local), and any OpenAI-compatible API.");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.addButton(QMessageBox::Ok);
            msgBox.exec();

            return;
        }
    }

    QUrl url(m_apiEndpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(m_apiKey).toUtf8());

    QJsonDocument requestData = createRequestJson(prompt);

    networkManager->post(request, requestData.toJson());
}




void MistralAPI::handleNetworkReply(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonResponse.object();

        if (jsonObject.contains("choices") && jsonObject["choices"].isArray()) {
            QJsonArray choices = jsonObject["choices"].toArray();
            if (!choices.isEmpty() && choices[0].isObject()) {
                QJsonObject firstChoice = choices[0].toObject();
                if (firstChoice.contains("message") && firstChoice["message"].isObject()) {
                    QJsonObject message = firstChoice["message"].toObject();
                    if (message.contains("content") && message["content"].isString()) {
                        QString reading = message["content"].toString();
                        emit readingReady(reading);
                        reply->deleteLater();
                        return;
                    }
                }
            }
        }

        emit errorOccurred("Invalid response format from AI");
    } else {
        QString errorString = reply->errorString();
        QByteArray responseData = reply->readAll();

        // Try to parse error message from API if available
        QString detailedError = errorString;
        QJsonDocument errorJson = QJsonDocument::fromJson(responseData);
        if (!errorJson.isNull() && errorJson.isObject()) {
            QJsonObject errorObj = errorJson.object();
            if (errorObj.contains("error") && errorObj["error"].isObject()) {
                QJsonObject error = errorObj["error"].toObject();
                if (error.contains("message") && error["message"].isString()) {
                    detailedError = error["message"].toString();
                }
            }
        }

        emit errorOccurred("Network error: " + detailedError);
    }

    reply->deleteLater();
}




void MistralAPI::setQuery(const QString &newQuery)
{
    query = newQuery;
}


bool MistralAPI::loadActiveModel()
{
    QSettings settings;
    settings.beginGroup("Models");

    // Get the active model name
    QString activeModelName = settings.value("ActiveModel").toString();
    if (activeModelName.isEmpty()) {
        m_lastError = "No active model selected";
        return false;
    }

    // Load only the active model's settings
    settings.beginGroup(activeModelName);
    m_apiEndpoint = settings.value("endpoint").toString();
    m_apiKey = settings.value("apiKey").toString();
    m_model = settings.value("modelName").toString();
    m_temperature = settings.value("temperature", 0.7).toDouble();
    m_maxTokens = settings.value("maxTokens", 4096).toInt();




    settings.endGroup();
    settings.endGroup();

    return !m_apiEndpoint.isEmpty() && !m_model.isEmpty();
}



///////////////////


QJsonDocument MistralAPI::createRequestJson(const QString& prompt)
{
    QJsonObject systemMessage;
    systemMessage["role"] = "system";
    if(query.isEmpty()) {
        systemMessage["content"] = "You are a skilled I-Ching interpreter with deep knowledge of the hexagrams, their meanings, and the philosophy of change. "
                                   "Provide insightful, thoughtful readings that connect the hexagrams to the querent's situation. "
                                   "Explain the symbolism of the hexagrams, the meaning of changing lines, and how the situation may evolve. "
                                   "Be wise but practical, offering both spiritual insights and actionable advice based on the I-Ching's wisdom.";
    } else {
        systemMessage["content"] = QString("You are a skilled I-Ching interpreter with deep knowledge of the hexagrams, their meanings, and the philosophy of change. "
                                           "Focus primarily and directly on the querent's specific question:\n"
                                           "**Question:** '%1'\n"
                                           "Provide a reading that directly addresses their concerns while interpreting the hexagrams in that context. "
                                           "Explain the source hexagram, any changing lines, and the resulting hexagram. "
                                           "Offer practical insights based on the I-Ching's wisdom.").arg(query);
    }

    QJsonObject userMessage;
    userMessage["role"] = "user";
    userMessage["content"] = prompt;

    QJsonArray messages;
    messages.append(systemMessage);
    messages.append(userMessage);

    QJsonObject requestObject;
    requestObject["model"] = m_model;
    requestObject["messages"] = messages;
    requestObject["temperature"] = m_temperature;
    requestObject["max_tokens"] = m_maxTokens;

    return QJsonDocument(requestObject);
}

