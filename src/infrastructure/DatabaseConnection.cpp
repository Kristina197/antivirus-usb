#include "DatabaseConnection.h"
#include "SqlQueryLoader.h"
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

DatabaseConnection::DatabaseConnection() {
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    
    // ИСПРАВЛЕНИЕ: используем фиксированный путь к БД
    QString dbPath = QDir::homePath() + "/antivirus-usb/build/antivirus.db";
    
    // Создаем директорию если не существует
    QDir dbDir = QFileInfo(dbPath).absoluteDir();
    if (!dbDir.exists()) {
        dbDir.mkpath(".");
    }
    
    db_.setDatabaseName(dbPath);
    
    if (!db_.open()) {
        qCritical() << "Failed to open database:" << db_.lastError().text();
        return;
    }
    
    QSqlQuery query(db_);
    query.exec("PRAGMA foreign_keys = ON");
    query.exec("PRAGMA journal_mode = WAL");
    
    qDebug() << "✓ Database opened successfully at:" << dbPath;
    
    // SQL файлы тоже ищем по абсолютному пути
    QString sqlBasePath = QDir::homePath() + "/antivirus-usb/build/sql/";
    
    auto& queryLoader = SqlQueryLoader::getInstance();
    if (!queryLoader.executeSchemaFile(db_, sqlBasePath + "schema.sql")) {
        qCritical() << "Failed to initialize database schema";
        return;
    }
    
    queryLoader.loadQueriesFromFile(sqlBasePath + "queries/signature_queries.sql");
    queryLoader.loadQueriesFromFile(sqlBasePath + "queries/device_queries.sql");
    
    qDebug() << "✓ Database initialized with schema and queries";
}

QSqlDatabase DatabaseConnection::getDatabase() const {
    return db_;
}
