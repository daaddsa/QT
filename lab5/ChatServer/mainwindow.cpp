#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_chatServer = new chatServer(this);
    connect(m_chatServer, &chatServer::logMessage, this, &MainWindow::logMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startStopButton_clicked()
{
    if (m_chatServer->isListening()) {
        m_chatServer->stopServer();
        ui->startStopButton->setText("启动服务器");
        logMessage("服务器已停止");
    } else {
        if (m_chatServer->listen(QHostAddress::Any, 1967)) {
            ui->startStopButton->setText("停止服务器");
            logMessage("服务器已经启动");
        } else {
            logMessage("无法启动服务器: " + m_chatServer->errorString());
        }
    }
}

void MainWindow::logMessage(const QString &msg)
{
    ui->logEditor->append(msg);
}

