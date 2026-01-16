#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QTimer>

static int parsePortArgument(const QStringList &args, int defaultPort)
{
    for (int i = 0; i < args.size(); ++i) {
        const QString &arg = args.at(i);
        if (arg.startsWith("--port=")) {
            bool ok = false;
            const int port = arg.mid(QString("--port=").size()).toInt(&ok);
            return ok ? port : defaultPort;
        }
        if (arg == "--port" && i + 1 < args.size()) {
            bool ok = false;
            const int port = args.at(i + 1).toInt(&ok);
            return ok ? port : defaultPort;
        }
    }
    return defaultPort;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    const QStringList args = QCoreApplication::arguments();
    if (args.contains("--auto-start") || args.contains("--autostart")) {
        const int port = parsePortArgument(args, 12345);
        QTimer::singleShot(0, &w, [&w, port]() { w.startServer(port); });
    }
    if (args.contains("--smoke-test")) {
        QTimer::singleShot(200, &a, &QCoreApplication::quit);
    }
    return a.exec();
}
