#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const std::string& path)
    : _db(nullptr), _stmt(nullptr), _dbPath(path)
{}

DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::open() {
    int rc = sqlite3_open_v2(_dbPath.c_str(), &_db, SQLITE_OPEN_READONLY, nullptr);
    if (rc != SQLITE_OK) {
        LOG("Failed to open database: " + std::string(sqlite3_errmsg(_db)));
        return false;
    }
    LOG("Database opened successfully");
    return true;
}

bool DatabaseManager::prepareQuery(const std::string& query) {
    int rc = sqlite3_prepare_v2(_db, query.c_str(), -1, &_stmt, nullptr);
    if (rc != SQLITE_OK) {
        LOG("Failed to prepare query: " + std::string(sqlite3_errmsg(_db)));
        return false;
    }
    return true;
}

bool DatabaseManager::step() {
    return sqlite3_step(_stmt) == SQLITE_ROW;
}

int DatabaseManager::getIntColumn(int index) {
    return sqlite3_column_int(_stmt, index);
}

void DatabaseManager::finalize() {
    if (_stmt) {
        sqlite3_finalize(_stmt);
        _stmt = nullptr;
    }
}

void DatabaseManager::close() {
    finalize();
    if (_db) {
        sqlite3_close(_db);
        _db = nullptr;
    }
}