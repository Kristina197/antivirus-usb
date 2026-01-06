#include "QuarantineManager.h"
#include <filesystem>
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

QuarantineManager::QuarantineManager(const std::string& quarantinePath) 
    : quarantinePath_(quarantinePath) {
    try {
        if (!fs::exists(quarantinePath_)) {
            fs::create_directories(quarantinePath_);
            std::cout << "✓ Created quarantine directory: " << quarantinePath_ << std::endl;
        } else {
            std::cout << "✓ Quarantine directory exists: " << quarantinePath_ << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "✗ Failed to create quarantine directory: " << e.what() << std::endl;
    }
}

bool QuarantineManager::quarantineFile(const std::string& filePath, const std::string& virusName) {
    try {
        std::cout << "\n=== QUARANTINE OPERATION ===" << std::endl;
        std::cout << "Original file: " << filePath << std::endl;
        
        if (!fs::exists(filePath)) {
            std::cerr << "✗ File does not exist: " << filePath << std::endl;
            return false;
        }
        
        std::cout << "✓ File exists, size: " << fs::file_size(filePath) << " bytes" << std::endl;
        
        // Генерируем имя для файла в карантине
        std::string quarantineFileName = generateQuarantineName(filePath);
        std::string quarantineFilePath = quarantinePath_ + "/" + quarantineFileName;
        
        std::cout << "Quarantine path: " << quarantineFilePath << std::endl;
        
        // ИСПРАВЛЕНИЕ: копируем файл, затем удаляем оригинал
        // (fs::rename не работает между разными файловыми системами)
        fs::copy_file(filePath, quarantineFilePath, fs::copy_options::overwrite_existing);
        std::cout << "✓ File copied to quarantine" << std::endl;
        
        // Удаляем оригинальный файл
        fs::remove(filePath);
        std::cout << "✓ Original file deleted" << std::endl;
        
        // Проверяем что операция прошла успешно
        if (!fs::exists(filePath) && fs::exists(quarantineFilePath)) {
            std::cout << "✓ SUCCESS: File quarantined" << std::endl;
            std::cout << "  Original: DELETED from " << filePath << std::endl;
            std::cout << "  Quarantine: " << quarantineFilePath << std::endl;
            std::cout << "  Virus: " << virusName << std::endl;
            std::cout << "==========================\n" << std::endl;
            return true;
        } else {
            std::cerr << "✗ FAILED: File was not properly quarantined" << std::endl;
            return false;
        }
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "✗ Failed to quarantine file: " << e.what() << std::endl;
        std::cerr << "  Error code: " << e.code() << std::endl;
        return false;
    }
}

std::string QuarantineManager::generateQuarantineName(const std::string& originalPath) {
    fs::path path(originalPath);
    std::string filename = path.filename().string();
    
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << filename << "_" << timestamp << ".quarantine";
    
    return ss.str();
}
