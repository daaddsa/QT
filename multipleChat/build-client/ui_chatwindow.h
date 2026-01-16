/********************************************************************************
** Form generated from reading UI file 'chatwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWINDOW_H
#define UI_CHATWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_chatWindow
{
public:
    QVBoxLayout *verticalLayout_root;
    QFrame *frameHeader;
    QHBoxLayout *horizontalLayout_header;
    QVBoxLayout *verticalLayout_headerText;
    QLabel *lblChatTitle;
    QLabel *lblChatSubtitle;
    QSpacerItem *horizontalSpacer_header;
    QToolButton *btnHeaderInfo;
    QToolButton *btnHeaderClose;
    QSplitter *splitterChat;
    QWidget *widgetChat;
    QVBoxLayout *verticalLayout_chat;
    QTextBrowser *txtMessages;
    QFrame *frameComposer;
    QVBoxLayout *verticalLayout_composer;
    QHBoxLayout *horizontalLayout_tools;
    QToolButton *btnEmoji;
    QToolButton *btnImage;
    QToolButton *btnFile;
    QSpacerItem *horizontalSpacer_tools;
    QLabel *lblHint;
    QHBoxLayout *horizontalLayout_input;
    QTextEdit *txtInput;
    QPushButton *btnSend;
    QWidget *widgetParticipants;
    QVBoxLayout *verticalLayout_participants;
    QLabel *lblParticipantsTitle;
    QListWidget *listParticipants;

    void setupUi(QWidget *chatWindow)
    {
        if (chatWindow->objectName().isEmpty())
            chatWindow->setObjectName("chatWindow");
        chatWindow->resize(720, 560);
        verticalLayout_root = new QVBoxLayout(chatWindow);
        verticalLayout_root->setObjectName("verticalLayout_root");
        frameHeader = new QFrame(chatWindow);
        frameHeader->setObjectName("frameHeader");
        frameHeader->setFrameShape(QFrame::StyledPanel);
        horizontalLayout_header = new QHBoxLayout(frameHeader);
        horizontalLayout_header->setObjectName("horizontalLayout_header");
        verticalLayout_headerText = new QVBoxLayout();
        verticalLayout_headerText->setObjectName("verticalLayout_headerText");
        lblChatTitle = new QLabel(frameHeader);
        lblChatTitle->setObjectName("lblChatTitle");
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        lblChatTitle->setFont(font);

        verticalLayout_headerText->addWidget(lblChatTitle);

        lblChatSubtitle = new QLabel(frameHeader);
        lblChatSubtitle->setObjectName("lblChatSubtitle");

        verticalLayout_headerText->addWidget(lblChatSubtitle);


        horizontalLayout_header->addLayout(verticalLayout_headerText);

        horizontalSpacer_header = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_header->addItem(horizontalSpacer_header);

        btnHeaderInfo = new QToolButton(frameHeader);
        btnHeaderInfo->setObjectName("btnHeaderInfo");

        horizontalLayout_header->addWidget(btnHeaderInfo);

        btnHeaderClose = new QToolButton(frameHeader);
        btnHeaderClose->setObjectName("btnHeaderClose");

        horizontalLayout_header->addWidget(btnHeaderClose);


        verticalLayout_root->addWidget(frameHeader);

        splitterChat = new QSplitter(chatWindow);
        splitterChat->setObjectName("splitterChat");
        splitterChat->setOrientation(Qt::Horizontal);
        widgetChat = new QWidget(splitterChat);
        widgetChat->setObjectName("widgetChat");
        verticalLayout_chat = new QVBoxLayout(widgetChat);
        verticalLayout_chat->setObjectName("verticalLayout_chat");
        verticalLayout_chat->setContentsMargins(0, 0, 0, 0);
        txtMessages = new QTextBrowser(widgetChat);
        txtMessages->setObjectName("txtMessages");

        verticalLayout_chat->addWidget(txtMessages);

        frameComposer = new QFrame(widgetChat);
        frameComposer->setObjectName("frameComposer");
        frameComposer->setFrameShape(QFrame::StyledPanel);
        verticalLayout_composer = new QVBoxLayout(frameComposer);
        verticalLayout_composer->setObjectName("verticalLayout_composer");
        horizontalLayout_tools = new QHBoxLayout();
        horizontalLayout_tools->setObjectName("horizontalLayout_tools");
        btnEmoji = new QToolButton(frameComposer);
        btnEmoji->setObjectName("btnEmoji");

        horizontalLayout_tools->addWidget(btnEmoji);

        btnImage = new QToolButton(frameComposer);
        btnImage->setObjectName("btnImage");

        horizontalLayout_tools->addWidget(btnImage);

        btnFile = new QToolButton(frameComposer);
        btnFile->setObjectName("btnFile");

        horizontalLayout_tools->addWidget(btnFile);

        horizontalSpacer_tools = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_tools->addItem(horizontalSpacer_tools);

        lblHint = new QLabel(frameComposer);
        lblHint->setObjectName("lblHint");
        lblHint->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        horizontalLayout_tools->addWidget(lblHint);


        verticalLayout_composer->addLayout(horizontalLayout_tools);

        horizontalLayout_input = new QHBoxLayout();
        horizontalLayout_input->setObjectName("horizontalLayout_input");
        txtInput = new QTextEdit(frameComposer);
        txtInput->setObjectName("txtInput");
        txtInput->setMinimumSize(QSize(0, 90));

        horizontalLayout_input->addWidget(txtInput);

        btnSend = new QPushButton(frameComposer);
        btnSend->setObjectName("btnSend");
        btnSend->setMinimumSize(QSize(90, 90));

        horizontalLayout_input->addWidget(btnSend);


        verticalLayout_composer->addLayout(horizontalLayout_input);


        verticalLayout_chat->addWidget(frameComposer);

        splitterChat->addWidget(widgetChat);
        widgetParticipants = new QWidget(splitterChat);
        widgetParticipants->setObjectName("widgetParticipants");
        widgetParticipants->setMinimumSize(QSize(220, 0));
        verticalLayout_participants = new QVBoxLayout(widgetParticipants);
        verticalLayout_participants->setObjectName("verticalLayout_participants");
        verticalLayout_participants->setContentsMargins(0, 0, 0, 0);
        lblParticipantsTitle = new QLabel(widgetParticipants);
        lblParticipantsTitle->setObjectName("lblParticipantsTitle");
        QFont font1;
        font1.setBold(true);
        lblParticipantsTitle->setFont(font1);

        verticalLayout_participants->addWidget(lblParticipantsTitle);

        listParticipants = new QListWidget(widgetParticipants);
        listParticipants->setObjectName("listParticipants");

        verticalLayout_participants->addWidget(listParticipants);

        splitterChat->addWidget(widgetParticipants);

        verticalLayout_root->addWidget(splitterChat);


        retranslateUi(chatWindow);

        QMetaObject::connectSlotsByName(chatWindow);
    } // setupUi

    void retranslateUi(QWidget *chatWindow)
    {
        chatWindow->setWindowTitle(QCoreApplication::translate("chatWindow", "\345\257\271\350\257\235", nullptr));
        lblChatTitle->setText(QCoreApplication::translate("chatWindow", "\347\276\244\350\201\212", nullptr));
        lblChatSubtitle->setText(QCoreApplication::translate("chatWindow", "\345\234\250\347\272\277\357\274\2320", nullptr));
        btnHeaderInfo->setText(QCoreApplication::translate("chatWindow", "\344\277\241\346\201\257", nullptr));
#if QT_CONFIG(tooltip)
        btnHeaderInfo->setToolTip(QCoreApplication::translate("chatWindow", "\346\237\245\347\234\213\344\274\232\350\257\235\344\277\241\346\201\257", nullptr));
#endif // QT_CONFIG(tooltip)
        btnHeaderClose->setText(QCoreApplication::translate("chatWindow", "\345\205\263\351\227\255", nullptr));
#if QT_CONFIG(tooltip)
        btnHeaderClose->setToolTip(QCoreApplication::translate("chatWindow", "\345\205\263\351\227\255\345\257\271\350\257\235\347\252\227\345\217\243", nullptr));
#endif // QT_CONFIG(tooltip)
        txtMessages->setPlaceholderText(QCoreApplication::translate("chatWindow", "\346\266\210\346\201\257\350\256\260\345\275\225", nullptr));
        btnEmoji->setText(QCoreApplication::translate("chatWindow", "\350\241\250\346\203\205", nullptr));
        btnImage->setText(QCoreApplication::translate("chatWindow", "\345\233\276\347\211\207", nullptr));
        btnFile->setText(QCoreApplication::translate("chatWindow", "\346\226\207\344\273\266", nullptr));
        lblHint->setText(QCoreApplication::translate("chatWindow", "Ctrl+Enter \345\217\221\351\200\201", nullptr));
        txtInput->setPlaceholderText(QCoreApplication::translate("chatWindow", "\350\276\223\345\205\245\346\266\210\346\201\257", nullptr));
        btnSend->setText(QCoreApplication::translate("chatWindow", "\345\217\221\351\200\201", nullptr));
        lblParticipantsTitle->setText(QCoreApplication::translate("chatWindow", "\346\210\220\345\221\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class chatWindow: public Ui_chatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWINDOW_H
