#pragma once

#include <string>
#include "../Dependencies/sqlite3cpp/sqlite3.h"
#include "../Logger/Logger.h"

class DatabaseManager {
public:
    DatabaseManager(const std::string& path);
    ~DatabaseManager();

    bool open();
    bool prepareQuery(const std::string& query);
    bool step();
    int  getIntColumn(int index);

private:
    sqlite3* _db;
    sqlite3_stmt* _stmt;
    std::string _dbPath;

    void close();
    void finalize();
};

