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
    explicit MainWindow(std::shared_ptr<DB> db = nullptr, QWidget* parent = nullptr);
    ~MainWindow();
private slots:
    // void on_actionOpen_Another_Client_triggered();
    void on_actionExit_triggered();
    void updateChats();

    void on_actionBanned_List_triggered();

    void on_actionUser_List_triggered();

    void on_actionDelete_triggered();

    void on_banButton_clicked();

    void on_unbanButton_clicked();

    void on_writeMessage_clicked();

private:
    Ui::MainWindow* ui;
    std::shared_ptr<DB> db_;
    // int user_id_;
    // QString username_;
};

#endif // MAINWINDOW_H
