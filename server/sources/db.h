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
    std::vector<std::string> getAllBannedUsers();

    std::vector<std::string> getAllCommonMessages();
    std::vector<std::string> getAllPrivateMessages(int current_user_id);
    std::vector<std::string> getAllPrivateMessages();

    // check if the user exists in the db
    bool isUserExisted(const std::string& username);
    bool isUserIDInBannedList(int user_id);
    std::string WhyUserInBlockedList(int user_id);
    bool checkPassword(const std::string& username, const std::string& password);
    // numbers of users in the database
    int getNumUserInDB();
    int getNumBannedUsersInDB();
    bool deleteMsg(int msg_id);
    bool updateMessage(int msg_id, const std::string& new_content);
    std::string getUsernameByID(int id);
    int getUserIDByUsername(const std::string& username);
    void addUserInBannedList(const std::string& username, const std::string& reason);
    void unBannedUser(int id);

private:
    MYSQL* conn_;
};
