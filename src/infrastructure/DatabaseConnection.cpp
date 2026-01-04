#include "DatabaseConnection.h"
#include "SqlQueryLoader.h"
#include <QSqlQuery>
#include <QSqlError>
#include <iostream>
DatabaseConnection& DatabaseConnection::getInstance() { static DatabaseConnection instance; return instance; }
bool DatabaseConnection::initialize(const std::string& dbPath) {
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(QString::fromStdString(dbPath));
    if (!db_.open()) { std::cerr << "Failed to open database\n"; return false; }
    auto& loader = SqlQueryLoader::getInstance();
    QSqlQuery query(db_);
    if (!query.exec(QString::fromStdString(loader.getQuery("create_signatures_table")))) return false;
    if (!query.exec(QString::fromStdString(loader.getQuery("create_scan_history_table")))) return false;
    if (!query.exec(QString::fromStdString(loader.getQuery("create_devices_table")))) return false;
    connected_ = true; return true;
}
QSqlDatabase& DatabaseConnection::getDatabase() { return db_; }
bool DatabaseConnection::isConnected() const { return connected_; }
