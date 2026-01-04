#pragma once
#include <string>
#include <unordered_map>
class SqlQueryLoader {
public:
    static SqlQueryLoader& getInstance();
    bool loadQueriesFromFile(const std::string& filePath);
    std::string getQuery(const std::string& queryName) const;
private:
    SqlQueryLoader() = default;
    std::unordered_map<std::string, std::string> queries_;
};
