#include "chatserver.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonArray>

ChatServer::ChatServer(QObject *parent)
    : QTcpServer(parent)
{
}

bool ChatServer::startServer(int port)
{
    if (!this->listen(QHostAddress::Any, port)) {
        emit logMessage(QString("启动失败: %1").arg(this->errorString()));
        return false;
    }
    emit logMessage(QString("服务已启动，正在监听端口: %1").arg(port));
    return true;
}

void ChatServer::stopServer()
{
    for (ServerWorker *worker : m_clients) {
        worker->disconnectFromClient();
    }
    m_clients.clear();
    this->close();
    emit logMessage("服务已停止监听.");
}

void ChatServer::broadcast(const QJsonObject &message, ServerWorker *exclude)
{
    for (ServerWorker *worker : m_clients) {
        if (worker == exclude) continue;
        worker->sendJson(message);
    }
}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    ServerWorker *worker = new ServerWorker(this);
    if (!worker->setSocketDescriptor(socketDescriptor)) {
        worker->deleteLater();
        return;
    }

    connect(worker, &ServerWorker::logMessage, this, &ChatServer::logMessage);
    
    connect(worker, &ServerWorker::jsonReceived, this, [this, worker](const QJsonObject &json){
        jsonReceived(json, worker);
    });
    
    connect(worker, &ServerWorker::disconnectedFromClient, this, [this, worker](){
        userDisconnected(worker);
    });

    m_clients.append(worker);
    emit logMessage(QString("新连接接入，Socket描述符: %1").arg(socketDescriptor));
}

void ChatServer::jsonReceived(const QJsonObject &jsonDoc, ServerWorker *sender)
{
    const QJsonValue typeVal = jsonDoc.value("type");
    if (typeVal.isNull() || !typeVal.isString()) return;

    const QString type = typeVal.toString();

    if (type == "login") {
        const QString username = jsonDoc.value("username").toString();
        if (username.isEmpty()) {
             QJsonObject response;
             response["type"] = "login_response";
             response["success"] = false;
             response["message"] = "用户名不能为空";
             sender->sendJson(response);
             return;
        }

        // 检查用户名是否重复
        for (ServerWorker *worker : m_clients) {
            if (worker != sender && worker->userName() == username) {
                QJsonObject response;
                response["type"] = "login_response";
                response["success"] = false;
                response["message"] = "用户名已存在";
                sender->sendJson(response);
                return;
            }
        }

        // 登录成功
        sender->setUserName(username);
        QJsonObject response;
        response["type"] = "login_response";
        response["success"] = true;
        sender->sendJson(response);

        // 发送用户列表
        QJsonObject userListObj;
        userListObj["type"] = "userlist";
        QJsonArray userListArray;
        for (ServerWorker *worker : m_clients) {
             if (!worker->userName().isEmpty())
                userListArray.append(worker->userName());
        }
        userListObj["userlist"] = userListArray;
        sender->sendJson(userListObj);

        // 广播新用户上线
        QJsonObject newUserObj;
        newUserObj["type"] = "newuser";
        newUserObj["username"] = username;
        broadcast(newUserObj, sender);
        
        emit logMessage(QString("用户登录成功: %1").arg(username));
    } else if (type == "message") {
        const QString text = jsonDoc.value("text").toString();
        if (text.isEmpty()) return;

        QJsonObject msgObj;
        msgObj["type"] = "message";
        msgObj["sender"] = sender->userName();
        msgObj["text"] = text;
        
        broadcast(msgObj);
    }
}

void ChatServer::userDisconnected(ServerWorker *sender)
{
    m_clients.removeAll(sender);
    QString userName = sender->userName();
    if (!userName.isEmpty()) {
        QJsonObject disconnectedObj;
        disconnectedObj["type"] = "userdisconnected";
        disconnectedObj["username"] = userName;
        broadcast(disconnectedObj);
        emit logMessage(QString("用户断开连接: %1").arg(userName));
    } else {
        emit logMessage("未知用户断开连接");
    }
    sender->deleteLater();
}
