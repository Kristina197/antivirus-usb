#pragma once
#include <string>
struct ScanResult {
    std::string filePath; std::string virusName; int severity{0}; bool isInfected{false};
    ScanResult() = default;
    ScanResult(const std::string& path, const std::string& virus, int sev)
        : filePath(path), virusName(virus), severity(sev), isInfected(true) {}
};
