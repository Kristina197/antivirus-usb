#pragma once
#include <QString>
#include <QMap>
#include <QSqlDatabase>

class SqlQueryLoader {
public:
    static SqlQueryLoader& getInstance();
    
    bool loadQueriesFromFile(const QString& filePath);
    QString getQuery(const QString& queryName) const;
    bool executeSchemaFile(QSqlDatabase& db, const QString& filePath);
    
    // Новый метод: загрузить один запрос из файла
    QString loadSingleQuery(const QString& filePath);
    
private:
    SqlQueryLoader() = default;
    QMap<QString, QString> queries_;
    
    SqlQueryLoader(const SqlQueryLoader&) = delete;
    SqlQueryLoader& operator=(const SqlQueryLoader&) = delete;
};
