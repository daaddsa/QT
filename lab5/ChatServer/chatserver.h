#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QVector>
#include "serverworker.h"

class chatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit chatServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void logMessage(const QString &msg);

public slots:
    void stopServer();

private slots:
    void broadcast(const QJsonObject &message, serverWorker *exclude);
    void jsonReceived(serverWorker *sender, const QJsonObject &doc);
    void userDisconnected(serverWorker *sender);

private:
    QVector<serverWorker *> m_clients;
};

#endif // CHATSERVER_H
