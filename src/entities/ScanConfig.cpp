#include "ScanConfig.h"
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

bool ScanConfig::shouldScanFile(const std::string& filePath) const {
    fs::path path(filePath);

    if (!scanHiddenFiles && path.filename().string()[0] == '.') {
        return false;
    }
    
    if (maxFileSizeMB > 0) {
        try {
            auto fileSize = fs::file_size(filePath);
            if (fileSize > static_cast<uintmax_t>(maxFileSizeMB) * 1024 * 1024) {
                return false;
            }
        } catch (...) {
            return false;
        }
    }
    
    if (!allowedExtensions.empty()) {
        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        auto it = std::find(allowedExtensions.begin(), allowedExtensions.end(), ext);
        if (it == allowedExtensions.end()) {
            return false;
        }
    }
    
    return true;
}
