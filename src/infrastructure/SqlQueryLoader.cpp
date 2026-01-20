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

QString SqlQueryLoader::loadSingleQuery(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open query file:" << filePath;
        return QString();
    }
    
    QTextStream in(&file);
    QString query;
    
    while (!in.atEnd()) {
        QString line = in.readLine();
        QString trimmed = line.trimmed();
        
        if (trimmed.startsWith("--")) {
            continue;
        }
        
        if (!trimmed.isEmpty()) {
            query += trimmed + " ";
        }
    }
    
    file.close();
    
    QString result = query.trimmed();
    qDebug() << "Loaded query from" << filePath << "(" << result.length() << "bytes)";
    
    return result;
}

bool SqlQueryLoader::loadQueriesFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open queries file:" << filePath;
        return false;
    }
    
    QTextStream in(&file);
    QString currentQuery;
    QString currentName;
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        
        if (line.startsWith("--") && line.contains(":")) {
            if (!currentName.isEmpty() && !currentQuery.isEmpty()) {
                queries_[currentName] = currentQuery.trimmed();
            }
            currentName = line.mid(2).split(":").first().trimmed();
            currentQuery.clear();
        } else if (!line.startsWith("--") && !line.isEmpty()) {
            currentQuery += line + " ";
        }
    }
    
    if (!currentName.isEmpty() && !currentQuery.isEmpty()) {
        queries_[currentName] = currentQuery.trimmed();
    }
    
    file.close();
    qDebug() << "Loaded" << queries_.size() << "queries from" << filePath;
    return true;
}

QString SqlQueryLoader::getQuery(const QString& queryName) const {
    return queries_.value(queryName);
}

bool SqlQueryLoader::executeSchemaFile(QSqlDatabase& db, const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open schema file:" << filePath;
        return false;
    }
    
    QTextStream in(&file);
    QString schema = in.readAll();
    file.close();
    
    QStringList statements = schema.split(';', Qt::SkipEmptyParts);
    
    for (const QString& statement : statements) {
        QString trimmed = statement.trimmed();
        if (trimmed.isEmpty()) continue;
        
        QSqlQuery query(db);
        if (!query.exec(trimmed)) {
            qWarning() << "Failed to execute statement:" << query.lastError().text();
            return false;
        }
    }
    
    qDebug() << "Schema executed successfully";
    return true;
}
