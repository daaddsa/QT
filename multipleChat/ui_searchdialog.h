/********************************************************************************
** Form generated from reading UI file 'searchdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHDIALOG_H
#define UI_SEARCHDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_searchDialog
{
public:

    void setupUi(QWidget *searchDialog)
    {
        if (searchDialog->objectName().isEmpty())
            searchDialog->setObjectName("searchDialog");
        searchDialog->resize(400, 300);

        retranslateUi(searchDialog);

        QMetaObject::connectSlotsByName(searchDialog);
    } // setupUi

    void retranslateUi(QWidget *searchDialog)
    {
        searchDialog->setWindowTitle(QCoreApplication::translate("searchDialog", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class searchDialog: public Ui_searchDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHDIALOG_H
