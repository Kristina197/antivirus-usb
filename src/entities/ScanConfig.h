#pragma once
#include <string>
#include <vector>

struct ScanConfig {
    // Глубина рекурсии для директорий
    int maxDepth = 10;
    
    // Максимальный размер файла (в МБ, 0 = без ограничений)
    int maxFileSizeMB = 100;
    
    // Типы файлов для сканирования (пусто = все файлы)
    std::vector<std::string> allowedExtensions;
    
    // Сканировать скрытые файлы
    bool scanHiddenFiles = false;
    
    // Следовать по символическим ссылкам
    bool followSymlinks = false;
    
    ScanConfig() {
        // По умолчанию сканируем исполняемые и документы
        allowedExtensions = {
            ".exe", ".dll", ".so", ".bin", ".sh",
            ".doc", ".docx", ".xls", ".xlsx", ".pdf",
            ".zip", ".rar", ".tar", ".gz", ".7z"
        };
    }
    
    bool shouldScanFile(const std::string& filePath) const;
};
