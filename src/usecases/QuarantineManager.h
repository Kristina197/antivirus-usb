#pragma once
#include <string>

class QuarantineManager {
public:
    explicit QuarantineManager(const std::string& quarantinePath);
    
    bool quarantineFile(const std::string& filePath, const std::string& virusName);
    bool restoreFile(const std::string& quarantinedFilePath, const std::string& restoreDir);
    bool deleteFile(const std::string& quarantinedFilePath);
    
    std::string getQuarantinePath() const { return quarantinePath_; }
    
private:
    std::string quarantinePath_;
    std::string generateQuarantineName(const std::string& originalPath);
};
