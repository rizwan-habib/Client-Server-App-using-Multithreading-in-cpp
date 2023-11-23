#include <iostream>
#include <sqlite3.h>
#include <vector>
using namespace std;
// Database Class
class Database
{
private:
    // sqlite object to access Database
    sqlite3 *db;

public:
    // constructor
    Database()
    {
    }
    // table creation
    int create()
    {
        // open database
        int flag = sqlite3_open("chat.db", &db);
        if (flag != SQLITE_OK)
        {
            cout << "Error opening db: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
            return -1;
        }
        // create table for messages
        std::string sql = "create table if not exists messages (id integer primary key autoincrement,client_msg text, timestamp text)";
        char *err;
        flag = sqlite3_exec(db, sql.c_str(), 0, 0, &err);
        if (flag != SQLITE_OK)
        {
            cout << "Error creating schema: " << err << endl;
            sqlite3_free(err);
            sqlite3_close(db);
            return -1;
        }
        return 0;
    }

    // data insertion
    void insert(string rcvMsg, string timestamp)
    {
        string sql = "insert into messages ( client_msg, timestamp) values ('" + rcvMsg + "', '" + timestamp + "');";
        char *err;
        int flag = sqlite3_exec(db, sql.c_str(), 0, 0, &err);
        if (flag != SQLITE_OK)
        {
            cout << "Error in inserting" << err << endl;
            sqlite3_free(err);
        }
    }
    // closing db
    void closeConnection()
    {
        // Close the database connection
        sqlite3_close(db);
    }

    std::vector<std::string> fetchMessages()
    {
        std::vector<std::string> messages;
        sqlite3_stmt *statement;
        std::string sql = "SELECT client_msg FROM messages";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK)
        {
            while (sqlite3_step(statement) == SQLITE_ROW)
            {
                const unsigned char *messageText = sqlite3_column_text(statement, 0);
                std::string message(reinterpret_cast<const char *>(messageText));
                messages.push_back(message);
            }
            sqlite3_finalize(statement);
        }

        return messages;
    }
};