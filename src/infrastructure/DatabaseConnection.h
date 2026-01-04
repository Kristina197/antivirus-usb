#pragma once
#include <QSqlDatabase>
#include <string>
class DatabaseConnection {
public:
    static DatabaseConnection& getInstance();
    bool initialize(const std::string& dbPath);
    QSqlDatabase& getDatabase();
    bool isConnected() const;
private:
    DatabaseConnection() = default;
    QSqlDatabase db_; bool connected_{false};
};
