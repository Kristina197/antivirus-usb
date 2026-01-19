#pragma once
#include <string>
#include <vector>

struct ScanConfig {
    int maxDepth = 10;
    int maxFileSizeMB = 100;
    std::vector<std::string> allowedExtensions;
    bool scanHiddenFiles = false;
    bool followSymlinks = false;
    
    ScanConfig() {
        allowedExtensions = {
            ".exe", ".dll", ".so", ".bin", ".sh",
            ".doc", ".docx", ".xls", ".xlsx", 
            ".pdf", ".zip", ".rar", ".tar", ".gz", ".7z",
            ".txt"  // <- Добавили .txt
        };
    }
    
    bool shouldScanFile(const std::string& filePath) const;
};
