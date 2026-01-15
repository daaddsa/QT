#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QWidget>
#include <QStringList>

namespace Ui {
class searchDialog;
}

class searchDialog : public QWidget
{
    Q_OBJECT

public:
    explicit searchDialog(QWidget *parent = nullptr);
    ~searchDialog();
    void setContacts(const QStringList &users);
    void setGroups(const QStringList &groups);
    void setMessages(const QStringList &messages);
    void setQueryAndSearch(const QString &query);

signals:
    void contactActivated(const QString &username);

private:
    void performSearch();
    void updateStatus(int users, int groups, int messages);

private:
    Ui::searchDialog *ui;
    QStringList m_contacts;
    QStringList m_groups;
    QStringList m_messages;
};

#endif // SEARCHDIALOG_H
