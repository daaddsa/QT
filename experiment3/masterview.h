#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QWidget>
#include "loginview.h"
#include "doctorview.h"
#include "departmentview.h"
#include "patienteditview.h"
#include "patientview.h"
#include "welcomeview.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class masterView;
}
QT_END_NAMESPACE

class masterView : public QWidget
{
    Q_OBJECT

public:
    masterView(QWidget *parent = nullptr);
    ~masterView();

public slots:
    void goLoginView();
    void goWelcomeView();
    void goDoctorView();
    void goDepartmentView();
    void goPatientEditView(int rowNo);
    void goPatientView();
    void goPreviousView();

private slots:
    void on_btnBack_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_btnLogOut_clicked();

private:
    void pushWidgetToStackView(QWidget *widget);

    Ui::masterView *ui;

    WelcomeView *welcomeView;
    DoctorView *doctorView;
    patientview *patientView;
    DepartmentView *departmentView;
    LoginView *loginView;
    patientEditView *m_patientEditView;
};
#endif // MASTERVIEW_H
