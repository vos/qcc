#include "registerdialog.h"
#include "ui_registerdialog.h"

#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

QString RegisterDialog::username() const
{
    return ui->usernameLineEdit->text();
}

QString RegisterDialog::password() const
{
    return ui->password1LineEdit->text();
}

void RegisterDialog::on_buttonBox_accepted()
{
    QString username = ui->usernameLineEdit->text();
    if (username.isEmpty() || username.length() < 3) {
        QMessageBox::warning(this, "Username error", "The username must be at least 3 characters long.");
        return;
    }

    QString password = ui->password1LineEdit->text();
    if (password.isEmpty() || password.length() < 3) {
        QMessageBox::warning(this, "Password error", "The password must be at least 3 characters long.");
        return;
    }
    if (password != ui->password2LineEdit->text()) {
        QMessageBox::warning(this, "Password error", "The passwords do not match.");
        return;
    }

    accept();
}
