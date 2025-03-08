#pragma once

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "../Logger/Logger.h"

class DatabaseManager {
public:
    DatabaseManager(const QString& path);
    ~DatabaseManager();

    bool open();
    bool prepareQuery(const QString& query);
    bool step();
    int getIntColumn(int index);
    void close();

private:
    void finalize();

    QSqlDatabase m_db;
    QSqlQuery m_query;
    QString m_dbPath;
    QString m_connectionName;
};
