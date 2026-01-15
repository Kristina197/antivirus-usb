#pragma once
#include <string>

struct ScanResult {
    std::string filePath;
    bool isInfected;
    std::string virusName;
    
    ScanResult() = default;
    ScanResult(const std::string& path, bool infected, const std::string& virus)
        : filePath(path), isInfected(infected), virusName(virus) {}
};
