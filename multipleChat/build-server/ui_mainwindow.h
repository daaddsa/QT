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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_main;
    QTabWidget *tabWidget;
    QWidget *tabMonitor;
    QVBoxLayout *verticalLayout_monitor;
    QHBoxLayout *horizontalLayout_control;
    QLabel *label_port;
    QLineEdit *txtPort;
    QPushButton *btnStartStop;
    QSpacerItem *horizontalSpacer;
    QLabel *label_online;
    QLCDNumber *lcdOnline;
    QGroupBox *groupBox_log;
    QVBoxLayout *verticalLayout_log;
    QTextBrowser *txtLog;
    QWidget *tabUsers;
    QVBoxLayout *verticalLayout_users;
    QTableWidget *tblUsers;
    QHBoxLayout *horizontalLayout_user_actions;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnKick;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(494, 302);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_main = new QVBoxLayout(centralwidget);
        verticalLayout_main->setObjectName("verticalLayout_main");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabMonitor = new QWidget();
        tabMonitor->setObjectName("tabMonitor");
        verticalLayout_monitor = new QVBoxLayout(tabMonitor);
        verticalLayout_monitor->setObjectName("verticalLayout_monitor");
        horizontalLayout_control = new QHBoxLayout();
        horizontalLayout_control->setObjectName("horizontalLayout_control");
        label_port = new QLabel(tabMonitor);
        label_port->setObjectName("label_port");

        horizontalLayout_control->addWidget(label_port);

        txtPort = new QLineEdit(tabMonitor);
        txtPort->setObjectName("txtPort");
        txtPort->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_control->addWidget(txtPort);

        btnStartStop = new QPushButton(tabMonitor);
        btnStartStop->setObjectName("btnStartStop");

        horizontalLayout_control->addWidget(btnStartStop);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_control->addItem(horizontalSpacer);

        label_online = new QLabel(tabMonitor);
        label_online->setObjectName("label_online");

        horizontalLayout_control->addWidget(label_online);

        lcdOnline = new QLCDNumber(tabMonitor);
        lcdOnline->setObjectName("lcdOnline");
        lcdOnline->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);

        horizontalLayout_control->addWidget(lcdOnline);


        verticalLayout_monitor->addLayout(horizontalLayout_control);

        groupBox_log = new QGroupBox(tabMonitor);
        groupBox_log->setObjectName("groupBox_log");
        verticalLayout_log = new QVBoxLayout(groupBox_log);
        verticalLayout_log->setObjectName("verticalLayout_log");
        txtLog = new QTextBrowser(groupBox_log);
        txtLog->setObjectName("txtLog");

        verticalLayout_log->addWidget(txtLog);


        verticalLayout_monitor->addWidget(groupBox_log);

        tabWidget->addTab(tabMonitor, QString());
        tabUsers = new QWidget();
        tabUsers->setObjectName("tabUsers");
        verticalLayout_users = new QVBoxLayout(tabUsers);
        verticalLayout_users->setObjectName("verticalLayout_users");
        tblUsers = new QTableWidget(tabUsers);
        if (tblUsers->columnCount() < 5)
            tblUsers->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblUsers->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblUsers->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tblUsers->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tblUsers->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tblUsers->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tblUsers->setObjectName("tblUsers");
        tblUsers->setColumnCount(5);

        verticalLayout_users->addWidget(tblUsers);

        horizontalLayout_user_actions = new QHBoxLayout();
        horizontalLayout_user_actions->setObjectName("horizontalLayout_user_actions");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_user_actions->addItem(horizontalSpacer_2);

        btnKick = new QPushButton(tabUsers);
        btnKick->setObjectName("btnKick");

        horizontalLayout_user_actions->addWidget(btnKick);


        verticalLayout_users->addLayout(horizontalLayout_user_actions);

        tabWidget->addTab(tabUsers, QString());

        verticalLayout_main->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MultipleChat Server Console", nullptr));
        label_port->setText(QCoreApplication::translate("MainWindow", "\347\233\221\345\220\254\347\253\257\345\217\243:", nullptr));
        txtPort->setText(QCoreApplication::translate("MainWindow", "12345", nullptr));
        btnStartStop->setText(QCoreApplication::translate("MainWindow", "\345\220\257\345\212\250\346\234\215\345\212\241", nullptr));
        label_online->setText(QCoreApplication::translate("MainWindow", "\345\234\250\347\272\277\344\272\272\346\225\260:", nullptr));
        groupBox_log->setTitle(QCoreApplication::translate("MainWindow", "\347\263\273\347\273\237\346\227\245\345\277\227", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMonitor), QCoreApplication::translate("MainWindow", "\350\277\220\350\241\214\347\233\221\346\216\247", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblUsers->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblUsers->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "\350\264\246\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblUsers->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "\346\230\265\347\247\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblUsers->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "\347\231\273\345\275\225\346\227\266\351\227\264", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblUsers->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201", nullptr));
        btnKick->setText(QCoreApplication::translate("MainWindow", "\345\274\272\345\210\266\344\270\213\347\272\277 ", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabUsers), QCoreApplication::translate("MainWindow", "\347\224\250\346\210\267\347\256\241\347\220\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
