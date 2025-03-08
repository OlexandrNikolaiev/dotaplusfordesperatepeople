#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const QString& path)
    : m_dbPath(path) {
    static int connectionCount = 0;
    m_connectionName = QString("SQLiteConnection_%1").arg(connectionCount++);
    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    m_db.setDatabaseName(m_dbPath);
}

DatabaseManager::~DatabaseManager() {
    close();
    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase::removeDatabase(m_connectionName);
    }
}

bool DatabaseManager::open() {
    m_db.setConnectOptions("QSQLITE_OPEN_READONLY");
    if (!m_db.open()) {
        qCritical() << "Database open error:" << m_db.lastError().text();
        return false;
    }
    qInfo() << "Opened database in read-only mode";
    return true;
}

bool DatabaseManager::prepareQuery(const QString& query) {
    m_query = QSqlQuery(m_db);
    if (!m_query.prepare(query)) {
        qCritical() << "Query prepare error:" << m_query.lastError().text();
        return false;
    }
    if (!m_query.exec()) {
        qCritical() << "Query exec error:" << m_query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::step() {
    return m_query.next();
}

int DatabaseManager::getIntColumn(int index) {
    return m_query.value(index).toInt();
}

void DatabaseManager::finalize() {
    m_query.finish();
}

void DatabaseManager::close() {
    finalize();
    if (m_db.isOpen()) {
        m_db.close();
        qInfo() << "Database closed";
    }
}
