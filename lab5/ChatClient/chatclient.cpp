#include "chatclient.h"
#include <QJsonDocument>
#include <QJsonObject>

chatClient::chatClient(QObject *parent)
    : QObject{parent}
{
    m_clientSocket = new QTcpSocket(this);
    connect(m_clientSocket, &QTcpSocket::connected, this, &chatClient::connected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &chatClient::onReadyRead);
}

void chatClient::onReadyRead()
{
    while (m_clientSocket->canReadLine()) {
        QByteArray jsonData = m_clientSocket->readLine().trimmed();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject obj = doc.object();
        
        if (!obj.isEmpty()) {
            emit jsonReceived(obj);
        } else {
            // Report parse error or empty data to UI for debugging
            QJsonObject errorObj;
            errorObj["type"] = "system";
            errorObj["message"] = "JSON Parse Error or Empty: " + QString::fromUtf8(jsonData);
            emit jsonReceived(errorObj);
        }
    }
}

void chatClient::sendMessage(const QString &text, const QString &type)
{
    if (m_clientSocket->state() != QAbstractSocket::ConnectedState)
        return;

    QJsonObject message;
    message["type"] = type;
    if (type == "login")
        message["username"] = text;
    else
        message["text"] = text;

    m_clientSocket->write(QJsonDocument(message).toJson(QJsonDocument::Compact));
    m_clientSocket->write("\n");
}

void chatClient::connectToServer(const QHostAddress &address, quint16 port)
{
    m_clientSocket->connectToHost(address, port);
}

void chatClient::disconnectFromHost()
{
    m_clientSocket->disconnectFromHost();
}
