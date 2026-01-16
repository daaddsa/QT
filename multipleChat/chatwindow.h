#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class chatWindow;
}

class chatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit chatWindow(QWidget *parent = nullptr);
    ~chatWindow();
    void attachSocket(QTcpSocket *socket, const QString &username);
    void setConversationTitle(const QString &title);
    void setChatTarget(const QString &target);
    void setConversationId(int conversationId);
    void setParticipants(const QStringList &users);

signals:
    void requestClose();

private:
    void appendMessage(const QString &sender, const QString &text);
    void updateParticipantsUi();

private slots:
    void onSocketReadyRead();
    void onSendClicked();
    void onCloseClicked();

private:
    Ui::chatWindow *ui;
    QTcpSocket *m_socket;
    QString m_username;
    QString m_target;
    int m_conversationId = 0;
    QStringList m_participants;
};

#endif // CHATWINDOW_H
