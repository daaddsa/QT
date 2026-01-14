/********************************************************************************
** Form generated from reading UI file 'signup.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNUP_H
#define UI_SIGNUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_signUp
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_title;
    QSpacerItem *verticalSpacer_1;
    QFormLayout *formLayout;
    QLabel *label_account;
    QLineEdit *txtAccount;
    QLabel *label_nickname;
    QLineEdit *txtNickname;
    QLabel *label_password;
    QLineEdit *txtPassword;
    QLabel *label_confirm;
    QLineEdit *txtConfirmPassword;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_buttons;
    QSpacerItem *horizontalSpacer_left;
    QPushButton *btnRegister;
    QPushButton *btnBackToLogin;
    QSpacerItem *horizontalSpacer_right;

    void setupUi(QWidget *signUp)
    {
        if (signUp->objectName().isEmpty())
            signUp->setObjectName("signUp");
        signUp->resize(380, 360);
        verticalLayout = new QVBoxLayout(signUp);
        verticalLayout->setObjectName("verticalLayout");
        label_title = new QLabel(signUp);
        label_title->setObjectName("label_title");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        label_title->setFont(font);
        label_title->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_title);

        verticalSpacer_1 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_1);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_account = new QLabel(signUp);
        label_account->setObjectName("label_account");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_account);

        txtAccount = new QLineEdit(signUp);
        txtAccount->setObjectName("txtAccount");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, txtAccount);

        label_nickname = new QLabel(signUp);
        label_nickname->setObjectName("label_nickname");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_nickname);

        txtNickname = new QLineEdit(signUp);
        txtNickname->setObjectName("txtNickname");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, txtNickname);

        label_password = new QLabel(signUp);
        label_password->setObjectName("label_password");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_password);

        txtPassword = new QLineEdit(signUp);
        txtPassword->setObjectName("txtPassword");
        txtPassword->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, txtPassword);

        label_confirm = new QLabel(signUp);
        label_confirm->setObjectName("label_confirm");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_confirm);

        txtConfirmPassword = new QLineEdit(signUp);
        txtConfirmPassword->setObjectName("txtConfirmPassword");
        txtConfirmPassword->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, txtConfirmPassword);


        verticalLayout->addLayout(formLayout);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_buttons = new QHBoxLayout();
        horizontalLayout_buttons->setObjectName("horizontalLayout_buttons");
        horizontalSpacer_left = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_buttons->addItem(horizontalSpacer_left);

        btnRegister = new QPushButton(signUp);
        btnRegister->setObjectName("btnRegister");
        btnRegister->setMinimumSize(QSize(100, 0));

        horizontalLayout_buttons->addWidget(btnRegister);

        btnBackToLogin = new QPushButton(signUp);
        btnBackToLogin->setObjectName("btnBackToLogin");
        btnBackToLogin->setMinimumSize(QSize(100, 0));

        horizontalLayout_buttons->addWidget(btnBackToLogin);

        horizontalSpacer_right = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_buttons->addItem(horizontalSpacer_right);


        verticalLayout->addLayout(horizontalLayout_buttons);


        retranslateUi(signUp);

        QMetaObject::connectSlotsByName(signUp);
    } // setupUi

    void retranslateUi(QWidget *signUp)
    {
        signUp->setWindowTitle(QCoreApplication::translate("signUp", "MultipleChat \346\263\250\345\206\214", nullptr));
        label_title->setText(QCoreApplication::translate("signUp", "\345\210\233\345\273\272\346\226\260\350\264\246\345\217\267", nullptr));
        label_account->setText(QCoreApplication::translate("signUp", "\350\264\246\345\217\267", nullptr));
        txtAccount->setPlaceholderText(QCoreApplication::translate("signUp", "\350\257\267\350\276\223\345\205\245\350\264\246\345\217\267", nullptr));
        label_nickname->setText(QCoreApplication::translate("signUp", "\346\230\265\347\247\260", nullptr));
        txtNickname->setPlaceholderText(QCoreApplication::translate("signUp", "\350\257\267\350\276\223\345\205\245\346\230\265\347\247\260", nullptr));
        label_password->setText(QCoreApplication::translate("signUp", "\345\257\206\347\240\201", nullptr));
        txtPassword->setPlaceholderText(QCoreApplication::translate("signUp", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        label_confirm->setText(QCoreApplication::translate("signUp", "\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        txtConfirmPassword->setPlaceholderText(QCoreApplication::translate("signUp", "\345\206\215\346\254\241\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        btnRegister->setText(QCoreApplication::translate("signUp", "\347\253\213\345\215\263\346\263\250\345\206\214", nullptr));
        btnBackToLogin->setText(QCoreApplication::translate("signUp", "\350\277\224\345\233\236\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class signUp: public Ui_signUp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNUP_H
