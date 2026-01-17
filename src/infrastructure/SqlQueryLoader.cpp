#include "SqlQueryLoader.h"
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SqlQueryLoader& SqlQueryLoader::getInstance() {
    static SqlQueryLoader instance;
    return instance;
}

bool SqlQueryLoader::loadQueriesFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Failed to open query file:" << filePath;
        return false;
    }
    
    QTextStream in(&file);
    QString currentName;
    QString currentQuery;
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        
        if (line.startsWith("-- name:")) {
            if (!currentName.isEmpty() && !currentQuery.isEmpty()) {
                queries_[currentName] = currentQuery.trimmed();
            }
            currentName = line.mid(8).trimmed();
            currentQuery.clear();
        } else if (!line.startsWith("--") && !line.isEmpty()) {
            currentQuery += line + " ";
        }
    }
    
    if (!currentName.isEmpty() && !currentQuery.isEmpty()) {
        queries_[currentName] = currentQuery.trimmed();
    }
    
    qDebug() << "Loaded" << queries_.size() << "queries from" << filePath;
    return true;
}

QString SqlQueryLoader::getQuery(const QString& queryName) const {
    if (!queries_.contains(queryName)) {
        qWarning() << "Query not found:" << queryName;
        return QString();
    }
    return queries_[queryName];
}

bool SqlQueryLoader::executeSchemaFile(QSqlDatabase& db, const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Failed to open schema file:" << filePath;
        return false;
    }
    
    QTextStream in(&file);
    QString sql = in.readAll();
    
    QStringList statements = sql.split(';', Qt::SkipEmptyParts);
    
    for (const QString& statement : statements) {
        QString trimmed = statement.trimmed();
        if (trimmed.isEmpty() || trimmed.startsWith("--")) {
            continue;
        }
        
        QSqlQuery query(db);
        if (!query.exec(trimmed)) {
            qCritical() << "Failed to execute statement:" << query.lastError().text();
            qCritical() << "Statement:" << trimmed;
            return false;
        }
    }
    
    qDebug() << "Schema executed successfully";
    return true;
}
