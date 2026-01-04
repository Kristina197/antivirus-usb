#include "SqlQueryLoader.h"
#include <fstream>
#include <iostream>
SqlQueryLoader& SqlQueryLoader::getInstance() { static SqlQueryLoader instance; return instance; }
bool SqlQueryLoader::loadQueriesFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) { std::cerr << "Failed to open SQL file\n"; return false; }
    std::string line, currentQuery, currentName;
    bool inQuery = false;
    while (std::getline(file, line)) {
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        line = line.substr(start);
        if (line.substr(0, 2) == "--") {
            if (line.find("@name:") != std::string::npos) {
                if (!currentName.empty() && !currentQuery.empty()) queries_[currentName] = currentQuery;
                currentName = line.substr(line.find(":") + 1);
                currentName.erase(0, currentName.find_first_not_of(" \t"));
                currentName.erase(currentName.find_last_not_of(" \t\r\n") + 1);
                currentQuery.clear(); inQuery = true;
            }
            continue;
        }
        if (inQuery && !line.empty()) currentQuery += line + "\n";
    }
    if (!currentName.empty() && !currentQuery.empty()) queries_[currentName] = currentQuery;
    file.close(); return !queries_.empty();
}
std::string SqlQueryLoader::getQuery(const std::string& queryName) const {
    auto it = queries_.find(queryName);
    return (it != queries_.end()) ? it->second : "";
}
