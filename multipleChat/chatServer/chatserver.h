#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QList>

class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
    bool startServer(int port);
    void stopServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void logMessage(const QString &msg);

private:
    // TODO: 这里将来会存放 ServerWorker 列表
    // QList<ServerWorker *> m_clients;
};

#endif // CHATSERVER_H
