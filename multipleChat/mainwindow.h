#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QTreeWidgetItem;
class chatWindow;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onLoginSuccess(QTcpSocket *socket);

private slots:
    void onSocketReadyRead();
    void onSendClicked();
    void onContactDoubleClicked(QTreeWidgetItem *item, int column);
    void onChatWindowClosed();

private:
    void appendLog(const QString &msg);
    void appendChat(const QString &sender, const QString &text);
    void refreshUserList(const QStringList &users);

private:
    Ui::MainWindow *ui;
    QTcpSocket *m_socket;
    QString m_username;
    chatWindow *m_chatWindow;
};
#endif // MAINWINDOW_H
