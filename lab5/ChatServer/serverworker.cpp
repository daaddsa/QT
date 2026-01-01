#include "serverworker.h"
#include <QJsonDocument>
#include <QJsonObject>

serverWorker::serverWorker(QObject *parent)
    : QObject(parent)
    , m_serverSocket(new QTcpSocket(this))
{
    connect(m_serverSocket, &QTcpSocket::readyRead, this, &serverWorker::receiveJson);
    connect(m_serverSocket, &QTcpSocket::disconnected, this, &serverWorker::disconnectedFromClient);
}

bool serverWorker::setSocketDescriptor(qintptr socketDescriptor)
{
    return m_serverSocket->setSocketDescriptor(socketDescriptor);
}

QString serverWorker::userName() const
{
    return m_userName;
}

void serverWorker::setUserName(const QString &userName)
{
    m_userName = userName;
}

void serverWorker::sendJson(const QJsonObject &jsonObject)
{
    const QByteArray jsonData = QJsonDocument(jsonObject).toJson(QJsonDocument::Compact);
    emit logMessage(QString("Sending to %1 - %2").arg(m_userName.isEmpty() ? "unknown" : m_userName, QString::fromUtf8(jsonData)));
    m_serverSocket->write(jsonData);
    m_serverSocket->write("\n");
    m_serverSocket->flush();
}

void serverWorker::disconnectFromClient()
{
    m_serverSocket->disconnectFromHost();
}

void serverWorker::receiveJson()
{
    while (m_serverSocket->canReadLine()) {
        QByteArray jsonData = m_serverSocket->readLine().trimmed();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject obj = doc.object();
        if (!obj.isEmpty()) {
            emit jsonReceived(obj);
        } else {
            if (!jsonData.isEmpty())
                emit logMessage("JSON parse error: " + QString::fromUtf8(jsonData));
        }
    }
}
