#pragma once
#include "../entities/ScanResult.h"
#include <vector>
#include <string>
class IVirusScanner {
public:
    virtual ~IVirusScanner() = default;
    virtual std::vector<ScanResult> scanDirectory(const std::string& path) = 0;
    virtual bool loadSignatures() = 0;
};
