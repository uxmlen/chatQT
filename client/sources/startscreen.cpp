#include "startscreen.h"
#include "ui_startscreen.h"

StartScreen::StartScreen(std::shared_ptr<DB> db, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::StartScreen)
{
    ui->setupUi(this);
    if (db) {
        db_ = db;
    } else {
        db_ = std::make_shared<DB>();
    }
    ui->loginPage->setDatabase(db_);
    ui->registrationPage->setDatabase(db_);

    connect(ui->loginPage, &LoginForm::registrationRequest, this, &StartScreen::setRegistrationForm);
    connect(ui->loginPage, &LoginForm::accepted, this, &StartScreen::onLoggedIn);
    connect(ui->loginPage, &LoginForm::rejected, this, &StartScreen::onRejectRequest);

    connect(ui->registrationPage, &RegistrationForm::loginRequest, this, &StartScreen::setLoginForm);
    connect(ui->registrationPage, &RegistrationForm::accepted, this, &StartScreen::onLoggedIn);
    connect(ui->registrationPage, &RegistrationForm::rejected, this, &StartScreen::onRejectRequest);
}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::setLoginForm()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StartScreen::setRegistrationForm()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void StartScreen::onLoggedIn(int user_id, QString username)
{
    user_id_ = user_id;
    username_ = username;
    accept();
}

void StartScreen::onRejectRequest()
{
    reject();
}

std::shared_ptr<DB> StartScreen::db() const
{
    return db_;
}

int StartScreen::user_id() const
{
    return user_id_;
}

QString StartScreen::username() const
{
    return username_;
}
