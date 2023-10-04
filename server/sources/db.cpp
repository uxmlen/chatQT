#include "db.h"

DB::DB()
{
    conn_ = mysql_init(nullptr);
    conn_ = mysql_real_connect(conn_, "127.0.0.1", "root", "", "chat_cpp", 0, NULL, 0);
    if (conn_ == nullptr) {
        // TODO: exception
        std::cerr << "ERROR: " << mysql_error(conn_) << std::endl;
        mysql_close(conn_);
        exit(1);
    }
    mysql_set_character_set(conn_, "utf8");
}

DB::~DB()
{
    if (conn_) {
        mysql_close(conn_);
    }
}

void DB::createdUser(const std::string& username, const std::string& password)
{
    std::stringstream query;
    query << "INSERT INTO users (name, password) VALUES ('" << username << "', '" << password
          << "');";
    mysql_query(conn_, query.str().c_str());
}

int DB::getUserIDByUsername(const std::string& username)
{
    MYSQL_RES* res;
    std::stringstream query;
    // if receiver_id == 0 then message to all users
    query << "SELECT user_id, name FROM users WHERE name = '" << username << "';";
    mysql_query(conn_, query.str().c_str());
    res = mysql_store_result(conn_);
    if (res) {
        int data = atoi(mysql_fetch_row(res)[0]);
        mysql_free_result(res);
        return data;
    }
    return -1;
}

std::string DB::getUsernameByID(int id)
{
    if (id == 0) {
        return "all";
    }
    MYSQL_RES* res;
    std::stringstream query;
    // if receiver_id == 0 then message to all users
    query << "SELECT user_id, name FROM users WHERE user_id = " << id << ";";
    mysql_query(conn_, query.str().c_str());
    res = mysql_store_result(conn_);
    std::string data = mysql_fetch_row(res)[1];
    mysql_free_result(res);
    return data;
}

void DB::sendMessage(const std::string& username, const std::string& content, int receiver)
{
    int current_user_id = getUserIDByUsername(username);

    std::stringstream query;
    // if receiver_id == 0 then message to all users
    query << "INSERT INTO messages (receiver_id, author_id, content) VALUES ( " << receiver << "," << current_user_id
          << ", '" << content << "');";
    mysql_query(conn_, query.str().c_str());
}

std::vector<std::string> DB::getAllCommonMessages()
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    std::vector<std::string> strings;
    mysql_query(conn_, "SELECT message_id, author_id, content FROM messages WHERE receiver_id = 0;");

    res = mysql_store_result(conn_);
    while ((row = mysql_fetch_row(res))) {
        std::stringstream s;
        std::string id = getUsernameByID(atoi(row[1]));

        s << "msg_id: " << row[0] << " < " << id << ">: " << row[2] << std::endl;
        strings.push_back(s.str().c_str());
    }
    mysql_free_result(res);
    return strings;
}

std::vector<std::string> DB::getAllUsers()
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    std::vector<std::string> user_list;

    mysql_query(conn_, "SELECT name, created_at FROM users");
    res = mysql_store_result(conn_);

    while ((row = mysql_fetch_row(res))) {
        user_list.push_back(row[0]);
    }
    return user_list;
}
std::vector<std::string> DB::getAllBannedUsers()
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    std::vector<std::string> user_list;

    mysql_query(conn_, "SELECT user_id FROM banned_users;");
    res = mysql_store_result(conn_);

    while ((row = mysql_fetch_row(res))) {
        std::string username = getUsernameByID(atoi(row[0]));

        user_list.push_back(username);
    }
    return user_list;
}

bool DB::isUserExisted(const std::string& username)
{
    MYSQL_RES* res;

    std::stringstream query;
    query << "SELECT EXISTS(SELECT 1 FROM users WHERE name = '" << username << "');";
    std::cout << query.str();
    mysql_query(conn_, query.str().c_str());
    res = mysql_store_result(conn_);
    return atoi(mysql_fetch_row(res)[0]);
}

bool DB::checkPassword(const std::string& username, const std::string& password)
{
    MYSQL_RES* res;
    std::stringstream query;

    query << "SELECT * FROM users WHERE name = "
          << "'" << username << "' and password = '" << password << "';";
    mysql_query(conn_, query.str().c_str());
    res = mysql_store_result(conn_);
    if (res) {
        return mysql_num_rows(res) == 1;
    }
    return 0;
}

int DB::getNumUserInDB()
{
    MYSQL_RES* res;
    mysql_query(conn_, "SELECT * FROM users;");

    res = mysql_store_result(conn_);
    if (res) {
        return mysql_num_rows(res);
    }
    return 0;
}
int DB::getNumBannedUsersInDB()
{
    MYSQL_RES* res;
    mysql_query(conn_, "SELECT * FROM banned_users;");

    res = mysql_store_result(conn_);
    if (res) {
        return mysql_num_rows(res);
    }
    return 0;
}

std::vector<std::string> DB::getAllPrivateMessages(int current_user_id)
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    std::vector<std::string> strings;
    std::stringstream query;

    query << "SELECT author_id, receiver_id, content FROM messages WHERE receiver_id = " << current_user_id << " or author_id =" << current_user_id << ";";

    mysql_query(conn_, query.str().c_str());
    res = mysql_store_result(conn_);
    if (res) {
        while ((row = mysql_fetch_row(res))) {
            std::stringstream s;
            std::string author_name = getUsernameByID(atoi(row[0]));
            std::string receiver_name = getUsernameByID(atoi(row[1]));

            s << tr("Author: ").toStdString() << author_name << tr("to ").toStdString() << receiver_name << ": '" << row[2] << '\n';
            strings.push_back(s.str().c_str());
            std::cout << std::endl;
        }
    } else {
        strings.push_back("there are no private messages ;(");
    }
    return strings;
}
std::vector<std::string> DB::getAllPrivateMessages()
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    std::vector<std::string> strings;
    std::stringstream query;

    query << "SELECT author_id, receiver_id, content FROM messages;";

    mysql_query(conn_, query.str().c_str());
    res = mysql_store_result(conn_);
    if (res) {
        while ((row = mysql_fetch_row(res))) {
            std::stringstream s;
            std::string author_name = getUsernameByID(atoi(row[0]));
            std::string receiver_name = getUsernameByID(atoi(row[1]));

            s << tr("Author: ").toStdString() << author_name << tr(" to ").toStdString() << receiver_name << ": '" << row[2] << '\n';
            strings.push_back(s.str().c_str());
            std::cout << std::endl;
        }
    } else {
        strings.push_back("there are no private messages ;(");
    }
    return strings;
}

bool DB::isUserIDInBannedList(int user_id)
{
    MYSQL_RES* res;

    std::stringstream query;
    query << "SELECT EXISTS(SELECT 1 FROM banned_users WHERE user_id = " << user_id << ");";
    mysql_query(conn_, query.str().c_str());
    std::cout << query.str() << std::endl;
    res = mysql_store_result(conn_);
    return atoi(mysql_fetch_row(res)[0]);
}

std::string DB::WhyUserInBlockedList(int user_id)
{
    MYSQL_RES* res;
    std::stringstream query;
    // if receiver_id == 0 then message to all users
    query << "SELECT user_id, reason FROM banned_users WHERE user_id = " << user_id << ";";
    mysql_query(conn_, query.str().c_str());
    res = mysql_store_result(conn_);
    std::string data = mysql_fetch_row(res)[1];
    mysql_free_result(res);
    return data;
}

bool DB::deleteMsg(int msg_id)
{
    std::stringstream query;
    // if receiver_id == 0 then message to all users
    query << "DELETE FROM messages WHERE message_id = " << msg_id << ";";
    if (mysql_query(conn_, query.str().c_str())) {
        return true;
    }
    return false;
}
void DB::addUserInBannedList(const std::string& username, const std::string& reason)
{
    std::stringstream query;
    int user_id = getUserIDByUsername(username);
    query << "INSERT INTO banned_users (user_id, reason) VALUES ('" << user_id << "', '" << reason
          << "');";
    mysql_query(conn_, query.str().c_str());
}

void DB::unBannedUser(int user_id)
{
    std::stringstream query;
    query << "DELETE FROM banned_users WHERE user_id = " << user_id << ";";
    mysql_query(conn_, query.str().c_str());
}

bool DB::updateMessage(int msg_id, const std::string& new_content)
{
    std::stringstream query;
    query << "UPDATE messages SET content = '" << new_content << "' WHERE message_id = " << msg_id;
    if (mysql_query(conn_, query.str().c_str())) {
        return true;
    }
    return false;
}
