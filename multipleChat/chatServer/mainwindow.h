#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chatserver.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 用于在日志框中追加信息
    void logMessage(const QString &msg);
    bool startServer(int port);
    void stopServer();
    bool isServerRunning() const;

private slots:
    void on_btnStartStop_clicked();
    void on_btnKick_clicked();

private:
    Ui::MainWindow *ui;
    ChatServer *m_chatServer;
    bool m_isServerRunning; // 记录服务器运行状态
};
#endif // MAINWINDOW_H
