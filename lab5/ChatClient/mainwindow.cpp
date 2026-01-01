#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QJsonValue>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
    m_chatClient = new chatClient(this);

    connect(m_chatClient, &chatClient::connected, this, &MainWindow::connectedToServer);
    connect(m_chatClient, &chatClient::messageReceived, this, &MainWindow::messageReceived);
    connect(m_chatClient, &chatClient::jsonReceived, this, &MainWindow::jsonReceived);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    m_chatClient->connectToServer(QHostAddress(ui->serverEdit->text()),1967);
}


void MainWindow::on_sayButton_clicked()
{
    if(!ui->sayLineEdit->text().isEmpty())
        m_chatClient->sendMessage(ui->sayLineEdit->text());
}


void MainWindow::on_logoutButton_clicked()
{
    m_chatClient->disconnectFromHost();
    ui->stackedWidget->setCurrentWidget(ui->loginPage);

    for (auto item : ui->userListWidget->findItems(ui->usernameEdit->text(), Qt::MatchExactly)) {
        delete item;
    }
}

void MainWindow::connectedToServer()
{
    ui->stackedWidget->setCurrentWidget(ui->chatPage);
    m_chatClient->sendMessage(ui->usernameEdit->text(),"login");
}

void MainWindow::messageReceived(const QString &sender, const QString &text)
{
    ui->roomTextEdit->append(QString("%1 : %2").arg(sender).arg(text));
}

void MainWindow::jsonReceived(const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value("type");
    if (typeVal.isNull() || !typeVal.isString())
        return;
    
    // Debug log
    // ui->roomTextEdit->append("Debug: Recv " + typeVal.toString());

    if (typeVal.toString().compare("message", Qt::CaseInsensitive) == 0){
        const QJsonValue textVal = docObj.value("text");
        const QJsonValue senderVal = docObj.value("sender");

        if (textVal.isNull() || !textVal.isString())
            return;

        if (senderVal.isNull() || !senderVal.isString())
            return;

        messageReceived(senderVal.toString(),textVal.toString());

    } else if (typeVal.toString().compare("newuser", Qt::CaseInsensitive) == 0) {
        const QJsonValue usernameVal = docObj.value("username");
        if (usernameVal.isNull() || !usernameVal.isString())
            return;

        userJoined(usernameVal.toString());
    } else if (typeVal.toString().compare("userlist", Qt::CaseInsensitive) == 0) {
        const QJsonValue userListVal = docObj.value("userlist");
        if (userListVal.isNull() || !userListVal.isArray())
            return;
        
        QJsonArray userArray = userListVal.toArray();
        for (const QJsonValue &val : userArray) {
            if (val.isString()) {
                userJoined(val.toString());
            }
        }
    } else if(typeVal.toString().compare("userdisconnected",Qt::CaseInsensitive) == 0){
        const QJsonValue usernameVal = docObj.value("username");
        if(usernameVal.isNull() || !usernameVal.isString())
            return;

        userLeft(usernameVal.toString());
    } else if (typeVal.toString().compare("system", Qt::CaseInsensitive) == 0) {
        // Handle system/debug messages
        const QJsonValue msgVal = docObj.value("message");
        if (!msgVal.isNull() && msgVal.isString()) {
             ui->roomTextEdit->append("System: " + msgVal.toString());
        }
    }
}

void MainWindow::userJoined(const QString &user)
{
    if (user.isEmpty()) return;
    
    // Manual check for duplicates to avoid findItems issues
    bool found = false;
    for(int i = 0; i < ui->userListWidget->count(); ++i) {
        if (ui->userListWidget->item(i)->text() == user) {
            found = true;
            break;
        }
    }
    
    if (!found) {
        ui->userListWidget->addItem(user);
        ui->roomTextEdit->append(QString("System: User %1 joined.").arg(user));
    }
}

void MainWindow::userLeft(const QString &user)
{
    for (auto item : ui->userListWidget->findItems(user, Qt::MatchExactly)) {
        delete item;
    }
}

