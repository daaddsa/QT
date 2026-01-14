#include "mainwindow.h"

#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    if (QCoreApplication::arguments().contains("--smoke-test")) {
        QTimer::singleShot(200, &a, &QCoreApplication::quit);
    }
    return a.exec();
}
