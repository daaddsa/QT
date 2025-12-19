#include "patienteditview.h"
#include "ui_patienteditview.h"
#include "idatabase.h"
#include <QSqlTableModel>

patientEditView::patientEditView(QWidget *parent,int index)
    : QWidget(parent)
    , ui(new Ui::patientEditView)
{
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    QSqlTableModel *tabModel = iDatabase::getInstance().patientTabModel;
    dataMapper->setModel(iDatabase::getInstance().patientTabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->dbEditId, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName, tabModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbEditIDCard, tabModel->fieldIndex("IDCARD"));
    dataMapper->addMapping(ui->dbEditHeight, tabModel->fieldIndex("HEIGHT"));
    dataMapper->addMapping(ui->dbEditWeight, tabModel->fieldIndex("WEIGHT"));
    dataMapper->addMapping(ui->dbEditMobile, tabModel->fieldIndex("MOBILEPHONE"));
    dataMapper->addMapping(ui->dbEditDOB, tabModel->fieldIndex("DOB"));
    dataMapper->addMapping(ui->dccomboSex,tabModel->fieldIndex("SEX"));
    dataMapper->addMapping(ui->dbCreateTimeStamp,tabModel->fieldIndex("CREATETIMESTAMP"));

    dataMapper->setCurrentIndex(index);
}

patientEditView::~patientEditView()
{
    delete ui;
}

void patientEditView::on_btnSave_clicked()
{
    dataMapper->submit(); // Explicitly submit mapper data to model
    iDatabase::getInstance().submitPatientEdit();

    emit goPreviousView();
}


void patientEditView::on_pushButton_2_clicked()
{
    iDatabase::getInstance().revertPatientEdit();
    emit goPreviousView();
}

