#include "deleteuser.h"
#include "ui_deleteuser.h"
#include <QMessageBox>
deleteuser::deleteuser(std::shared_ptr<DB> db, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::deleteuser)
    , db_(db)
{
    ui->setupUi(this);
}

deleteuser::~deleteuser()
{
    delete ui;
}

void deleteuser::on_deleteMsgButton_clicked()
{
    bool res = !db_->deleteMsg(atoi(ui->msgLineEdit->text().toStdString().c_str()));
    if (res) {
        QMessageBox::critical(this, tr("Error!"), tr("messages is not existeed"));
        ui->msgLineEdit->clear();
        return;
    }
    ui->msgLineEdit->clear();

    QMessageBox::information(this, tr("Success!"), tr("message was deleted"));
}
