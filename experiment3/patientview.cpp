#include "patientview.h"
#include "ui_patientview.h"
#include "idatabase.h"

patientview::patientview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientview)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode (QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    iDatabase &iDatabase = iDatabase:: getInstance();
    if(iDatabase.initPatientModel()){
        ui->tableView->setModel(iDatabase.patientTabModel);
        ui->tableView->setSelectionModel(iDatabase.thePatientSelection);
    }
}

patientview::~patientview()
{
    delete ui;
}

void patientview::on_btnAdd_clicked()
{
    int currow = iDatabase::getInstance().addNewPatient();
    emit goPatientEditView(currow);
}


void patientview::on_btnSearch_clicked()
{
    QString filter = QString("name like '%%1%'").arg(ui->lineEdit->text());
    iDatabase::getInstance().searchPatient(filter);
}


void patientview::on_btnEdit_clicked()
{
    QModelIndex curIndex = iDatabase::getInstance().thePatientSelection->currentIndex();

    emit goPatientEditView(curIndex.row());
}


void patientview::on_btnDelete_clicked()
{
    iDatabase::getInstance().deleteCurrentPatient();
}

