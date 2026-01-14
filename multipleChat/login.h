#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

signals:
    void loginSuccess(QTcpSocket *socket);

private slots:
    void on_btnLogin_clicked();
    void on_btnCancel_clicked();
    void onConnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onReadyRead();

private:
    Ui::login *ui;
    QTcpSocket *m_socket;
};

#endif // LOGIN_H
