#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "db.h"
#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(int user_id, QString username, std::shared_ptr<DB> db = nullptr, QWidget* parent = nullptr);
    ~MainWindow();
    static MainWindow* createClient(std::shared_ptr<DB> db = nullptr);
private slots:
    void on_MessageLineEdit_returnPressed();
    void on_sendMessageButton_clicked();
    void on_sendPrivateMsgButton_clicked();
    void on_actionOpen_Another_Client_triggered();
    void on_actionExit_triggered();
    void updateChats();

private:
    Ui::MainWindow* ui;
    std::shared_ptr<DB> db_;
    int user_id_;
    QString username_;
};

#endif // MAINWINDOW_H
