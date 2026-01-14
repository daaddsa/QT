/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_root;
    QSplitter *splitterMain;
    QWidget *widgetSidebar;
    QVBoxLayout *verticalLayout_sidebar;
    QFrame *frameProfile;
    QHBoxLayout *horizontalLayout_profile;
    QLabel *lblAvatar;
    QVBoxLayout *verticalLayout_profileText;
    QLabel *lblNickname;
    QLabel *lblStatus;
    QSpacerItem *horizontalSpacer_profile;
    QLineEdit *txtSearch;
    QTabWidget *tabNav;
    QWidget *tabContacts;
    QVBoxLayout *verticalLayout_contacts;
    QTreeWidget *treeContacts;
    QWidget *tabGroups;
    QVBoxLayout *verticalLayout_groups;
    QListWidget *listGroups;
    QWidget *tabRecent;
    QVBoxLayout *verticalLayout_recent;
    QListWidget *listRecent;
    QHBoxLayout *horizontalLayout_sidebarButtons;
    QPushButton *btnAdd;
    QPushButton *btnCreateGroup;
    QPushButton *btnSettings;
    QWidget *widgetContent;
    QVBoxLayout *verticalLayout_content;
    QLabel *lblConversationTitle;
    QTextBrowser *txtChatHistory;
    QHBoxLayout *horizontalLayout_input;
    QTextEdit *txtMessageInput;
    QPushButton *btnSend;
    QGroupBox *groupBoxLog;
    QVBoxLayout *verticalLayout_log;
    QTextBrowser *txtClientLog;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(980, 640);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_root = new QHBoxLayout(centralwidget);
        horizontalLayout_root->setObjectName("horizontalLayout_root");
        splitterMain = new QSplitter(centralwidget);
        splitterMain->setObjectName("splitterMain");
        splitterMain->setOrientation(Qt::Horizontal);
        widgetSidebar = new QWidget(splitterMain);
        widgetSidebar->setObjectName("widgetSidebar");
        widgetSidebar->setMinimumSize(QSize(280, 0));
        verticalLayout_sidebar = new QVBoxLayout(widgetSidebar);
        verticalLayout_sidebar->setObjectName("verticalLayout_sidebar");
        verticalLayout_sidebar->setContentsMargins(0, 0, 0, 0);
        frameProfile = new QFrame(widgetSidebar);
        frameProfile->setObjectName("frameProfile");
        frameProfile->setFrameShape(QFrame::StyledPanel);
        horizontalLayout_profile = new QHBoxLayout(frameProfile);
        horizontalLayout_profile->setObjectName("horizontalLayout_profile");
        lblAvatar = new QLabel(frameProfile);
        lblAvatar->setObjectName("lblAvatar");
        lblAvatar->setMinimumSize(QSize(40, 40));
        lblAvatar->setMaximumSize(QSize(40, 40));
        lblAvatar->setAlignment(Qt::AlignCenter);

        horizontalLayout_profile->addWidget(lblAvatar);

        verticalLayout_profileText = new QVBoxLayout();
        verticalLayout_profileText->setObjectName("verticalLayout_profileText");
        lblNickname = new QLabel(frameProfile);
        lblNickname->setObjectName("lblNickname");
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        lblNickname->setFont(font);

        verticalLayout_profileText->addWidget(lblNickname);

        lblStatus = new QLabel(frameProfile);
        lblStatus->setObjectName("lblStatus");

        verticalLayout_profileText->addWidget(lblStatus);


        horizontalLayout_profile->addLayout(verticalLayout_profileText);

        horizontalSpacer_profile = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_profile->addItem(horizontalSpacer_profile);


        verticalLayout_sidebar->addWidget(frameProfile);

        txtSearch = new QLineEdit(widgetSidebar);
        txtSearch->setObjectName("txtSearch");

        verticalLayout_sidebar->addWidget(txtSearch);

        tabNav = new QTabWidget(widgetSidebar);
        tabNav->setObjectName("tabNav");
        tabContacts = new QWidget();
        tabContacts->setObjectName("tabContacts");
        verticalLayout_contacts = new QVBoxLayout(tabContacts);
        verticalLayout_contacts->setObjectName("verticalLayout_contacts");
        treeContacts = new QTreeWidget(tabContacts);
        treeContacts->setObjectName("treeContacts");
        treeContacts->setHeaderHidden(true);

        verticalLayout_contacts->addWidget(treeContacts);

        tabNav->addTab(tabContacts, QString());
        tabGroups = new QWidget();
        tabGroups->setObjectName("tabGroups");
        verticalLayout_groups = new QVBoxLayout(tabGroups);
        verticalLayout_groups->setObjectName("verticalLayout_groups");
        listGroups = new QListWidget(tabGroups);
        listGroups->setObjectName("listGroups");

        verticalLayout_groups->addWidget(listGroups);

        tabNav->addTab(tabGroups, QString());
        tabRecent = new QWidget();
        tabRecent->setObjectName("tabRecent");
        verticalLayout_recent = new QVBoxLayout(tabRecent);
        verticalLayout_recent->setObjectName("verticalLayout_recent");
        listRecent = new QListWidget(tabRecent);
        listRecent->setObjectName("listRecent");

        verticalLayout_recent->addWidget(listRecent);

        tabNav->addTab(tabRecent, QString());

        verticalLayout_sidebar->addWidget(tabNav);

        horizontalLayout_sidebarButtons = new QHBoxLayout();
        horizontalLayout_sidebarButtons->setObjectName("horizontalLayout_sidebarButtons");
        btnAdd = new QPushButton(widgetSidebar);
        btnAdd->setObjectName("btnAdd");

        horizontalLayout_sidebarButtons->addWidget(btnAdd);

        btnCreateGroup = new QPushButton(widgetSidebar);
        btnCreateGroup->setObjectName("btnCreateGroup");

        horizontalLayout_sidebarButtons->addWidget(btnCreateGroup);

        btnSettings = new QPushButton(widgetSidebar);
        btnSettings->setObjectName("btnSettings");

        horizontalLayout_sidebarButtons->addWidget(btnSettings);


        verticalLayout_sidebar->addLayout(horizontalLayout_sidebarButtons);

        splitterMain->addWidget(widgetSidebar);
        widgetContent = new QWidget(splitterMain);
        widgetContent->setObjectName("widgetContent");
        verticalLayout_content = new QVBoxLayout(widgetContent);
        verticalLayout_content->setObjectName("verticalLayout_content");
        verticalLayout_content->setContentsMargins(0, 0, 0, 0);
        lblConversationTitle = new QLabel(widgetContent);
        lblConversationTitle->setObjectName("lblConversationTitle");
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        lblConversationTitle->setFont(font1);

        verticalLayout_content->addWidget(lblConversationTitle);

        txtChatHistory = new QTextBrowser(widgetContent);
        txtChatHistory->setObjectName("txtChatHistory");

        verticalLayout_content->addWidget(txtChatHistory);

        horizontalLayout_input = new QHBoxLayout();
        horizontalLayout_input->setObjectName("horizontalLayout_input");
        txtMessageInput = new QTextEdit(widgetContent);
        txtMessageInput->setObjectName("txtMessageInput");
        txtMessageInput->setMinimumSize(QSize(0, 70));

        horizontalLayout_input->addWidget(txtMessageInput);

        btnSend = new QPushButton(widgetContent);
        btnSend->setObjectName("btnSend");
        btnSend->setMinimumSize(QSize(90, 70));

        horizontalLayout_input->addWidget(btnSend);


        verticalLayout_content->addLayout(horizontalLayout_input);

        groupBoxLog = new QGroupBox(widgetContent);
        groupBoxLog->setObjectName("groupBoxLog");
        verticalLayout_log = new QVBoxLayout(groupBoxLog);
        verticalLayout_log->setObjectName("verticalLayout_log");
        txtClientLog = new QTextBrowser(groupBoxLog);
        txtClientLog->setObjectName("txtClientLog");

        verticalLayout_log->addWidget(txtClientLog);


        verticalLayout_content->addWidget(groupBoxLog);

        splitterMain->addWidget(widgetContent);

        horizontalLayout_root->addWidget(splitterMain);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 980, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabNav->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MultipleChat", nullptr));
        lblAvatar->setText(QCoreApplication::translate("MainWindow", "\345\244\264\345\203\217", nullptr));
        lblNickname->setText(QCoreApplication::translate("MainWindow", "\346\234\252\347\231\273\345\275\225", nullptr));
        lblStatus->setText(QCoreApplication::translate("MainWindow", "\347\246\273\347\272\277", nullptr));
        txtSearch->setPlaceholderText(QCoreApplication::translate("MainWindow", "\346\220\234\347\264\242\345\245\275\345\217\213/\347\276\244\347\273\204", nullptr));
        tabNav->setTabText(tabNav->indexOf(tabContacts), QCoreApplication::translate("MainWindow", "\345\245\275\345\217\213", nullptr));
        tabNav->setTabText(tabNav->indexOf(tabGroups), QCoreApplication::translate("MainWindow", "\347\276\244\347\273\204", nullptr));
        tabNav->setTabText(tabNav->indexOf(tabRecent), QCoreApplication::translate("MainWindow", "\346\234\200\350\277\221", nullptr));
        btnAdd->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240", nullptr));
        btnCreateGroup->setText(QCoreApplication::translate("MainWindow", "\345\273\272\347\276\244", nullptr));
        btnSettings->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        lblConversationTitle->setText(QCoreApplication::translate("MainWindow", "\347\276\244\350\201\212", nullptr));
        txtChatHistory->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\201\212\345\244\251\350\256\260\345\275\225", nullptr));
        txtMessageInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\346\266\210\346\201\257\357\274\214\345\233\236\350\275\246\345\217\221\351\200\201\357\274\210\345\220\216\347\273\255\345\217\257\345\256\236\347\216\260\357\274\211", nullptr));
        btnSend->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        groupBoxLog->setTitle(QCoreApplication::translate("MainWindow", "\350\277\220\350\241\214\346\227\245\345\277\227", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
