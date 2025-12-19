#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QDataWidgetMapper>

class iDatabase : public QObject
{
    Q_OBJECT
public:

    static iDatabase &getInstance(){
        static iDatabase instance;
        return instance;
    }

    QString userLogin(QString userName,QString password);

private:
    explicit iDatabase(QObject *parent = nullptr);
    iDatabase(iDatabase const &) = delete;
    void operator = (iDatabase const &) = delete;

    QSqlDatabase database;

    void ininDatabase();



signals:

public:
    bool initPatientModel();
    int addNewPatient();
    bool searchPatient(QString filter);
    bool deleteCurrentPatient();
    bool submitPatientEdit();
    bool revertPatientEdit();

    QSqlTableModel *patientTabModel;
    QItemSelectionModel *thePatientSelection;


};

#endif // IDATABASE_H
