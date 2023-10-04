#include "registrationform.h"
#include "ui_registrationform.h"

#include <QMessageBox>
#include <algorithm>
#include <cctype>
RegistrationForm::RegistrationForm(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationForm)
{
    ui->setupUi(this);
}

RegistrationForm::~RegistrationForm()
{
    delete ui;
}

void RegistrationForm::on_loginButton_clicked()
{
    emit loginRequest();
}

bool isValidUserName(std::string s) noexcept
{
    return s.length() >= 3
        && s.length() <= 64
        && std::isalpha((unsigned char)s[0]);
}

void RegistrationForm::on_buttonBox_accepted()
{
    if (!isValidUserName(ui->loginEdit->text().toStdString())) {
        QMessageBox::critical(this, tr("error"), tr("username is incorrect"));
        return;
    }

    if (ui->passwordEdit->text() != ui->passwordConfirmEdit->text()) {
        QMessageBox::critical(this, tr("error"), tr("Password not match with confirm password"));
        return;
    }
    if (db_->isUserExisted(ui->loginEdit->text().toStdString())) {
        QMessageBox::critical(this, tr("error"), tr("username is already busy"));
        return;
    }
    db_->createdUser(ui->loginEdit->text().toStdString(), ui->passwordEdit->text().toStdString());
    int user_id = db_->getUserIDByUsername(ui->passwordEdit->text().toStdString());
    emit accepted(user_id, ui->loginEdit->text());
}

void RegistrationForm::on_buttonBox_rejected()
{
    emit rejected();
}

void RegistrationForm::setDatabase(std::shared_ptr<DB> db)
{
    db_ = db;
}
