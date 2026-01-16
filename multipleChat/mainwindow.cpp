#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chatwindow.h"
#include "login.h"
#include "searchdialog.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QShortcut>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_socket(nullptr)
    , m_chatWindow(nullptr)
    , m_searchDialog(nullptr)
{
    ui->setupUi(this);

    connect(ui->btnSend, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    connect(ui->treeContacts, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onContactDoubleClicked);
    connect(ui->txtSearch, &QLineEdit::returnPressed, this, &MainWindow::onMainSearchReturnPressed);
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->btnCreateGroup, &QPushButton::clicked, this, &MainWindow::onCreateGroupClicked);
    connect(ui->listGroups, &QListWidget::itemDoubleClicked, this, [this]() { onGroupDoubleClicked(); });
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
    openChatToTarget(target);
}

void MainWindow::onChatWindowClosed()
{
    if (!m_socket) return;
    if (m_chatWindow) {
        m_chatWindow->attachSocket(nullptr, m_username);
    }
    ui->btnAdd->setEnabled(true);
    ui->btnCreateGroup->setEnabled(true);
    connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead, Qt::UniqueConnection);
}

void MainWindow::onMainSearchReturnPressed()
{
    const QString query = ui->txtSearch->text().trimmed();
    if (!m_searchDialog) {
        m_searchDialog = new searchDialog(nullptr);
        m_searchDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(m_searchDialog, &QObject::destroyed, this, [this]() { m_searchDialog = nullptr; });
        connect(m_searchDialog, &searchDialog::contactActivated, this, [this](const QString &username) {
            openChatToTarget(username);
        });
    }

    QStringList contacts;
    for (int i = 0; i < ui->treeContacts->topLevelItemCount(); ++i) {
        auto *top = ui->treeContacts->topLevelItem(i);
        for (int j = 0; j < top->childCount(); ++j) contacts.append(top->child(j)->text(0));
    }

    QStringList groups;
    for (int i = 0; i < ui->listGroups->count(); ++i) {
        const QString g = ui->listGroups->item(i)->text().trimmed();
        if (!g.isEmpty()) groups.append(g);
    }

    const QString history = ui->txtChatHistory->toPlainText();
    QStringList messages = history.split('\n', Qt::SkipEmptyParts);

    m_searchDialog->setContacts(contacts);
    m_searchDialog->setGroups(groups);
    m_searchDialog->setMessages(messages);
    m_searchDialog->setQueryAndSearch(query);
    m_searchDialog->show();
    m_searchDialog->raise();
    m_searchDialog->activateWindow();
}

void MainWindow::openChatToTarget(const QString &target)
{
    if (target.isEmpty()) return;
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState) {
        appendLog("未连接服务器，无法打开对话窗口");
        return;
    }

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
    m_chatWindow->setChatTarget(target);
    m_chatWindow->setConversationId(0);
    ui->lblConversationTitle->setText(title);

    disconnect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead);
    m_chatWindow->attachSocket(m_socket, m_username);
    m_chatWindow->show();
    m_chatWindow->raise();
    m_chatWindow->activateWindow();

    ui->btnAdd->setEnabled(false);
    ui->btnCreateGroup->setEnabled(false);
}

void MainWindow::openGroupConversation(int conversationId, const QString &title)
{
    if (conversationId <= 0) return;
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState) {
        appendLog("未连接服务器，无法打开对话窗口");
        return;
    }

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

    const QString windowTitle = title.isEmpty() ? "群聊" : QString("群聊：%1").arg(title);
    m_chatWindow->setConversationTitle(windowTitle);
    m_chatWindow->setChatTarget(m_username);
    m_chatWindow->setConversationId(conversationId);
    ui->lblConversationTitle->setText(windowTitle);

    disconnect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead);
    m_chatWindow->attachSocket(m_socket, m_username);
    m_chatWindow->show();
    m_chatWindow->raise();
    m_chatWindow->activateWindow();

    ui->btnAdd->setEnabled(false);
    ui->btnCreateGroup->setEnabled(false);
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
        } else if (type == "check_user_response") {
            const bool success = root.value("success").toBool();
            const QString nickname = root.value("nickname").toString().trimmed();
            if (success && !nickname.isEmpty() && nickname == m_pendingCheckNickname) {
                m_pendingCheckNickname.clear();
                openChatToTarget(nickname);
            } else if (nickname == m_pendingCheckNickname) {
                m_pendingCheckNickname.clear();
                QMessageBox::warning(this, "提示", "用户不存在");
            }
        } else if (type == "create_group_response") {
            const bool success = root.value("success").toBool();
            const int conversationId = root.value("conversation_id").toInt();
            const QString title = root.value("title").toString();
            if (!success || conversationId <= 0) {
                QMessageBox::warning(this, "提示", root.value("message").toString().isEmpty() ? "建群失败" : root.value("message").toString());
                continue;
            }
            auto *item = new QListWidgetItem(title.isEmpty() ? "未命名群" : title);
            item->setData(Qt::UserRole, conversationId);
            ui->listGroups->addItem(item);
            appendLog(QString("建群成功：%1(id=%2)").arg(item->text()).arg(conversationId));
        } else if (type == "group_list") {
            ui->listGroups->clear();
            const QJsonArray groups = root.value("groups").toArray();
            for (const QJsonValue &v : groups) {
                if (!v.isObject()) continue;
                const QJsonObject g = v.toObject();
                const int id = g.value("id").toInt();
                const QString title = g.value("title").toString();
                if (id <= 0) continue;
                auto *item = new QListWidgetItem(title.isEmpty() ? QString("群%1").arg(id) : title);
                item->setData(Qt::UserRole, id);
                ui->listGroups->addItem(item);
            }
        } else {
            appendLog(QString("收到未知消息类型：%1").arg(type));
        }
    }
}

void MainWindow::onAddClicked()
{
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState) {
        QMessageBox::warning(this, "提示", "未连接服务器");
        return;
    }

    bool ok = false;
    const QString nickname = QInputDialog::getText(this, "添加", "输入对方昵称：", QLineEdit::Normal, "", &ok).trimmed();
    if (!ok || nickname.isEmpty()) return;
    if (nickname == m_username) {
        QMessageBox::warning(this, "提示", "不能添加自己");
        return;
    }

    m_pendingCheckNickname = nickname;
    QJsonObject req;
    req["type"] = "check_user";
    req["nickname"] = nickname;
    m_socket->write(QJsonDocument(req).toJson(QJsonDocument::Compact) + "\n");
}

void MainWindow::onCreateGroupClicked()
{
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState) {
        QMessageBox::warning(this, "提示", "未连接服务器");
        return;
    }

    bool ok = false;
    const QString title = QInputDialog::getText(this, "建群", "群名称：", QLineEdit::Normal, "我的群聊", &ok).trimmed();
    if (!ok || title.isEmpty()) return;

    const QString hint = "输入成员昵称（逗号分隔，可空）：";
    const QString membersText = QInputDialog::getText(this, "建群", hint, QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QStringList members = membersText.split(',', Qt::SkipEmptyParts);
    for (QString &m : members) m = m.trimmed();
    members.removeAll(QString());
    if (!members.contains(m_username)) members.prepend(m_username);
    members.removeDuplicates();

    QJsonArray arr;
    for (const QString &m : members) arr.append(m);

    QJsonObject req;
    req["type"] = "create_group";
    req["title"] = title;
    req["members"] = arr;
    m_socket->write(QJsonDocument(req).toJson(QJsonDocument::Compact) + "\n");
}

void MainWindow::onGroupDoubleClicked()
{
    auto *item = ui->listGroups->currentItem();
    if (!item) return;
    const int conversationId = item->data(Qt::UserRole).toInt();
    openGroupConversation(conversationId, item->text().trimmed());
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
