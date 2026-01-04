#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QWidget>

namespace Ui {
class searchDialog;
}

class searchDialog : public QWidget
{
    Q_OBJECT

public:
    explicit searchDialog(QWidget *parent = nullptr);
    ~searchDialog();

private:
    Ui::searchDialog *ui;
};

#endif // SEARCHDIALOG_H
