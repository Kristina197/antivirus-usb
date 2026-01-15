#pragma once
#include <string>
#include <vector>
#include "../entities/ScanResult.h"

class IVirusScanner {
public:
    virtual ~IVirusScanner() = default;
    virtual void scanDirectory(const std::string& path, std::vector<ScanResult>& results) = 0;
};
