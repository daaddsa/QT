#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QList>
#include "serverworker.h"

class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
    bool startServer(int port);
    void stopServer();
    void broadcast(const QJsonObject &message, ServerWorker *exclude = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void logMessage(const QString &msg);

private slots:
    void jsonReceived(const QJsonObject &jsonDoc, ServerWorker *sender);
    void userDisconnected(ServerWorker *sender);

private:
    QList<ServerWorker *> m_clients;
};

#endif // CHATSERVER_H
