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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_searchDialog
{
public:
    QVBoxLayout *verticalLayout_root;
    QHBoxLayout *horizontalLayout_header;
    QLabel *lblTitle;
    QSpacerItem *horizontalSpacer_header;
    QToolButton *btnClose;
    QHBoxLayout *horizontalLayout_search;
    QLineEdit *txtQuery;
    QComboBox *comboScope;
    QPushButton *btnSearch;
    QTabWidget *tabResults;
    QWidget *tabUsers;
    QVBoxLayout *verticalLayout_users;
    QListWidget *listUsers;
    QWidget *tabGroups;
    QVBoxLayout *verticalLayout_groups;
    QListWidget *listGroups;
    QWidget *tabMessages;
    QVBoxLayout *verticalLayout_messages;
    QListWidget *listMessages;
    QHBoxLayout *horizontalLayout_footer;
    QLabel *lblStatus;
    QSpacerItem *horizontalSpacer_footer;
    QPushButton *btnClear;

    void setupUi(QWidget *searchDialog)
    {
        if (searchDialog->objectName().isEmpty())
            searchDialog->setObjectName("searchDialog");
        searchDialog->resize(560, 420);
        verticalLayout_root = new QVBoxLayout(searchDialog);
        verticalLayout_root->setObjectName("verticalLayout_root");
        horizontalLayout_header = new QHBoxLayout();
        horizontalLayout_header->setObjectName("horizontalLayout_header");
        lblTitle = new QLabel(searchDialog);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

        horizontalLayout_header->addWidget(lblTitle);

        horizontalSpacer_header = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_header->addItem(horizontalSpacer_header);

        btnClose = new QToolButton(searchDialog);
        btnClose->setObjectName("btnClose");
        btnClose->setAutoRaise(true);

        horizontalLayout_header->addWidget(btnClose);


        verticalLayout_root->addLayout(horizontalLayout_header);

        horizontalLayout_search = new QHBoxLayout();
        horizontalLayout_search->setObjectName("horizontalLayout_search");
        txtQuery = new QLineEdit(searchDialog);
        txtQuery->setObjectName("txtQuery");

        horizontalLayout_search->addWidget(txtQuery);

        comboScope = new QComboBox(searchDialog);
        comboScope->addItem(QString());
        comboScope->addItem(QString());
        comboScope->addItem(QString());
        comboScope->addItem(QString());
        comboScope->setObjectName("comboScope");

        horizontalLayout_search->addWidget(comboScope);

        btnSearch = new QPushButton(searchDialog);
        btnSearch->setObjectName("btnSearch");
        btnSearch->setMinimumSize(QSize(90, 0));

        horizontalLayout_search->addWidget(btnSearch);


        verticalLayout_root->addLayout(horizontalLayout_search);

        tabResults = new QTabWidget(searchDialog);
        tabResults->setObjectName("tabResults");
        tabResults->setTabPosition(QTabWidget::North);
        tabUsers = new QWidget();
        tabUsers->setObjectName("tabUsers");
        verticalLayout_users = new QVBoxLayout(tabUsers);
        verticalLayout_users->setObjectName("verticalLayout_users");
        listUsers = new QListWidget(tabUsers);
        listUsers->setObjectName("listUsers");

        verticalLayout_users->addWidget(listUsers);

        tabResults->addTab(tabUsers, QString());
        tabGroups = new QWidget();
        tabGroups->setObjectName("tabGroups");
        verticalLayout_groups = new QVBoxLayout(tabGroups);
        verticalLayout_groups->setObjectName("verticalLayout_groups");
        listGroups = new QListWidget(tabGroups);
        listGroups->setObjectName("listGroups");

        verticalLayout_groups->addWidget(listGroups);

        tabResults->addTab(tabGroups, QString());
        tabMessages = new QWidget();
        tabMessages->setObjectName("tabMessages");
        verticalLayout_messages = new QVBoxLayout(tabMessages);
        verticalLayout_messages->setObjectName("verticalLayout_messages");
        listMessages = new QListWidget(tabMessages);
        listMessages->setObjectName("listMessages");

        verticalLayout_messages->addWidget(listMessages);

        tabResults->addTab(tabMessages, QString());

        verticalLayout_root->addWidget(tabResults);

        horizontalLayout_footer = new QHBoxLayout();
        horizontalLayout_footer->setObjectName("horizontalLayout_footer");
        lblStatus = new QLabel(searchDialog);
        lblStatus->setObjectName("lblStatus");

        horizontalLayout_footer->addWidget(lblStatus);

        horizontalSpacer_footer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_footer->addItem(horizontalSpacer_footer);

        btnClear = new QPushButton(searchDialog);
        btnClear->setObjectName("btnClear");

        horizontalLayout_footer->addWidget(btnClear);


        verticalLayout_root->addLayout(horizontalLayout_footer);


        retranslateUi(searchDialog);

        QMetaObject::connectSlotsByName(searchDialog);
    } // setupUi

    void retranslateUi(QWidget *searchDialog)
    {
        searchDialog->setWindowTitle(QCoreApplication::translate("searchDialog", "\346\220\234\347\264\242", nullptr));
        lblTitle->setText(QCoreApplication::translate("searchDialog", "\346\220\234\347\264\242", nullptr));
        btnClose->setText(QCoreApplication::translate("searchDialog", "\303\227", nullptr));
#if QT_CONFIG(tooltip)
        btnClose->setToolTip(QCoreApplication::translate("searchDialog", "\345\205\263\351\227\255", nullptr));
#endif // QT_CONFIG(tooltip)
        txtQuery->setPlaceholderText(QCoreApplication::translate("searchDialog", "\350\276\223\345\205\245\345\205\263\351\224\256\345\255\227", nullptr));
        comboScope->setItemText(0, QCoreApplication::translate("searchDialog", "\345\205\250\351\203\250", nullptr));
        comboScope->setItemText(1, QCoreApplication::translate("searchDialog", "\350\201\224\347\263\273\344\272\272", nullptr));
        comboScope->setItemText(2, QCoreApplication::translate("searchDialog", "\347\276\244\347\273\204", nullptr));
        comboScope->setItemText(3, QCoreApplication::translate("searchDialog", "\346\266\210\346\201\257", nullptr));

        btnSearch->setText(QCoreApplication::translate("searchDialog", "\346\220\234\347\264\242", nullptr));
        tabResults->setTabText(tabResults->indexOf(tabUsers), QCoreApplication::translate("searchDialog", "\350\201\224\347\263\273\344\272\272", nullptr));
        tabResults->setTabText(tabResults->indexOf(tabGroups), QCoreApplication::translate("searchDialog", "\347\276\244\347\273\204", nullptr));
        tabResults->setTabText(tabResults->indexOf(tabMessages), QCoreApplication::translate("searchDialog", "\346\266\210\346\201\257", nullptr));
        lblStatus->setText(QCoreApplication::translate("searchDialog", "\350\257\267\350\276\223\345\205\245\345\205\263\351\224\256\345\255\227\350\277\233\350\241\214\346\220\234\347\264\242", nullptr));
        btnClear->setText(QCoreApplication::translate("searchDialog", "\346\270\205\347\251\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class searchDialog: public Ui_searchDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHDIALOG_H
