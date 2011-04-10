#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
    class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = 0);
    ~RegisterDialog();

    QString username() const;
    QString password() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
