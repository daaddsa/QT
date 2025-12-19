#include "idatabase.h"
#include <QUuid>

void iDatabase::ininDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    QString aFile = "D:/Users/Lenovo/Desktop/Library/QTapplication/实验3/database/experiment3.db";
    database.setDatabaseName(aFile);

    if(!database.open()){
        qDebug() << "failed to open database";
    } else
        qDebug() << "open database is ok";
}

bool iDatabase::initPatientModel()
{
    patientTabModel = new QSqlTableModel(this,database);
    patientTabModel->setTable("patient");
    patientTabModel->setEditStrategy(
        QSqlTableModel::OnManualSubmit);
    patientTabModel->setSort(patientTabModel->fieldIndex("name"),Qt::AscendingOrder);
    if(!(patientTabModel->select()))
        return false;

    thePatientSelection = new QItemSelectionModel(patientTabModel);
    return true;
}

int iDatabase::addNewPatient()
{
    patientTabModel->insertRows(patientTabModel->rowCount(),1);
    QModelIndex curIndex = patientTabModel->index(patientTabModel->rowCount()-1,1);

    int curRecNo = curIndex.row();
    QSqlRecord curRec = patientTabModel->record(curRecNo);
    curRec.setValue("CREATEDTIMESTAMP",QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithoutBraces));

    patientTabModel->setRecord(curRecNo,curRec);

    return curIndex.row();
}

bool iDatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

bool iDatabase::deleteCurrentPatient()
{
    QModelIndex curIndex = thePatientSelection->currentIndex();
    if (!curIndex.isValid()) {
        return false;
    }
    patientTabModel->removeRow(curIndex.row());
    bool success = patientTabModel->submitAll();
    patientTabModel->select();
    return success;
}

bool iDatabase::submitPatientEdit()
{
    return patientTabModel->submitAll();
}

bool iDatabase::revertPatientEdit()
{
    patientTabModel->revertAll();
    return true;
}

QString iDatabase::userLogin(QString userName, QString password)
{
    QSqlQuery query(database); // Use the member database connection
    if (!database.isOpen()) {
        qDebug() << "Database is not open in userLogin";
        return "loginFailed";
    }

    query.prepare("select username,password from user where username = :USER");
    query.bindValue(":USER",userName);
    if (!query.exec()) {
        qDebug() << "Query execution failed:" << query.lastError().text();
        return "loginFailed";
    }

    if(query.first() && query.value("username").isValid()){
        QString passwd = query.value("password").toString();
        if(passwd == password){
            qDebug() << "Login successful";
            return "loginOk"; // Note: Case sensitivity check "loginOK" vs "loginOk"
        }else{
            qDebug() << "Wrong password";
            return "wrongPassword";
        }
    }else{
        qDebug() << "no such user";
        return "wrongUsername";
    }
}

iDatabase::iDatabase(QObject *parent) : QObject(parent)
{
    ininDatabase();
}
