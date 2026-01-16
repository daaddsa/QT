#include "signup.h"
#include "ui_signup.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

signUp::signUp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signUp)
    , m_socket(new QTcpSocket(this))
    , m_serverPort(0)
{
    ui->setupUi(this);

    connect(ui->btnRegister, &QPushButton::clicked, this, &signUp::onRegisterClicked);
    connect(ui->btnBackToLogin, &QPushButton::clicked, this, &signUp::onBackClicked);

    connect(m_socket, &QTcpSocket::connected, this, &signUp::onConnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &signUp::onErrorOccurred);
    connect(m_socket, &QTcpSocket::readyRead, this, &signUp::onReadyRead);
}

signUp::~signUp()
{
    delete ui;
}

void signUp::setServerEndpoint(const QString &ip, int port)
{
    m_serverIp = ip;
    m_serverPort = port;
}

void signUp::onRegisterClicked()
{
    const QString account = ui->txtAccount->text().trimmed();
    const QString nickname = ui->txtNickname->text().trimmed();
    const QString password = ui->txtPassword->text();
    const QString confirmPassword = ui->txtConfirmPassword->text();

    if (account.isEmpty() || nickname.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "提示", "请完整填写账号信息");
        return;
    }
    if (password != confirmPassword) {
        QMessageBox::warning(this, "提示", "两次输入的密码不一致");
        return;
    }
    if (m_serverIp.isEmpty() || m_serverPort <= 0 || m_serverPort > 65535) {
        QMessageBox::warning(this, "提示", "服务器地址或端口无效，请从登录页进入注册");
        return;
    }

    m_pendingAccount = account;
    m_pendingNickname = nickname;

    ui->btnRegister->setEnabled(false);
    ui->btnRegister->setText("注册中...");
    ui->btnBackToLogin->setEnabled(false);

    m_socket->abort();
    m_socket->connectToHost(m_serverIp, m_serverPort);
}

void signUp::onBackClicked()
{
    emit backToLogin();
    close();
}

void signUp::onConnected()
{
    QJsonObject json;
    json["type"] = "signup";
    json["account"] = ui->txtAccount->text().trimmed();
    json["nickname"] = ui->txtNickname->text().trimmed();
    json["password"] = ui->txtPassword->text();

    const QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_socket->write(data + "\n");
}

void signUp::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    QMessageBox::critical(this, "注册失败", m_socket->errorString());
    ui->btnRegister->setEnabled(true);
    ui->btnRegister->setText("立即注册");
    ui->btnBackToLogin->setEnabled(true);
}

void signUp::onReadyRead()
{
    while (m_socket->canReadLine()) {
        const QByteArray data = m_socket->readLine().trimmed();
        if (data.isEmpty()) continue;

        const QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || !doc.isObject()) continue;

        const QJsonObject root = doc.object();
        const QString type = root.value("type").toString();
        if (type != "signup_response") continue;

        const bool success = root.value("success").toBool();
        const QString message = root.value("message").toString();

        if (success) {
            QMessageBox::information(this, "成功", "注册成功！");
            emit signUpSuccess(m_pendingAccount.isEmpty() ? ui->txtAccount->text().trimmed() : m_pendingAccount);
            close();
            return;
        }

        QMessageBox::warning(this, "注册失败", message.isEmpty() ? "注册失败" : message);
        ui->btnRegister->setEnabled(true);
        ui->btnRegister->setText("立即注册");
        ui->btnBackToLogin->setEnabled(true);
        m_socket->disconnectFromHost();
    }
}
