#pragma once
#include "IVirusScanner.h"
#include "../interfaces/ISignatureRepository.h"
#include "../entities/Signature.h"
#include "QuarantineManager.h"
#include <memory>
#include <vector>

class VirusScanner : public IVirusScanner {
public:
    explicit VirusScanner(std::shared_ptr<ISignatureRepository> sigRepo,
                         std::shared_ptr<QuarantineManager> quarantineManager = nullptr);
    std::vector<ScanResult> scanDirectory(const std::string& path) override;
    bool loadSignatures() override;
    
private:
    bool scanFile(const std::string& filePath, std::vector<ScanResult>& results);
    std::string calculateMD5(const std::string& filePath, uint32_t offset, uint32_t length);
    
    std::shared_ptr<ISignatureRepository> sigRepo_;
    std::shared_ptr<QuarantineManager> quarantineManager_;
    std::vector<Signature> signatures_;
};
