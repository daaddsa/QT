#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

class ServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit ServerWorker(QObject *parent = nullptr);
    virtual bool setSocketDescriptor(qintptr socketDescriptor);
    QString userName() const;
    void setUserName(const QString &user);
    void sendJson(const QJsonObject &json);

signals:
    void jsonReceived(const QJsonObject &jsonDoc);
    void disconnectedFromClient();
    void logMessage(const QString &msg);

public slots:
    void disconnectFromClient();

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket *m_serverSocket;
    QString m_userName;
};

#endif // SERVERWORKER_H
