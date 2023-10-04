#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "db.h"

#include <QDialog>

#include <memory>
namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog {
    Q_OBJECT

public:
    explicit StartScreen(std::shared_ptr<DB> db = nullptr, QWidget* parent = nullptr);
    ~StartScreen();
    void setLoginForm();
    void setRegistrationForm();
    QString username() const;

    int user_id() const;

    std::shared_ptr<DB> db() const;

public slots:
    void onLoggedIn(int user_id, QString username);
    void onRejectRequest();

private:
    Ui::StartScreen* ui;
    int user_id_;
    QString username_;
    std::shared_ptr<DB> db_;
};

#endif // STARTSCREEN_H
