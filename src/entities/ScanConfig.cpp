#include "ScanConfig.h"
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

bool ScanConfig::shouldScanFile(const std::string& filePath) const {
    fs::path path(filePath);
    
    // Проверка скрытых файлов
    if (!scanHiddenFiles && path.filename().string()[0] == '.') {
        return false;
    }
    
    // Проверка размера файла
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
    
    // Проверка расширения (если список не пустой)
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
