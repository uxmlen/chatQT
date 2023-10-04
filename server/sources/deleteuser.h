#ifndef DELETEUSER_H
#define DELETEUSER_H
#include "db.h"
#include <QDialog>

namespace Ui {
class deleteuser;
}

class deleteuser : public QDialog {
    Q_OBJECT

public:
    explicit deleteuser(std::shared_ptr<DB> db, QWidget* parent = nullptr);
    ~deleteuser();

private slots:
    void on_deleteMsgButton_clicked();

private:
    Ui::deleteuser* ui;
    std::shared_ptr<DB> db_;
};

#endif // DELETEUSER_H
