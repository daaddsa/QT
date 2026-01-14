#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chatwindow.h"
#include "login.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QShortcut>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_socket(nullptr)
    , m_chatWindow(nullptr)
{
    ui->setupUi(this);

    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    connect(ui->treeContacts, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onContactDoubleClicked);
    auto *sendShortcut1 = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), this);
    auto *sendShortcut2 = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Enter), this);
    connect(sendShortcut1, &QShortcut::activated, this, &MainWindow::onSendClicked);
    connect(sendShortcut2, &QShortcut::activated, this, &MainWindow::onSendClicked);

    if (QCoreApplication::arguments().contains("--no-login")) {
        this->show();
        return;
    }

    QTimer::singleShot(0, this, [this]() {
        this->hide();
        auto *loginWindow = new login(nullptr);
        connect(loginWindow, &login::loginSuccess, this, &MainWindow::onLoginSuccess);
        connect(loginWindow, &QObject::destroyed, this, [this]() {
            if (!m_socket) {
                QCoreApplication::quit();
            }
        });
        loginWindow->setAttribute(Qt::WA_DeleteOnClose);
        loginWindow->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoginSuccess(QTcpSocket *socket)
{
    m_socket = socket;
    m_socket->setParent(this);
    m_username = m_socket->property("username").toString();

    ui->lblNickname->setText(m_username.isEmpty() ? "已登录" : m_username);
    ui->lblStatus->setText("在线");
    appendLog(QString("已连接服务器：%1:%2").arg(m_socket->peerAddress().toString()).arg(m_socket->peerPort()));

    connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead, Qt::UniqueConnection);
    connect(m_socket, &QTcpSocket::disconnected, this, [this]() {
        ui->lblStatus->setText("离线");
        appendLog("与服务器断开连接");
    });
    connect(m_socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
        appendLog(QString("Socket错误：%1").arg(m_socket->errorString()));
    });

    this->show();
}

void MainWindow::onContactDoubleClicked(QTreeWidgetItem *item, int)
{
    if (!item || item->childCount() > 0) return;
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState) {
        appendLog("未连接服务器，无法打开对话窗口");
        return;
    }

    const QString target = item->text(0).trimmed();
    if (target.isEmpty()) return;

    if (m_chatWindow) {
        m_chatWindow->raise();
        m_chatWindow->activateWindow();
    } else {
        m_chatWindow = new chatWindow(nullptr);
        m_chatWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(m_chatWindow, &chatWindow::requestClose, this, &MainWindow::onChatWindowClosed);
        connect(m_chatWindow, &QObject::destroyed, this, [this]() {
            m_chatWindow = nullptr;
            onChatWindowClosed();
        });
    }

    QStringList users;
    for (int i = 0; i < ui->treeContacts->topLevelItemCount(); ++i) {
        auto *top = ui->treeContacts->topLevelItem(i);
        for (int j = 0; j < top->childCount(); ++j) users.append(top->child(j)->text(0));
    }
    m_chatWindow->setParticipants(users);

    const QString title = (target == m_username) ? "群聊" : QString("与 %1 的对话").arg(target);
    m_chatWindow->setConversationTitle(title);
    ui->lblConversationTitle->setText(title);

    disconnect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead);
    m_chatWindow->attachSocket(m_socket, m_username);
    m_chatWindow->show();
    m_chatWindow->raise();
    m_chatWindow->activateWindow();
}

void MainWindow::onChatWindowClosed()
{
    if (!m_socket) return;
    if (m_chatWindow) {
        m_chatWindow->attachSocket(nullptr, m_username);
    }
    connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead, Qt::UniqueConnection);
}

void MainWindow::onSocketReadyRead()
{
    while (m_socket && m_socket->canReadLine()) {
        const QByteArray line = m_socket->readLine().trimmed();
        if (line.isEmpty()) continue;

        QJsonParseError err;
        const QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            appendLog(QString("收到非法JSON：%1").arg(QString::fromUtf8(line)));
            continue;
        }

        const QJsonObject root = doc.object();
        const QString type = root.value("type").toString();

        if (type == "userlist") {
            const QJsonArray arr = root.value("userlist").toArray();
            QStringList users;
            users.reserve(arr.size());
            for (const QJsonValue &v : arr) users.append(v.toString());
            refreshUserList(users);
            appendLog(QString("在线用户数：%1").arg(users.size()));
        } else if (type == "newuser") {
            const QString username = root.value("username").toString();
            if (!username.isEmpty()) {
                QStringList users;
                for (int i = 0; i < ui->treeContacts->topLevelItemCount(); ++i) {
                    auto *top = ui->treeContacts->topLevelItem(i);
                    for (int j = 0; j < top->childCount(); ++j) users.append(top->child(j)->text(0));
                }
                if (!users.contains(username)) users.append(username);
                refreshUserList(users);
                appendLog(QString("用户上线：%1").arg(username));
            }
        } else if (type == "userdisconnected") {
            const QString username = root.value("username").toString();
            if (!username.isEmpty()) {
                QStringList users;
                for (int i = 0; i < ui->treeContacts->topLevelItemCount(); ++i) {
                    auto *top = ui->treeContacts->topLevelItem(i);
                    for (int j = 0; j < top->childCount(); ++j) {
                        const QString u = top->child(j)->text(0);
                        if (u != username) users.append(u);
                    }
                }
                refreshUserList(users);
                appendLog(QString("用户下线：%1").arg(username));
            }
        } else if (type == "message") {
            const QString sender = root.value("sender").toString();
            const QString text = root.value("text").toString();
            if (!sender.isEmpty() && !text.isEmpty()) {
                appendChat(sender, text);
            }
        } else {
            appendLog(QString("收到未知消息类型：%1").arg(type));
        }
    }
}

void MainWindow::onSendClicked()
{
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState) {
        appendLog("未连接服务器，无法发送消息");
        return;
    }

    const QString text = ui->txtMessageInput->toPlainText().trimmed();
    if (text.isEmpty()) return;

    QJsonObject json;
    json["type"] = "message";
    json["text"] = text;
    const QByteArray payload = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_socket->write(payload + "\n");

    ui->txtMessageInput->clear();
    appendLog(QString("已发送：%1").arg(text.left(60)));
}

void MainWindow::appendLog(const QString &msg)
{
    const QString ts = QDateTime::currentDateTime().toString("HH:mm:ss");
    ui->txtClientLog->append(QString("[%1] %2").arg(ts, msg));
}

void MainWindow::appendChat(const QString &sender, const QString &text)
{
    const QString ts = QDateTime::currentDateTime().toString("HH:mm:ss");
    ui->txtChatHistory->append(QString("[%1] %2: %3").arg(ts, sender, text));
}

void MainWindow::refreshUserList(const QStringList &users)
{
    ui->treeContacts->clear();
    auto *onlineRoot = new QTreeWidgetItem(ui->treeContacts);
    onlineRoot->setText(0, QString("在线 (%1)").arg(users.size()));
    for (const QString &u : users) {
        auto *item = new QTreeWidgetItem(onlineRoot);
        item->setText(0, u);
    }
    ui->treeContacts->expandAll();
}
