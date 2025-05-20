#include <sqlite3.h>
#include <iostream>
#include <string>

class Database {
private:
    sqlite3* db;

public:
    Database() : db(nullptr) {}

    ~Database() {
        close();
    }

    bool open(const std::string& filename) {
        return sqlite3_open(filename.c_str(), &db) == SQLITE_OK;
    }

    void close() {
        if (db) {
            sqlite3_close(db);
            db = nullptr;
        }
    }

    bool execute(const std::string& sql) {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (errMsg) sqlite3_free(errMsg);
        return rc == SQLITE_OK;
    }

    // Example: Add a user
    bool addUser(const std::string& username, const std::string& password) {
        std::string sql = "INSERT INTO users (username, password) VALUES ('" +
                          username + "', '" + password + "');";
        return execute(sql);
    }

    // Example: Check if user exists
    bool userExists(const std::string& username) {
        std::string sql = "SELECT COUNT(*) FROM users WHERE username='" + username + "';";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) return false;
        rc = sqlite3_step(stmt);
        bool exists = false;
        if (rc == SQLITE_ROW) {
            exists = sqlite3_column_int(stmt, 0) > 0;
        }
        sqlite3_finalize(stmt);
        return exists;
    }
};