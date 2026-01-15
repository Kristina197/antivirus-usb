#pragma once
#include <QSqlDatabase>

class DatabaseConnection {
public:
    DatabaseConnection();
    QSqlDatabase getDatabase() const;
    
private:
    QSqlDatabase db_;
};
