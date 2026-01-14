#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTcpSocket>
#include <QTimer>

static QString argValue(const QStringList &args, const QString &key, const QString &defaultValue = QString())
{
    const QString prefix = key + "=";
    for (const QString &arg : args) {
        if (arg.startsWith(prefix)) return arg.mid(prefix.size());
    }
    return defaultValue;
}

static int argIntValue(const QStringList &args, const QString &key, int defaultValue)
{
    bool ok = false;
    const int v = argValue(args, key).toInt(&ok);
    return ok ? v : defaultValue;
}

static void runBotMode()
{
    const QStringList args = QCoreApplication::arguments();
    const QString host = argValue(args, "--host", "127.0.0.1");
    const int port = argIntValue(args, "--port", 12345);
    const QString username = argValue(args, "--username", "bot");
    const QString sendText = argValue(args, "--send");
    const QString expectNewUser = argValue(args, "--expect-newuser");
    const QString expectFrom = argValue(args, "--expect-from");
    const QString expectText = argValue(args, "--expect-text");
    const int stayMs = argIntValue(args, "--stay-ms", sendText.isEmpty() ? 1500 : 2500);
    const int timeoutMs = argIntValue(args, "--timeout-ms", 5000);

    auto *socket = new QTcpSocket(QCoreApplication::instance());
    QByteArray buffer;

    bool loginOk = false;
    bool gotUserList = false;
    bool gotEcho = sendText.isEmpty();
    bool gotExpectedNewUser = expectNewUser.isEmpty();
    bool gotExpectedMessage = (expectFrom.isEmpty() || expectText.isEmpty());

    QTimer *timeoutTimer = new QTimer(QCoreApplication::instance());
    timeoutTimer->setSingleShot(true);
    QObject::connect(timeoutTimer, &QTimer::timeout, [=]() {
        const bool ok = loginOk && gotUserList && gotEcho && gotExpectedNewUser && gotExpectedMessage;
        qInfo().noquote() << QString("BOT_RESULT ok=%1 loginOk=%2 userlist=%3 echo=%4 newuser=%5 expectedMsg=%6 error=%7")
                                 .arg(ok)
                                 .arg(loginOk)
                                 .arg(gotUserList)
                                 .arg(gotEcho)
                                 .arg(gotExpectedNewUser)
                                 .arg(gotExpectedMessage)
                                 .arg(socket->errorString());
        QCoreApplication::exit(ok ? 0 : 1);
    });

    QObject::connect(socket, &QTcpSocket::connected, [=, &buffer]() mutable {
        QJsonObject json;
        json["type"] = "login";
        json["username"] = username;
        socket->write(QJsonDocument(json).toJson(QJsonDocument::Compact) + "\n");
    });

    QObject::connect(socket, &QTcpSocket::readyRead, [=, &buffer, &loginOk, &gotUserList, &gotEcho]() mutable {
        buffer.append(socket->readAll());
        while (true) {
            const int idx = buffer.indexOf('\n');
            if (idx < 0) break;
            const QByteArray line = buffer.left(idx).trimmed();
            buffer.remove(0, idx + 1);
            if (line.isEmpty()) continue;

            QJsonParseError err;
            const QJsonDocument doc = QJsonDocument::fromJson(line, &err);
            if (err.error != QJsonParseError::NoError || !doc.isObject()) continue;
            const QJsonObject root = doc.object();
            const QString type = root.value("type").toString();

            if (type == "login_response") {
                loginOk = root.value("success").toBool();
                if (loginOk) {
                    qInfo().noquote() << "BOT_EVENT login_ok";
                    if (!sendText.isEmpty()) {
                        QJsonObject msg;
                        msg["type"] = "message";
                        msg["text"] = sendText;
                        socket->write(QJsonDocument(msg).toJson(QJsonDocument::Compact) + "\n");
                    }
                } else {
                    qInfo().noquote() << "BOT_EVENT login_fail" << root.value("message").toString();
                }
            } else if (type == "userlist") {
                gotUserList = true;
                const QJsonArray arr = root.value("userlist").toArray();
                qInfo().noquote() << QString("BOT_EVENT userlist size=%1").arg(arr.size());
            } else if (type == "message") {
                const QString sender = root.value("sender").toString();
                const QString text = root.value("text").toString();
                if (!sendText.isEmpty() && sender == username && text == sendText) {
                    gotEcho = true;
                    qInfo().noquote() << "BOT_EVENT echo_ok";
                }
                if (!expectFrom.isEmpty() && !expectText.isEmpty() && sender == expectFrom && text == expectText) {
                    gotExpectedMessage = true;
                    qInfo().noquote() << "BOT_EVENT expected_message_ok";
                }
            } else if (type == "newuser") {
                const QString newUser = root.value("username").toString();
                qInfo().noquote() << "BOT_EVENT newuser" << newUser;
                if (!expectNewUser.isEmpty() && newUser == expectNewUser) {
                    gotExpectedNewUser = true;
                    qInfo().noquote() << "BOT_EVENT expected_newuser_ok";
                }
            }
        }

        if (loginOk && gotUserList && gotEcho && gotExpectedNewUser && gotExpectedMessage) {
            QTimer::singleShot(stayMs, QCoreApplication::instance(), [=]() {
                qInfo().noquote() << "BOT_RESULT ok=1";
                QCoreApplication::exit(0);
            });
        }
    });

    QObject::connect(socket, &QTcpSocket::errorOccurred, [=](QAbstractSocket::SocketError) {
        qInfo().noquote() << "BOT_EVENT socket_error" << socket->errorString();
    });

    socket->connectToHost(host, port);
    timeoutTimer->start(timeoutMs);
}

int main(int argc, char *argv[])
{
    bool bot = false;
    for (int i = 1; i < argc; ++i) {
        if (QString::fromLocal8Bit(argv[i]) == "--bot") {
            bot = true;
            break;
        }
    }

    if (bot) {
        QCoreApplication a(argc, argv);
        runBotMode();
        return a.exec();
    }

    QApplication a(argc, argv);
    const QStringList args = QCoreApplication::arguments();

    MainWindow w;
    w.show();
    if (args.contains("--smoke-test")) {
        QTimer::singleShot(200, &a, &QCoreApplication::quit);
    }
    return a.exec();
}
