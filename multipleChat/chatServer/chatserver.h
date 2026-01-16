#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QList>
#include <QHash>
#include <QSqlDatabase>
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
    bool ensureDatabaseOpen();
    QString resolveDatabasePath() const;
    void ensureSchema();
    int findUserIdByNickname(const QString &nickname);
    int ensureDirectConversation(int userId1, int userId2);
    int ensureGroupConversation();
    void deliverUndeliveredMessages(ServerWorker *recipient);

    struct RegisteredUser {
        QString nickname;
        QString password;
    };
    QList<ServerWorker *> m_clients;
    QHash<QString, RegisteredUser> m_registeredUsers;
    QSqlDatabase m_db;
};

#endif // CHATSERVER_H
