#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class signUp;
}

class signUp : public QWidget
{
    Q_OBJECT

public:
    explicit signUp(QWidget *parent = nullptr);
    ~signUp();
    void setServerEndpoint(const QString &ip, int port);

signals:
    void backToLogin();
    void signUpSuccess(const QString &username);

private slots:
    void onRegisterClicked();
    void onBackClicked();
    void onConnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onReadyRead();

private:
    Ui::signUp *ui;
    QTcpSocket *m_socket;
    QString m_serverIp;
    int m_serverPort;
    QString m_pendingAccount;
    QString m_pendingNickname;
};

#endif // SIGNUP_H
