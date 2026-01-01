#include "chatserver.h"
#include "serverworker.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

chatServer::chatServer(QObject *parent)
    : QTcpServer(parent)
{}

void chatServer::incomingConnection(qintptr socketDescriptor)
{
    serverWorker *worker = new serverWorker(this);
    if (!worker->setSocketDescriptor(socketDescriptor)) {
        worker->deleteLater();
        return;
    }

    connect(worker, &serverWorker::disconnectedFromClient, this, [this, worker]() {
        userDisconnected(worker);
    });

    connect(worker, &serverWorker::jsonReceived, this, [this, worker](const QJsonObject &doc) {
        jsonReceived(worker, doc);
    });

    connect(worker, &serverWorker::logMessage, this, &chatServer::logMessage);

    m_clients.append(worker);
    emit logMessage("新的用户连接上了");
}

void chatServer::broadcast(const QJsonObject &message, serverWorker *exclude)
{
    for (serverWorker *worker : m_clients) {
        if (worker == exclude)
            continue;
        worker->sendJson(message);
    }
}

void chatServer::jsonReceived(serverWorker *sender, const QJsonObject &doc)
{
    emit logMessage(QString::fromUtf8(QJsonDocument(doc).toJson(QJsonDocument::Compact)));

    const QJsonValue typeVal = doc.value("type");
    if (typeVal.isNull() || !typeVal.isString())
        return;

    if (typeVal.toString().compare("login", Qt::CaseInsensitive) == 0) {
        const QJsonValue usernameVal = doc.value("username");
        if (usernameVal.isNull() || !usernameVal.isString())
            return;

        QString newUserName = usernameVal.toString();
        // Ideally check for duplicate names here
        sender->setUserName(newUserName);
        emit logMessage(QString("User logged in: %1. Total clients: %2").arg(newUserName).arg(m_clients.size()));

        // Notify others about new user
        QJsonObject newUserMessage;
        newUserMessage["type"] = "newuser";
        newUserMessage["username"] = newUserName;
        broadcast(newUserMessage, sender);

        // Send user list to new user
        QJsonArray userList;
        for (serverWorker *worker : m_clients) {
            if (worker->userName().isEmpty()) continue;
            userList.append(worker->userName());
        }
        
        QJsonObject userListMessage;
        userListMessage["type"] = "userlist";
        userListMessage["userlist"] = userList;
        sender->sendJson(userListMessage);
        
        emit logMessage(QString("Sent user list to %1: %2 users").arg(newUserName).arg(userList.size()));

    } else if (typeVal.toString().compare("message", Qt::CaseInsensitive) == 0) {
        const QJsonValue textVal = doc.value("text");
        if (textVal.isNull() || !textVal.isString())
            return;

        QJsonObject message;
        message["type"] = "message";
        message["text"] = textVal.toString();
        message["sender"] = sender->userName();

        broadcast(message, nullptr); // Broadcast to all, including sender
    }
}

void chatServer::userDisconnected(serverWorker *sender)
{
    m_clients.removeAll(sender);
    QString userName = sender->userName();
    if (!userName.isEmpty()) {
        QJsonObject disconnectedMessage;
        disconnectedMessage["type"] = "userdisconnected";
        disconnectedMessage["username"] = userName;
        broadcast(disconnectedMessage, nullptr);
        emit logMessage(userName + " 断开连接");
    }
    sender->deleteLater();
}

void chatServer::stopServer()
{
    for (serverWorker *worker : m_clients) {
        worker->disconnectFromClient();
    }
    close();
}
