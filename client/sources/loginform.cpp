#include "loginform.h"
#include "ui_loginform.h"

#include <QMessageBox>

LoginForm::LoginForm(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_registrationButton_clicked()
{
    emit registrationRequest();
}

void LoginForm::on_buttonBox_accepted()
{

    if (!db_->checkPassword(ui->loginEdit->text().toStdString(), ui->passwordEdit->text().toStdString())) {
        QMessageBox::critical(this, tr("error"), tr("username or password is not corrected"));
        return;
    }
    int user_id = db_->getUserIDByUsername(ui->loginEdit->text().toStdString());
    if (db_->isUserIDInBannedList(user_id)) {
        QMessageBox::critical(this, tr("Error!"), tr("Your account ") + ui->loginEdit->text() + ("was blocked ;(. reason: ") + QString::fromStdString(db_->WhyUserInBlockedList(user_id)));
        return;
    }

    emit accepted(user_id, ui->loginEdit->text());
}

void LoginForm::on_buttonBox_rejected()
{
    emit rejected();
}

void LoginForm::setDatabase(std::shared_ptr<DB> db)
{
    db_ = db;
}
