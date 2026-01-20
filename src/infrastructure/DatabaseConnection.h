#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QSqlDatabase>
#include <QString>

class DatabaseConnection {
public:
    DatabaseConnection();
    ~DatabaseConnection();
    
    QSqlDatabase& getDatabase();
    bool isConnected() const;

private:
    QSqlDatabase db_;
    bool initializeConnection();
    bool createSchema();
    
    // PostgreSQL настройки
    static constexpr const char* DB_DRIVER = "QPSQL";
    static constexpr const char* DB_HOST = "localhost";
    static constexpr const char* DB_NAME = "antivirus_usb";
    static constexpr const char* DB_USER = "antivirus_user";
    static constexpr const char* DB_PASSWORD = "antivirus_password_2026";
    static constexpr int DB_PORT = 5432;
};

#endif // DATABASECONNECTION_H
