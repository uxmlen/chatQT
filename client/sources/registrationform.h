#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include "db.h"

#include <QWidget>
#include <memory>

namespace Ui {
class RegistrationForm;
}

class RegistrationForm : public QWidget {
    Q_OBJECT

public:
    explicit RegistrationForm(QWidget* parent = nullptr);
    ~RegistrationForm();
    void setDatabase(std::shared_ptr<DB> db);
signals:
    void loginRequest();
    void accepted(int user_id, QString username);
    void rejected();

private slots:
    void on_loginButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::RegistrationForm* ui;
    std::shared_ptr<DB> db_;
};

#endif // REGISTRATIONFORM_H
