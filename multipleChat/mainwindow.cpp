#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <QCoreApplication>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_socket(nullptr)
{
    ui->setupUi(this);

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
    this->show();
}
