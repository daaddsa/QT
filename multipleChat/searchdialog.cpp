#include "searchdialog.h"
#include "ui_searchdialog.h"

searchDialog::searchDialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::searchDialog)
{
    ui->setupUi(this);
}

searchDialog::~searchDialog()
{
    delete ui;
}
