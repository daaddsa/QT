#include "masterview.h"
#include "ui_masterview.h"
#include <QDebug>
#include "idatabase.h"

masterView::masterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::masterView)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    goLoginView();

    iDatabase::getInstance();
}

masterView::~masterView()
{
    delete ui;
}

void masterView::goLoginView()
{
    qDebug() << "goLoginView";
    loginView = new LoginView(this);
    pushWidgetToStackView(loginView);

    connect(loginView,SIGNAL(loginSuccess()),this,SLOT(goWelcomeView()));
}

void masterView::goWelcomeView()
{
    qDebug() << "goWelcomeView";
    welcomeView = new WelcomeView(this);
    pushWidgetToStackView(welcomeView);

    connect(welcomeView,SIGNAL(goDoctorView()),this,SLOT(goDoctorView()));
    connect(welcomeView,SIGNAL(goPatientView()),this,SLOT(goPatientView()));
    connect(welcomeView,SIGNAL(goDepartmentView()),this,SLOT(goDepartmentView()));
}

void masterView::goDoctorView()
{
    qDebug() << "goDoctorView";
    doctorView = new DoctorView(this);
    pushWidgetToStackView(doctorView);
}

void masterView::goDepartmentView()
{
    qDebug() << "goDepartmentView";
    departmentView = new DepartmentView(this);
    pushWidgetToStackView(departmentView);
}

void masterView::goPatientEditView(int rowNo)
{
    qDebug() << "goPatientEditView";
    m_patientEditView = new patientEditView(this,rowNo);
    pushWidgetToStackView(m_patientEditView);

    connect(m_patientEditView,SIGNAL(goPreviousView()),this,SLOT(goPreviousView()));
}

void masterView::goPatientView()
{
    qDebug() << "goPatientView";
    patientView = new patientview(this);
    pushWidgetToStackView(patientView);

    connect(patientView,SIGNAL(goPatientEditView(int)),this,SLOT(goPatientEditView(int)));
}

void masterView::goPreviousView()
{
    int count = ui->stackedWidget->count();

    if(count > 1)
    {
        ui->stackedWidget->setCurrentIndex(count - 2);
        ui->labelTitle->setText(ui->stackedWidget->currentWidget()->windowTitle());

        QWidget * widget = ui->stackedWidget->widget(count - 1 );
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
}

void masterView::pushWidgetToStackView(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
    int count = ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count - 1);
    ui->labelTitle->setText(widget->windowTitle());
}

void masterView::on_btnBack_clicked()
{
    goPreviousView();
}


void masterView::on_stackedWidget_currentChanged(int arg1)
{
    int count = ui->stackedWidget->count();
    if(count > 1)
        ui->btnBack->setEnabled(true);
    else
        ui->btnBack->setEnabled(false);

    QString title = ui->stackedWidget->currentWidget()->windowTitle();

    if(title == "欢迎") {
        ui->btnLogOut->setEnabled(true);
        ui->btnBack->setEnabled(false);
    } else
        ui->btnLogOut->setEnabled(false);

}


void masterView::on_btnLogOut_clicked()
{
    goPreviousView();
}

