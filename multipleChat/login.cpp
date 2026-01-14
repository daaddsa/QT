#include "login.h"
#include "ui_login.h"
#include "signup.h"
#include <QMessageBox>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
    , m_socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    connect(m_socket, &QTcpSocket::connected, this, &login::onConnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &login::onErrorOccurred);
    connect(m_socket, &QTcpSocket::readyRead, this, &login::onReadyRead);
}

login::~login()
{
    delete ui;
}

void login::on_btnLogin_clicked()
{
    QString ip = ui->txtServerIP->text();
    QString portStr = ui->txtPort->text();
    QString username = ui->txtUsername->text();

    if (ip.isEmpty() || portStr.isEmpty() || username.isEmpty()) {
        QMessageBox::warning(this, "提示", "请完整填写服务器信息和用户名");
        return;
    }

    bool ok;
    int port = portStr.toInt(&ok);
    if (!ok || port <= 0 || port > 65535) {
        QMessageBox::warning(this, "错误", "端口号无效");
        return;
    }

    ui->btnLogin->setEnabled(false);
    ui->btnLogin->setText("连接中...");

    m_socket->abort();
    m_socket->connectToHost(ip, port);
}

void login::on_btnCancel_clicked()
{
    this->close();
}

void login::on_btnToRegister_clicked()
{
    const QString ip = ui->txtServerIP->text().trimmed();
    bool ok = false;
    const int port = ui->txtPort->text().toInt(&ok);
    if (ip.isEmpty() || !ok || port <= 0 || port > 65535) {
        QMessageBox::warning(this, "提示", "请先填写有效的服务器IP和端口");
        return;
    }

    auto *signUpWindow = new signUp(nullptr);
    signUpWindow->setAttribute(Qt::WA_DeleteOnClose);
    signUpWindow->setServerEndpoint(ip, port);

    connect(signUpWindow, &signUp::backToLogin, this, [this]() {
        this->show();
    });
    connect(signUpWindow, &signUp::signUpSuccess, this, [this](const QString &username) {
        ui->txtUsername->setText(username);
        this->show();
    });
    connect(signUpWindow, &QObject::destroyed, this, [this]() {
        this->show();
    });

    this->hide();
    signUpWindow->show();
}

void login::onConnected()
{
    QJsonObject json;
    json["type"] = "login";
    json["username"] = ui->txtUsername->text();

    QJsonDocument doc(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    m_socket->write(data + "\n");
    ui->btnLogin->setText("验证中...");
}

void login::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    QMessageBox::critical(this, "连接失败", m_socket->errorString());
    ui->btnLogin->setEnabled(true);
    ui->btnLogin->setText("登 录");
}

void login::onReadyRead()
{
    while (m_socket->canReadLine()) {
        QByteArray data = m_socket->readLine().trimmed();
        if (data.isEmpty()) continue;

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            continue;
        }

        QJsonObject root = doc.object();
        QString type = root["type"].toString();

        if (type == "login_response") {
            bool success = root["success"].toBool();
            if (success) {
                QMessageBox::information(this, "成功", "登录成功！");
                m_socket->setParent(nullptr);
                m_socket->setProperty("username", ui->txtUsername->text().trimmed());
                emit loginSuccess(m_socket); 
                this->close(); 
                return;
            } else {
                QString reason = root["message"].toString();
                QMessageBox::warning(this, "登录失败", reason);
                ui->btnLogin->setEnabled(true);
                ui->btnLogin->setText("登 录");
                m_socket->disconnectFromHost();
            }
        }
    }
}
