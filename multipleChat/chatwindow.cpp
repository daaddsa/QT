#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QShortcut>

chatWindow::chatWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chatWindow)
    , m_socket(nullptr)
{
    ui->setupUi(this);

    ui->btnEmoji->setEnabled(false);
    ui->btnImage->setEnabled(false);
    ui->btnFile->setEnabled(false);

    connect(ui->btnSend, &QPushButton::clicked, this, &chatWindow::onSendClicked);
    connect(ui->btnHeaderClose, &QToolButton::clicked, this, &chatWindow::onCloseClicked);

    auto *sendShortcut1 = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), this);
    auto *sendShortcut2 = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Enter), this);
    connect(sendShortcut1, &QShortcut::activated, this, &chatWindow::onSendClicked);
    connect(sendShortcut2, &QShortcut::activated, this, &chatWindow::onSendClicked);
}

chatWindow::~chatWindow()
{
    delete ui;
}

void chatWindow::attachSocket(QTcpSocket *socket, const QString &username)
{
    if (m_socket == socket) return;
    if (m_socket) {
        disconnect(m_socket, nullptr, this, nullptr);
    }
    m_socket = socket;
    m_username = username;

    if (m_socket) {
        connect(m_socket, &QTcpSocket::readyRead, this, &chatWindow::onSocketReadyRead);
        connect(m_socket, &QTcpSocket::disconnected, this, [this]() {
            ui->lblChatSubtitle->setText("离线");
        });
    }
}

void chatWindow::setConversationTitle(const QString &title)
{
    ui->lblChatTitle->setText(title);
    setWindowTitle(title);
}

void chatWindow::setParticipants(const QStringList &users)
{
    m_participants = users;
    updateParticipantsUi();
}

void chatWindow::onSocketReadyRead()
{
    if (!m_socket) return;
    while (m_socket->canReadLine()) {
        const QByteArray line = m_socket->readLine().trimmed();
        if (line.isEmpty()) continue;

        QJsonParseError err;
        const QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            continue;
        }

        const QJsonObject root = doc.object();
        const QString type = root.value("type").toString();

        if (type == "userlist") {
            const QJsonArray arr = root.value("userlist").toArray();
            QStringList users;
            users.reserve(arr.size());
            for (const QJsonValue &v : arr) users.append(v.toString());
            m_participants = users;
            updateParticipantsUi();
        } else if (type == "newuser") {
            const QString username = root.value("username").toString();
            if (!username.isEmpty() && !m_participants.contains(username)) {
                m_participants.append(username);
                updateParticipantsUi();
            }
        } else if (type == "userdisconnected") {
            const QString username = root.value("username").toString();
            if (!username.isEmpty()) {
                m_participants.removeAll(username);
                updateParticipantsUi();
            }
        } else if (type == "message") {
            const QString sender = root.value("sender").toString();
            const QString text = root.value("text").toString();
            if (!sender.isEmpty() && !text.isEmpty()) {
                appendMessage(sender, text);
            }
        }
    }
}

void chatWindow::onSendClicked()
{
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState) return;
    const QString text = ui->txtInput->toPlainText().trimmed();
    if (text.isEmpty()) return;

    QJsonObject json;
    json["type"] = "message";
    json["text"] = text;
    const QByteArray payload = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_socket->write(payload + "\n");
    ui->txtInput->clear();
}

void chatWindow::onCloseClicked()
{
    emit requestClose();
    close();
}

void chatWindow::appendMessage(const QString &sender, const QString &text)
{
    const QString ts = QDateTime::currentDateTime().toString("HH:mm:ss");
    ui->txtMessages->append(QString("[%1] %2: %3").arg(ts, sender, text));
}

void chatWindow::updateParticipantsUi()
{
    ui->listParticipants->clear();
    ui->listParticipants->addItems(m_participants);
    ui->lblChatSubtitle->setText(QString("在线：%1").arg(m_participants.size()));
}
