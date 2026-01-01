#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonObject>

class chatClient : public QObject
{
    Q_OBJECT
public:
    explicit chatClient(QObject *parent = nullptr);
    void connectToServer(const QHostAddress &address, quint16 port);
    void sendMessage(const QString &text, const QString &type = "message");
    void disconnectFromHost();

signals:
    void connected();
    void messageReceived(const QString &sender, const QString &text);
    void jsonReceived(const QJsonObject &docObj);

private slots:
    void onReadyRead();

private:
    QTcpSocket *m_clientSocket;
};

#endif // CHATCLIENT_H
