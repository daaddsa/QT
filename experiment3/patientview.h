#ifndef PATIENTVIEW_H
#define PATIENTVIEW_H

#include <QWidget>

namespace Ui {
class patientview;
}

class patientview : public QWidget
{
    Q_OBJECT

public:
    explicit patientview(QWidget *parent = nullptr);
    ~patientview();

private slots:
    void on_btnAdd_clicked();

    void on_btnSearch_clicked();

    void on_btnEdit_clicked();

    void on_btnDelete_clicked();

signals:
    void goPatientEditView(int idx);

private:
    Ui::patientview *ui;
};

#endif // PATIENTVIEW_H
