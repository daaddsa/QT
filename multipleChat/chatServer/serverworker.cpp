#include "serverworker.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDataStream>

ServerWorker::ServerWorker(QObject *parent)
    : QObject(parent)
    , m_serverSocket(new QTcpSocket(this))
{
    connect(m_serverSocket, &QTcpSocket::readyRead, this, &ServerWorker::onReadyRead);
    connect(m_serverSocket, &QTcpSocket::disconnected, this, &ServerWorker::onDisconnected);
}

bool ServerWorker::setSocketDescriptor(qintptr socketDescriptor)
{
    return m_serverSocket->setSocketDescriptor(socketDescriptor);
}

QString ServerWorker::userName() const
{
    return m_userName;
}

void ServerWorker::setUserName(const QString &user)
{
    m_userName = user;
}

void ServerWorker::sendJson(const QJsonObject &json)
{
    const QByteArray jsonData = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_serverSocket->write(jsonData + "\n");
}

void ServerWorker::disconnectFromClient()
{
    m_serverSocket->disconnectFromHost();
}

void ServerWorker::onReadyRead()
{
    while (m_serverSocket->canReadLine()) {
        QByteArray jsonData = m_serverSocket->readLine().trimmed();
        if (jsonData.isEmpty()) continue;

        QJsonParseError parseError;
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

        if (parseError.error == QJsonParseError::NoError) {
            if (jsonDoc.isObject()) {
                emit jsonReceived(jsonDoc.object());
            }
        } else {
            emit logMessage(QString("JSON Parse Error: %1").arg(parseError.errorString()));
        }
    }
}

void ServerWorker::onDisconnected()
{
    emit disconnectedFromClient();
}
