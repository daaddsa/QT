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

        if (m_chatServer->startServer(port)) {
            m_isServerRunning = true;
            ui->btnStartStop->setText("停止服务 (Stop)");
            ui->txtPort->setEnabled(false); // 运行时禁止修改端口
            // Log message is handled by signal
        }

    } else {
        // 停止服务器
        m_chatServer->stopServer();
        
        m_isServerRunning = false;
        ui->btnStartStop->setText("启动服务 (Start)");
        ui->txtPort->setEnabled(true);
        // Log message is handled by signal
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
    
    // TODO: 调用服务器逻辑强制断开该用户连接
    logMessage(QString("管理员强制下线用户: %1").arg(account));
}
