#pragma once
#include <string>

class QuarantineManager {
public:
    explicit QuarantineManager(const std::string& quarantinePath);
    bool quarantineFile(const std::string& filePath, const std::string& virusName);
    
private:
    std::string quarantinePath_;
    std::string generateQuarantineName(const std::string& originalPath);
};
