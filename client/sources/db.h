#pragma once

#include <QCoreApplication>
#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

#include <vector>

class DB {
    Q_DECLARE_TR_FUNCTIONS(DB)
public:
    DB();
    ~DB();

    void createdUser(const std::string& username, const std::string& password);
    void sendMessage(const std::string& username, const std::string& content, int receiver_id);
    // getter
    std::vector<std::string> getAllUsers();

    std::vector<std::string> getAllCommonMessages();
    std::vector<std::string> getAllPrivateMessages(int current_user_id);

    // check if the user exists in the db
    bool isUserExisted(const std::string& username);
    bool isUserIDInBannedList(int user_id);
    std::string WhyUserInBlockedList(int user_id);
    bool checkPassword(const std::string& username, const std::string& password);
    // numbers of users in the database
    int getNumUserInDB();
    std::string getUsernameByID(int id);
    int getUserIDByUsername(const std::string& username);

private:
    MYSQL* conn_;
};
