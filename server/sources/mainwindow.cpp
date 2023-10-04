#include "mainwindow.h"
#include "deleteuser.h"
#include "ui_mainwindow.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>

#include <QTimer>
#include <QVBoxLayout>
MainWindow::MainWindow(std::shared_ptr<DB> db, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (db) {
        db_ = db;
    } else {
        db_ = std::make_shared<DB>();
    }
    auto timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::updateChats);
    timer->start(10);
    ui->numBannedUsersEdit->setText(QString::number(db_->getNumBannedUsersInDB()));
    ui->numberUsersRegistredEdit->setText(QString::number(db_->getNumUserInDB()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool isValidMsg(std::string s) noexcept
{
    return s.length() >= 3
        && s.length() <= 64
        && std::isalpha((unsigned char)s[0]);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::updateChats()
{
    auto all_msgs = db_->getAllCommonMessages();
    QString chat_msgs;

    for (const auto& msg : all_msgs) {
        chat_msgs.append(QString::fromStdString(msg));
    }
    ui->commonChatBrowser->setText(chat_msgs);
    chat_msgs.clear();
    auto private_msgs = db_->getAllPrivateMessages();
    for (const auto& msg : private_msgs) {
        chat_msgs.append(QString::fromStdString(msg));
    }
    ui->privateChatBrowser->setText(chat_msgs);
}

void MainWindow::on_actionBanned_List_triggered()
{

    QDialog dial(this);
    dial.setModal(true);
    QVBoxLayout* l = new QVBoxLayout();
    dial.setLayout(l);
    QListWidget* user_list_w = new QListWidget(&dial);
    l->addWidget(user_list_w);
    QDialogButtonBox* button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dial);
    l->addWidget(button_box);

    connect(button_box, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, &dial, &QDialog::rejected);

    auto user_list = db_->getAllBannedUsers();
    for (const auto& user : user_list) {

        user_list_w->addItem(QString::fromStdString(user));
    }
    user_list_w->setCurrentRow(0);
    auto res = dial.exec();
    if (res == QDialog::Accepted && user_list_w->currentItem()) {
        int choosen_user_id = db_->getUserIDByUsername(user_list_w->currentItem()->text().toStdString());
        std::string s = db_->WhyUserInBlockedList(choosen_user_id);
        QMessageBox::information(this, tr("Reason of the ban: "), QString::fromStdString(s));
    }

    ui->numBannedUsersEdit->setText(QString::number(db_->getNumBannedUsersInDB()));
    ui->numberUsersRegistredEdit->setText(QString::number(db_->getNumUserInDB()));
}

void MainWindow::on_actionUser_List_triggered()
{
    QDialog dial(this);
    dial.setModal(true);
    QVBoxLayout* l = new QVBoxLayout();
    dial.setLayout(l);
    QListWidget* user_list_w = new QListWidget(&dial);
    l->addWidget(user_list_w);
    QDialogButtonBox* button_box = new QDialogButtonBox(QDialogButtonBox::Ok, &dial);
    l->addWidget(button_box);
    connect(button_box, &QDialogButtonBox::accepted, &dial, &QDialog::accept);

    auto user_list = db_->getAllUsers();
    for (const auto& user : user_list) {

        user_list_w->addItem(QString::fromStdString(user));
    }
    user_list_w->setCurrentRow(0);
    dial.exec();
}

void MainWindow::on_actionDelete_triggered()
{
    deleteuser* f1 = new deleteuser(db_);
    f1->show();
}

void MainWindow::on_banButton_clicked()
{
    if (!isValidMsg(ui->banEdit->text().toStdString())) {
        QMessageBox::critical(this, tr("error"), tr("is not valid message"));
        return;
    }

    QDialog dial(this);
    dial.setModal(true);
    QVBoxLayout* l = new QVBoxLayout();
    dial.setLayout(l);
    QListWidget* user_list_w = new QListWidget(&dial);
    l->addWidget(user_list_w);
    QDialogButtonBox* button_box = new QDialogButtonBox(QDialogButtonBox::Ok, &dial);
    l->addWidget(button_box);
    connect(button_box, &QDialogButtonBox::accepted, &dial, &QDialog::accept);

    auto user_list = db_->getAllUsers();
    for (const auto& user : user_list) {

        user_list_w->addItem(QString::fromStdString(user));
    }
    user_list_w->setCurrentRow(0);
    auto res = dial.exec();

    if (res == QDialog::Accepted && user_list_w->currentItem()) {
        QMessageBox::information(this, tr("User was banned"), "Reason of the ban:" + ui->banEdit->text());
        db_->addUserInBannedList(user_list_w->currentItem()->text().toStdString(), ui->banEdit->text().toStdString());
    }
}

void MainWindow::on_unbanButton_clicked()
{
    if (!isValidMsg(ui->unbanEdit->text().toStdString())) {
        QMessageBox::critical(this, tr("error"), tr("is not valid message"));
        return;
    }
    if (!db_->isUserExisted(ui->unbanEdit->text().toStdString())) {
        QMessageBox::critical(this, tr("Error!"), tr("this user does not exist"));
        return;
    }

    int user_id = db_->getUserIDByUsername(ui->unbanEdit->text().toStdString());
    if (!db_->isUserIDInBannedList(user_id)) {
        QMessageBox::critical(this, tr("Error!"), tr("this user is not in a banned list"));
        return;
    }
    QMessageBox::information(this, tr("success!!"), ui->unbanEdit->text() + " was unbanned");
    db_->unBannedUser(user_id);
    ui->numBannedUsersEdit->setText(QString::number(db_->getNumBannedUsersInDB()));
    ui->numberUsersRegistredEdit->setText(QString::number(db_->getNumUserInDB()));
}

void MainWindow::on_writeMessage_clicked()
{
    if (!isValidMsg(ui->usernameMsgEdit->text().toStdString())) {
        QMessageBox::critical(this, tr("error"), tr("is not valid message"));
        return;
    }
    QDialog dial(this);
    dial.setModal(true);
    QVBoxLayout* l = new QVBoxLayout();
    dial.setLayout(l);
    QListWidget* user_list_w = new QListWidget(&dial);
    l->addWidget(user_list_w);
    QDialogButtonBox* button_box = new QDialogButtonBox(QDialogButtonBox::Ok, &dial);
    l->addWidget(button_box);
    connect(button_box, &QDialogButtonBox::accepted, &dial, &QDialog::accept);

    auto user_list = db_->getAllUsers();
    user_list_w->addItem("all");
    for (const auto& user : user_list) {

        user_list_w->addItem(QString::fromStdString(user));
    }
    user_list_w->setCurrentRow(0);
    auto res = dial.exec();

    if (res == QDialog::Accepted && user_list_w->currentItem()) {
        db_->sendMessage(user_list_w->currentItem()->text().toStdString(), ui->usernameMsgEdit->text().toStdString(), 0);
    }
}
