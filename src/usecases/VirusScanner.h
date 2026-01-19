#pragma once
#include <string>
#include <vector>
#include <memory>
#include "IVirusScanner.h"
#include "../entities/ScanResult.h"
#include "../entities/ScanConfig.h"
#include "../interfaces/ISignatureRepository.h"
#include "QuarantineManager.h"

class VirusScanner : public IVirusScanner {
public:
    VirusScanner(std::shared_ptr<ISignatureRepository> signatureRepo,
                 std::shared_ptr<QuarantineManager> quarantineManager);
    
    bool scanFile(const std::string& filePath, std::vector<ScanResult>& results);
    void scanDirectory(const std::string& directoryPath, std::vector<ScanResult>& results, int& totalFiles) override;
    
    void setScanConfig(const ScanConfig& config) { this->config = config; }
    ScanConfig getScanConfig() { return config; }

private:
    std::shared_ptr<ISignatureRepository> signatureRepo_;
    std::shared_ptr<QuarantineManager> quarantineManager_;
    ScanConfig config;
    
    void scanDirectoryRecursive(const std::string& path, std::vector<ScanResult>& results, int& totalFiles, int currentDepth);
    bool checkFileSignature(const std::string& filePath, const std::string& hash, std::vector<ScanResult>& results);
    std::string calculateMD5(const std::string& filePath, uint32_t offset, uint32_t length);
};
