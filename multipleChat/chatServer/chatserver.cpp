#include "chatserver.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSet>
#include <QSqlQuery>

ChatServer::ChatServer(QObject *parent)
    : QTcpServer(parent)
{
}

bool ChatServer::startServer(int port)
{
    ensureDatabaseOpen();

    if (!this->listen(QHostAddress::Any, port)) {
        return false;
    }
    return true;
}

void ChatServer::stopServer()
{
    for (ServerWorker *worker : m_clients) {
        worker->disconnectFromClient();
    }
    m_clients.clear();
    this->close();
}

void ChatServer::broadcast(const QJsonObject &message, ServerWorker *exclude)
{
    for (ServerWorker *worker : m_clients) {
        if (worker == exclude) continue;
        worker->sendJson(message);
    }
}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    ServerWorker *worker = new ServerWorker(this);
    if (!worker->setSocketDescriptor(socketDescriptor)) {
        worker->deleteLater();
        return;
    }
    
    connect(worker, &ServerWorker::jsonReceived, this, [this, worker](const QJsonObject &json){
        jsonReceived(json, worker);
    });
    
    connect(worker, &ServerWorker::disconnectedFromClient, this, [this, worker](){
        userDisconnected(worker);
    });

    m_clients.append(worker);
}

QString ChatServer::resolveDatabasePath() const
{
    const QString fileName = "multiplechatdatabase.db";
    const QString appDir = QCoreApplication::applicationDirPath();
    const QString cwd = QDir::currentPath();

    const QString p1 = QDir(appDir).filePath(fileName);
    const QString p2 = QDir(appDir).filePath(QString("../%1").arg(fileName));
    const QString p3 = QDir(cwd).filePath(fileName);

    if (QFileInfo::exists(p1)) return QFileInfo(p1).absoluteFilePath();
    if (QFileInfo::exists(p2)) return QFileInfo(p2).absoluteFilePath();
    if (QFileInfo::exists(p3)) return QFileInfo(p3).absoluteFilePath();
    return QFileInfo(p2).absoluteFilePath();
}

bool ChatServer::ensureDatabaseOpen()
{
    if (m_db.isValid() && m_db.isOpen()) return true;

    const QString connName = "multiplechat_sqlite";
    if (QSqlDatabase::contains(connName)) {
        m_db = QSqlDatabase::database(connName);
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", connName);
    }
    m_db.setDatabaseName(resolveDatabasePath());
    if (!m_db.open()) {
        return false;
    }

    QSqlQuery pragma(m_db);
    pragma.exec("PRAGMA foreign_keys = ON");
    ensureSchema();
    return true;
}

void ChatServer::ensureSchema()
{
    QSqlQuery q(m_db);
    q.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " account TEXT NOT NULL UNIQUE,"
        " nickname TEXT NOT NULL,"
        " password TEXT NOT NULL,"
        " created_at DATETIME NOT NULL DEFAULT (datetime('now'))"
        ")"
    );

    q.exec(
        "CREATE TABLE IF NOT EXISTS conversations ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " type TEXT NOT NULL CHECK (type IN ('direct', 'group')),"
        " title TEXT,"
        " created_at DATETIME NOT NULL DEFAULT (datetime('now'))"
        ")"
    );

    q.exec(
        "CREATE TABLE IF NOT EXISTS conversation_members ("
        " conversation_id INTEGER NOT NULL,"
        " user_id INTEGER NOT NULL,"
        " role TEXT NOT NULL DEFAULT 'member',"
        " joined_at DATETIME NOT NULL DEFAULT (datetime('now')),"
        " PRIMARY KEY (conversation_id, user_id),"
        " FOREIGN KEY (conversation_id) REFERENCES conversations(id) ON DELETE CASCADE,"
        " FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE"
        ")"
    );

    q.exec(
        "CREATE TABLE IF NOT EXISTS messages ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " conversation_id INTEGER NOT NULL,"
        " sender_id INTEGER NOT NULL,"
        " content TEXT NOT NULL,"
        " msg_type TEXT NOT NULL DEFAULT 'text',"
        " created_at DATETIME NOT NULL DEFAULT (datetime('now')),"
        " FOREIGN KEY (conversation_id) REFERENCES conversations(id) ON DELETE CASCADE,"
        " FOREIGN KEY (sender_id) REFERENCES users(id) ON DELETE CASCADE"
        ")"
    );

    q.exec(
        "CREATE TABLE IF NOT EXISTS message_receipts ("
        " message_id INTEGER NOT NULL,"
        " user_id INTEGER NOT NULL,"
        " delivered_at DATETIME,"
        " read_at DATETIME,"
        " PRIMARY KEY (message_id, user_id),"
        " FOREIGN KEY (message_id) REFERENCES messages(id) ON DELETE CASCADE,"
        " FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE"
        ")"
    );
}

int ChatServer::findUserIdByNickname(const QString &nickname)
{
    if (nickname.isEmpty()) return 0;
    ensureDatabaseOpen();
    QSqlQuery q(m_db);
    q.prepare("SELECT id FROM users WHERE nickname = ? LIMIT 1");
    q.addBindValue(nickname);
    if (!q.exec() || !q.next()) return 0;
    return q.value(0).toInt();
}

int ChatServer::ensureDirectConversation(int userId1, int userId2)
{
    ensureDatabaseOpen();
    if (userId1 <= 0 || userId2 <= 0 || userId1 == userId2) return 0;

    QSqlQuery q(m_db);
    q.prepare(
        "SELECT c.id FROM conversations c "
        "JOIN conversation_members m1 ON m1.conversation_id = c.id AND m1.user_id = ? "
        "JOIN conversation_members m2 ON m2.conversation_id = c.id AND m2.user_id = ? "
        "WHERE c.type = 'direct' "
        "LIMIT 1"
    );
    q.addBindValue(userId1);
    q.addBindValue(userId2);
    if (q.exec() && q.next()) return q.value(0).toInt();

    QSqlQuery ins(m_db);
    ins.prepare("INSERT INTO conversations(type, title) VALUES('direct', NULL)");
    if (!ins.exec()) return 0;
    const int convId = ins.lastInsertId().toInt();
    if (convId <= 0) return 0;

    QSqlQuery mem(m_db);
    mem.prepare("INSERT OR IGNORE INTO conversation_members(conversation_id, user_id, role) VALUES(?, ?, 'member')");
    mem.addBindValue(convId);
    mem.addBindValue(userId1);
    if (!mem.exec()) return 0;
    mem.clear();
    mem.prepare("INSERT OR IGNORE INTO conversation_members(conversation_id, user_id, role) VALUES(?, ?, 'member')");
    mem.addBindValue(convId);
    mem.addBindValue(userId2);
    if (!mem.exec()) return 0;

    return convId;
}

int ChatServer::ensureGroupConversation()
{
    ensureDatabaseOpen();
    const QString title = "群聊";
    QSqlQuery q(m_db);
    q.prepare("SELECT id FROM conversations WHERE type = 'group' AND IFNULL(title, '') = ? ORDER BY id LIMIT 1");
    q.addBindValue(title);
    if (q.exec() && q.next()) return q.value(0).toInt();

    QSqlQuery ins(m_db);
    ins.prepare("INSERT INTO conversations(type, title) VALUES('group', ?)");
    ins.addBindValue(title);
    if (!ins.exec()) return 0;
    return ins.lastInsertId().toInt();
}

void ChatServer::deliverUndeliveredMessages(ServerWorker *recipient)
{
    if (!recipient) return;
    ensureDatabaseOpen();
    const int userId = recipient->userId();
    if (userId <= 0) return;

    const QString myNick = recipient->userName();
    QSqlQuery q(m_db);
    q.prepare(
        "SELECT m.id, m.conversation_id, m.content, u.nickname, c.type "
        "FROM message_receipts r "
        "JOIN messages m ON m.id = r.message_id "
        "JOIN users u ON u.id = m.sender_id "
        "JOIN conversations c ON c.id = m.conversation_id "
        "WHERE r.user_id = ? AND r.delivered_at IS NULL "
        "ORDER BY m.id"
    );
    q.addBindValue(userId);
    if (!q.exec()) return;

    QSqlQuery upd(m_db);
    upd.prepare("UPDATE message_receipts SET delivered_at = datetime('now') WHERE message_id = ? AND user_id = ? AND delivered_at IS NULL");

    while (q.next()) {
        const int messageId = q.value(0).toInt();
        const int conversationId = q.value(1).toInt();
        const QString content = q.value(2).toString();
        const QString senderNick = q.value(3).toString();
        const QString convType = q.value(4).toString();

        QJsonObject msgObj;
        msgObj["type"] = "message";
        msgObj["sender"] = senderNick;
        msgObj["text"] = content;
        msgObj["conversation_id"] = conversationId;
        if (convType == "direct") msgObj["to"] = myNick;
        recipient->sendJson(msgObj);

        upd.addBindValue(messageId);
        upd.addBindValue(userId);
        upd.exec();
        upd.finish();
    }
}

void ChatServer::jsonReceived(const QJsonObject &jsonDoc, ServerWorker *sender)
{
    const QJsonValue typeVal = jsonDoc.value("type");
    if (typeVal.isNull() || !typeVal.isString()) return;

    const QString type = typeVal.toString();

    if (type == "signup") {
        const QString account = jsonDoc.value("account").toString().trimmed();
        const QString nickname = jsonDoc.value("nickname").toString().trimmed();
        const QString password = jsonDoc.value("password").toString();

        QJsonObject resp;
        resp["type"] = "signup_response";

        if (account.isEmpty() || nickname.isEmpty() || password.isEmpty()) {
            resp["success"] = false;
            sender->sendJson(resp);
            return;
        }

        bool exists = m_registeredUsers.contains(account);
        ensureDatabaseOpen();
        if (!exists) {
            QSqlQuery check(m_db);
            check.prepare("SELECT 1 FROM users WHERE account = ? LIMIT 1");
            check.addBindValue(account);
            if (check.exec() && check.next()) exists = true;
        }
        if (exists) {
            resp["success"] = false;
            sender->sendJson(resp);
            return;
        }

        QSqlQuery ins(m_db);
        ins.prepare("INSERT INTO users(account, nickname, password) VALUES(?, ?, ?)");
        ins.addBindValue(account);
        ins.addBindValue(nickname);
        ins.addBindValue(password);
        ins.exec();

        RegisteredUser u{nickname, password};
        m_registeredUsers.insert(account, u);

        resp["success"] = true;
        sender->sendJson(resp);
        return;
    } else if (type == "login") {
        const QString account = jsonDoc.value("account").toString().trimmed();
        const QString password = jsonDoc.value("password").toString();

        QJsonObject response;
        response["type"] = "login_response";

        if (account.isEmpty() || password.isEmpty()) {
            response["success"] = false;
            sender->sendJson(response);
            return;
        }

        int userId = 0;
        QString nickname;
        ensureDatabaseOpen();
        QSqlQuery q(m_db);
        q.prepare("SELECT id, nickname FROM users WHERE account = ? AND password = ? LIMIT 1");
        q.addBindValue(account);
        q.addBindValue(password);
        if (q.exec() && q.next()) {
            userId = q.value(0).toInt();
            nickname = q.value(1).toString();
        } else if (m_registeredUsers.contains(account) && m_registeredUsers.value(account).password == password) {
            nickname = m_registeredUsers.value(account).nickname;
        } else {
            response["success"] = false;
            sender->sendJson(response);
            return;
        }

        for (ServerWorker *worker : m_clients) {
            if (worker == sender) continue;
            if (!worker->account().isEmpty() && worker->account() == account) {
                response["success"] = false;
                sender->sendJson(response);
                return;
            }
            if (userId > 0 && worker->userId() == userId) {
                response["success"] = false;
                sender->sendJson(response);
                return;
            }
        }

        sender->setAccount(account);
        sender->setUserId(userId);
        sender->setUserName(nickname);

        response["success"] = true;
        response["account"] = account;
        response["nickname"] = nickname;
        sender->sendJson(response);

        QJsonObject userListObj;
        userListObj["type"] = "userlist";
        QJsonArray userListArray;
        for (ServerWorker *worker : m_clients) {
            if (!worker->userName().isEmpty()) userListArray.append(worker->userName());
        }
        userListObj["userlist"] = userListArray;
        sender->sendJson(userListObj);

        QJsonObject newUserObj;
        newUserObj["type"] = "newuser";
        newUserObj["username"] = nickname;
        broadcast(newUserObj, sender);

        if (userId > 0) deliverUndeliveredMessages(sender);
        if (userId > 0) {
            QSqlQuery gq(m_db);
            gq.prepare(
                "SELECT c.id, c.title "
                "FROM conversations c "
                "JOIN conversation_members m ON m.conversation_id = c.id "
                "WHERE c.type = 'group' AND m.user_id = ? "
                "ORDER BY c.id"
            );
            gq.addBindValue(userId);
            if (gq.exec()) {
                QJsonArray groups;
                while (gq.next()) {
                    QJsonObject g;
                    g["id"] = gq.value(0).toInt();
                    g["title"] = gq.value(1).toString();
                    groups.append(g);
                }
                QJsonObject gl;
                gl["type"] = "group_list";
                gl["groups"] = groups;
                sender->sendJson(gl);
            }
        }
    } else if (type == "message") {
        const QString text = jsonDoc.value("text").toString().trimmed();
        if (text.isEmpty()) return;
        const QString toNick = jsonDoc.value("to").toString().trimmed();

        const QString senderNick = sender->userName();
        const int senderId = sender->userId();
        ensureDatabaseOpen();

        if (senderId <= 0) {
            QJsonObject msgObj;
            msgObj["type"] = "message";
            msgObj["sender"] = senderNick;
            msgObj["text"] = text;
            if (!toNick.isEmpty()) msgObj["to"] = toNick;

            if (toNick.isEmpty()) {
                broadcast(msgObj);
                return;
            }

            for (ServerWorker *worker : m_clients) {
                if (worker == sender || worker->userName() == toNick) worker->sendJson(msgObj);
            }
            return;
        }

        if (!toNick.isEmpty()) {
            const int recipientId = findUserIdByNickname(toNick);
            if (recipientId <= 0) return;
            int conversationId = jsonDoc.value("conversation_id").toInt();
            if (conversationId <= 0) conversationId = ensureDirectConversation(senderId, recipientId);
            if (conversationId <= 0) return;

            QSqlQuery ins(m_db);
            ins.prepare("INSERT INTO messages(conversation_id, sender_id, content, msg_type) VALUES(?, ?, ?, 'text')");
            ins.addBindValue(conversationId);
            ins.addBindValue(senderId);
            ins.addBindValue(text);
            if (!ins.exec()) return;
            const int messageId = ins.lastInsertId().toInt();

            QSqlQuery rec(m_db);
            rec.prepare("INSERT OR IGNORE INTO message_receipts(message_id, user_id) VALUES(?, ?)");
            rec.addBindValue(messageId);
            rec.addBindValue(recipientId);
            rec.exec();

            QJsonObject msgObj;
            msgObj["type"] = "message";
            msgObj["sender"] = senderNick;
            msgObj["text"] = text;
            msgObj["conversation_id"] = conversationId;
            msgObj["to"] = toNick;

            bool delivered = false;
            for (ServerWorker *worker : m_clients) {
                if (worker == sender || worker->userId() == recipientId) {
                    worker->sendJson(msgObj);
                    if (worker->userId() == recipientId) delivered = true;
                }
            }

            if (delivered) {
                QSqlQuery upd(m_db);
                upd.prepare("UPDATE message_receipts SET delivered_at = datetime('now') WHERE message_id = ? AND user_id = ? AND delivered_at IS NULL");
                upd.addBindValue(messageId);
                upd.addBindValue(recipientId);
                upd.exec();
            }
            return;
        }

        int conversationId = jsonDoc.value("conversation_id").toInt();
        if (conversationId <= 0) conversationId = ensureGroupConversation();
        if (conversationId <= 0) return;

        QSqlQuery ins(m_db);
        ins.prepare("INSERT INTO messages(conversation_id, sender_id, content, msg_type) VALUES(?, ?, ?, 'text')");
        ins.addBindValue(conversationId);
        ins.addBindValue(senderId);
        ins.addBindValue(text);
        if (!ins.exec()) return;
        const int messageId = ins.lastInsertId().toInt();

        QSet<int> memberIds;
        QSqlQuery members(m_db);
        members.prepare("SELECT user_id FROM conversation_members WHERE conversation_id = ? AND user_id <> ?");
        members.addBindValue(conversationId);
        members.addBindValue(senderId);
        if (members.exec()) {
            while (members.next()) memberIds.insert(members.value(0).toInt());
        }

        if (!memberIds.isEmpty()) {
            QSqlQuery rec(m_db);
            rec.prepare("INSERT OR IGNORE INTO message_receipts(message_id, user_id) VALUES(?, ?)");
            for (int uid : memberIds) {
                if (uid <= 0) continue;
                rec.addBindValue(messageId);
                rec.addBindValue(uid);
                rec.exec();
                rec.finish();
            }
        }

        QJsonObject msgObj;
        msgObj["type"] = "message";
        msgObj["sender"] = senderNick;
        msgObj["text"] = text;
        msgObj["conversation_id"] = conversationId;
        if (memberIds.isEmpty()) {
            broadcast(msgObj);
        } else {
            for (ServerWorker *worker : m_clients) {
                const int uid = worker->userId();
                if (worker == sender || memberIds.contains(uid)) worker->sendJson(msgObj);
            }
        }

        QSqlQuery upd(m_db);
        upd.prepare("UPDATE message_receipts SET delivered_at = datetime('now') WHERE message_id = ? AND user_id = ? AND delivered_at IS NULL");
        if (!memberIds.isEmpty()) {
            for (ServerWorker *worker : m_clients) {
                const int uid = worker->userId();
                if (!memberIds.contains(uid)) continue;
                upd.addBindValue(messageId);
                upd.addBindValue(uid);
                upd.exec();
                upd.finish();
            }
        }
    } else if (type == "check_user") {
        QJsonObject resp;
        resp["type"] = "check_user_response";
        const QString nickname = jsonDoc.value("nickname").toString().trimmed();
        resp["nickname"] = nickname;
        if (nickname.isEmpty()) {
            resp["success"] = false;
            sender->sendJson(resp);
            return;
        }
        ensureDatabaseOpen();
        QSqlQuery q(m_db);
        q.prepare("SELECT 1 FROM users WHERE nickname = ? LIMIT 1");
        q.addBindValue(nickname);
        resp["success"] = (q.exec() && q.next());
        sender->sendJson(resp);
    } else if (type == "create_group") {
        QJsonObject resp;
        resp["type"] = "create_group_response";

        const int ownerId = sender->userId();
        ensureDatabaseOpen();
        if (ownerId <= 0) {
            resp["success"] = false;
            sender->sendJson(resp);
            return;
        }

        const QString title = jsonDoc.value("title").toString().trimmed();
        if (title.isEmpty()) {
            resp["success"] = false;
            sender->sendJson(resp);
            return;
        }

        QSet<int> memberIds;
        memberIds.insert(ownerId);
        const QJsonArray members = jsonDoc.value("members").toArray();
        for (const QJsonValue &v : members) {
            const QString nick = v.toString().trimmed();
            if (nick.isEmpty()) continue;
            const int uid = findUserIdByNickname(nick);
            if (uid > 0) memberIds.insert(uid);
        }

        QSqlQuery ins(m_db);
        ins.prepare("INSERT INTO conversations(type, title) VALUES('group', ?)");
        ins.addBindValue(title);
        if (!ins.exec()) {
            resp["success"] = false;
            sender->sendJson(resp);
            return;
        }
        const int conversationId = ins.lastInsertId().toInt();
        if (conversationId <= 0) {
            resp["success"] = false;
            sender->sendJson(resp);
            return;
        }

        QSqlQuery mem(m_db);
        mem.prepare("INSERT OR IGNORE INTO conversation_members(conversation_id, user_id, role) VALUES(?, ?, ?)");
        for (int uid : memberIds) {
            mem.addBindValue(conversationId);
            mem.addBindValue(uid);
            mem.addBindValue(uid == ownerId ? "owner" : "member");
            mem.exec();
            mem.finish();
        }

        resp["success"] = true;
        resp["conversation_id"] = conversationId;
        resp["title"] = title;
        sender->sendJson(resp);
    }
}

void ChatServer::userDisconnected(ServerWorker *sender)
{
    m_clients.removeAll(sender);
    QString userName = sender->userName();
    if (!userName.isEmpty()) {
        QJsonObject disconnectedObj;
        disconnectedObj["type"] = "userdisconnected";
        disconnectedObj["username"] = userName;
        broadcast(disconnectedObj);
    }
    sender->deleteLater();
}
