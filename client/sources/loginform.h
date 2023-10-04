#ifndef LOGINFORM_H
#define LOGINFORM_H

#include "db.h"
#include "memory"
#include <QWidget>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget {
    Q_OBJECT

public:
    explicit LoginForm(QWidget* parent = nullptr);
    ~LoginForm();
    void setDatabase(std::shared_ptr<DB> db);

signals:
    void registrationRequest();
    void accepted(int user_id, QString username);
    void rejected();
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_registrationButton_clicked();

private:
    Ui::LoginForm* ui;
    std::shared_ptr<DB> db_;
};

#endif // LOGINFORM_H
