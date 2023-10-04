#include "mainwindow.h"
#include "startscreen.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QMessageBox>
#include <QTimer>
#include <QVBoxLayout>
MainWindow::MainWindow(int user_id, QString username, std::shared_ptr<DB> db, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , user_id_(user_id)
    , username_(username)
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
    ui->loggedInAsLineEdit->setText(username_);
    ui->numberUsersRegistredEdit->setText(QString::number(db_->getNumUserInDB()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
MainWindow* MainWindow::createClient(std::shared_ptr<DB> db)
{
    StartScreen s(db);
    auto res = s.exec();
    if (res == QDialog::Rejected) {
        return nullptr;
    }
    auto w = new MainWindow(s.user_id(), s.username(), s.db());
    w->setWindowTitle("Main Menu");

    return w;
}
void MainWindow::on_MessageLineEdit_returnPressed()
{
    on_sendMessageButton_clicked();
}

bool isValidMsg(std::string s) noexcept
{
    return s.length() >= 3
        && s.length() <= 64
        && std::isalpha((unsigned char)s[0]);
}

void MainWindow::on_sendMessageButton_clicked()
{
    if (db_->isUserIDInBannedList(user_id_)) {
        QMessageBox::critical(this, tr("Error!"), tr("Your account ") + username_ + ("was blocked ;(. reason: ") + QString::fromStdString(db_->WhyUserInBlockedList(user_id_)));
        return;
    }
    if (!isValidMsg(ui->MessageLineEdit->text().toStdString())) {
        QMessageBox::critical(this, tr("error"), tr("is not valid message"));
        return;
    }
    int send_to_all = 0;
    db_->sendMessage(username_.toStdString(), ui->MessageLineEdit->text().toStdString(), send_to_all);
    ui->MessageLineEdit->clear();
}

void MainWindow::on_sendPrivateMsgButton_clicked()
{
    if (db_->isUserIDInBannedList(user_id_)) {
        QMessageBox::critical(this, tr("Error!"), tr("Your account ") + username_ + ("was blocked ;(. reason: ") + QString::fromStdString(db_->WhyUserInBlockedList(user_id_)));
        return;
    }
    if (!isValidMsg(ui->MessageLineEdit->text().toStdString())) {
        QMessageBox::critical(this, tr("error"), tr("is not valid message"));
        return;
    }
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

    auto user_list = db_->getAllUsers();
    for (const auto& user : user_list) {

        user_list_w->addItem(QString::fromStdString(user));
    }
    user_list_w->setCurrentRow(0);
    auto res = dial.exec();
    if (res == QDialog::Accepted && user_list_w->currentItem()) {
        int choosen_user_id = db_->getUserIDByUsername(user_list_w->currentItem()->text().toStdString());
        db_->sendMessage(username_.toStdString(), ui->MessageLineEdit->text().toStdString(), choosen_user_id);
    }
    ui->MessageLineEdit->clear();
}

void MainWindow::on_actionOpen_Another_Client_triggered()
{
    auto w = createClient(db_);
    if (w)
        w->show();
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
    auto private_msgs = db_->getAllPrivateMessages(user_id_);
    for (const auto& msg : private_msgs) {
        chat_msgs.append(QString::fromStdString(msg));
    }
    ui->privateChatBrowser->setText(chat_msgs);
}
