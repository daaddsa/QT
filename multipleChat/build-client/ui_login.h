/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_title;
    QSpacerItem *verticalSpacer_1;
    QFormLayout *formLayout;
    QLabel *label_server;
    QLineEdit *txtServerIP;
    QLabel *label_port;
    QLineEdit *txtPort;
    QLabel *label_user;
    QLineEdit *txtUsername;
    QSpacerItem *verticalSpacer_2;
    QPushButton *btnLogin;
    QPushButton *btnCancel;

    void setupUi(QWidget *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName("login");
        login->resize(350, 280);
        verticalLayout = new QVBoxLayout(login);
        verticalLayout->setObjectName("verticalLayout");
        label_title = new QLabel(login);
        label_title->setObjectName("label_title");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        label_title->setFont(font);
        label_title->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_title);

        verticalSpacer_1 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_1);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_server = new QLabel(login);
        label_server->setObjectName("label_server");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_server);

        txtServerIP = new QLineEdit(login);
        txtServerIP->setObjectName("txtServerIP");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, txtServerIP);

        label_port = new QLabel(login);
        label_port->setObjectName("label_port");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_port);

        txtPort = new QLineEdit(login);
        txtPort->setObjectName("txtPort");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, txtPort);

        label_user = new QLabel(login);
        label_user->setObjectName("label_user");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_user);

        txtUsername = new QLineEdit(login);
        txtUsername->setObjectName("txtUsername");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, txtUsername);


        verticalLayout->addLayout(formLayout);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        btnLogin = new QPushButton(login);
        btnLogin->setObjectName("btnLogin");

        verticalLayout->addWidget(btnLogin);

        btnCancel = new QPushButton(login);
        btnCancel->setObjectName("btnCancel");

        verticalLayout->addWidget(btnCancel);


        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QWidget *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "MultipleChat Login", nullptr));
        label_title->setText(QCoreApplication::translate("login", "MultipleChat \347\231\273\345\275\225", nullptr));
        label_server->setText(QCoreApplication::translate("login", "\346\234\215\345\212\241\345\231\250 IP:", nullptr));
        txtServerIP->setText(QCoreApplication::translate("login", "127.0.0.1", nullptr));
        label_port->setText(QCoreApplication::translate("login", "\347\253\257\345\217\243:", nullptr));
        txtPort->setText(QCoreApplication::translate("login", "12345", nullptr));
        label_user->setText(QCoreApplication::translate("login", "\347\224\250\346\210\267\345\220\215:", nullptr));
        txtUsername->setPlaceholderText(QCoreApplication::translate("login", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\346\230\265\347\247\260", nullptr));
        btnLogin->setText(QCoreApplication::translate("login", "\347\231\273 \345\275\225", nullptr));
        btnCancel->setText(QCoreApplication::translate("login", "\345\217\226 \346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
