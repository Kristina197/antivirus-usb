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
        qCritical() << "Не удалось открыть файл запросов:" << filePath;
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

    qDebug() << "Загружено" << queries_.size() << "запросов из" << filePath;
    return true;
}

QString SqlQueryLoader::getQuery(const QString& queryName) const {
    if (!queries_.contains(queryName)) {
        qWarning() << "Запрос не найден:" << queryName;
        return QString();
    }
    return queries_[queryName];
}

bool SqlQueryLoader::executeSchemaFile(QSqlDatabase& db, const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Не удалось открыть файл схемы:" << filePath;
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

        if (trimmed.toUpper() == "BEGIN" || 
            trimmed.toUpper() == "COMMIT" || 
            trimmed.toUpper() == "END") {
            continue;
        }

        QSqlQuery query(db);
        if (!query.exec(trimmed)) {
            qWarning() << "Не удалось выполнить выражение:" << query.lastError().text();
            qWarning() << "Выражение:" << trimmed.left(100);
        }
    }

    qDebug() << "Схема базы данных успешно выполнена";
    return true;
}
