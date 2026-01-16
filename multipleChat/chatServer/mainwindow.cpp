#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_chatServer(new ChatServer(this))
    , m_isServerRunning(false)
{
    ui->setupUi(this);

    // 连接日志信号
    connect(m_chatServer, &ChatServer::logMessage, this, &MainWindow::logMessage);

    // 初始化 UI 状态
    ui->tblUsers->horizontalHeader()->setStretchLastSection(true);
    ui->tblUsers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblUsers->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止直接编辑表格

    logMessage("服务器管理控制台已就绪.");
    logMessage("请确认端口设置，并点击'启动服务'开始监听.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::startServer(int port)
{
    if (m_isServerRunning) return true;
    if (port <= 0 || port > 65535) return false;

    ui->txtPort->setText(QString::number(port));
    if (!m_chatServer->startServer(port)) return false;

    m_isServerRunning = true;
    ui->btnStartStop->setText("停止服务 (Stop)");
    ui->txtPort->setEnabled(false);
    return true;
}

void MainWindow::stopServer()
{
    if (!m_isServerRunning) return;
    m_chatServer->stopServer();
    m_isServerRunning = false;
    ui->btnStartStop->setText("启动服务 (Start)");
    ui->txtPort->setEnabled(true);
}

bool MainWindow::isServerRunning() const
{
    return m_isServerRunning;
}

void MainWindow::logMessage(const QString &msg)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->txtLog->append(QString("[%1] %2").arg(timestamp, msg));
}

void MainWindow::on_btnStartStop_clicked()
{
    if (!m_isServerRunning) {
        // 尝试启动服务器
        QString portStr = ui->txtPort->text();
        bool ok;
        int port = portStr.toInt(&ok);
        if (!ok || port <= 0 || port > 65535) {
            QMessageBox::critical(this, "错误", "请输入有效的端口号 (1-65535)");
            return;
        }

        if (!startServer(port)) return;

    } else {
        stopServer();
    }
}

void MainWindow::on_btnKick_clicked()
{
    // 获取当前选中的行
    int currentRow = ui->tblUsers->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择一个要下线的用户");
        return;
    }

    QString account = ui->tblUsers->item(currentRow, 1)->text(); // 假设第2列是账号
    Q_UNUSED(account);
}
