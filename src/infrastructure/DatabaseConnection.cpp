#include "DatabaseConnection.h"
#include "SqlQueryLoader.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>
#include <QDir>

DatabaseConnection::DatabaseConnection() {
    if (!initializeConnection()) {
        qCritical() << "Failed to initialize database connection";
    }
}

DatabaseConnection::~DatabaseConnection() {
    if (db_.isOpen()) {
        db_.close();
    }
}

bool DatabaseConnection::initializeConnection() {
    if (!QSqlDatabase::isDriverAvailable(DB_DRIVER)) {
        qCritical() << "PostgreSQL driver not available!";
        qCritical() << "Available drivers:" << QSqlDatabase::drivers();
        return false;
    }
    
    db_ = QSqlDatabase::addDatabase(DB_DRIVER);
    db_.setHostName(DB_HOST);
    db_.setDatabaseName(DB_NAME);
    db_.setUserName(DB_USER);
    db_.setPassword(DB_PASSWORD);
    db_.setPort(DB_PORT);
    
    if (!db_.open()) {
        qCritical() << "Failed to connect to PostgreSQL:";
        qCritical() << db_.lastError().text();
        qCritical() << "Host:" << DB_HOST;
        qCritical() << "Database:" << DB_NAME;
        qCritical() << "User:" << DB_USER;
        return false;
    }
    
    qDebug() << "Connected to PostgreSQL database:" << DB_NAME;
    
    QSqlQuery query(db_);
    query.exec("SELECT COUNT(*) FROM information_schema.tables WHERE table_name = 'virus_signatures'");
    
    if (query.next() && query.value(0).toInt() == 0) {
        qDebug() << "Tables not found, creating schema...";
        if (!createSchema()) {
            qCritical() << "Failed to create schema";
            return false;
        }
    }
    
    return true;
}

bool DatabaseConnection::createSchema() {
    SqlQueryLoader& loader = SqlQueryLoader::getInstance();
    
    QString schemaPath = "sql/schema_postgresql.sql";
    
    if (!QFile::exists(schemaPath)) {
        qCritical() << "Schema file not found:" << schemaPath;
        return false;
    }
    
    if (!loader.executeSchemaFile(db_, schemaPath)) {
        qCritical() << "Failed to execute schema";
        return false;
    }
    
    qDebug() << "Database schema created successfully";
    return true;
}

QSqlDatabase& DatabaseConnection::getDatabase() {
    return db_;
}

bool DatabaseConnection::isConnected() const {
    return db_.isOpen();
}
