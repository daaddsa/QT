#include "chatserver.h"

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
    this->close();
    emit logMessage("服务已停止监听.");
}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    // 当有新用户连接时触发
    emit logMessage(QString("新连接接入，Socket描述符: %1").arg(socketDescriptor));

    // TODO: 创建 ServerWorker 来处理该连接
}
