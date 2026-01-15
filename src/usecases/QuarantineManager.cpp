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
        }
    } catch (const std::exception& e) {
        std::cerr << "✗ Failed to create quarantine directory: " << e.what() << std::endl;
    }
}

bool QuarantineManager::quarantineFile(const std::string& filePath, const std::string& virusName) {
    try {
        if (!fs::exists(filePath)) {
            std::cerr << "✗ File does not exist: " << filePath << std::endl;
            return false;
        }
        
        std::string quarantineFileName = generateQuarantineName(filePath);
        std::string quarantineFilePath = quarantinePath_ + "/" + quarantineFileName;
        
        fs::copy_file(filePath, quarantineFilePath, fs::copy_options::overwrite_existing);
        fs::remove(filePath);
        
        std::cout << "✓ Quarantined: " << filePath << " -> " << quarantineFilePath << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Failed to quarantine file: " << e.what() << std::endl;
        return false;
    }
}

bool QuarantineManager::restoreFile(const std::string& quarantinedFilePath, const std::string& restoreDir) {
    try {
        if (!fs::exists(quarantinedFilePath)) {
            std::cerr << "✗ Quarantined file does not exist" << std::endl;
            return false;
        }
        
        if (!fs::exists(restoreDir)) {
            fs::create_directories(restoreDir);
        }
        
        fs::path quarantinedFile(quarantinedFilePath);
        std::string filename = quarantinedFile.filename().string();
        
        // Убираем timestamp и .quarantine
        std::string suffix = ".quarantine";
        if (filename.find("_") != std::string::npos && 
            filename.size() >= suffix.size() &&
            filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0) {
            size_t lastUnderscore = filename.rfind('_');
            filename = filename.substr(0, lastUnderscore);
        }
        
        fs::path restorePath = fs::path(restoreDir) / filename;
        
        fs::copy_file(quarantinedFilePath, restorePath, fs::copy_options::overwrite_existing);
        fs::remove(quarantinedFilePath);
        
        std::cout << "✓ Restored: " << quarantinedFilePath << " -> " << restorePath << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Failed to restore file: " << e.what() << std::endl;
        return false;
    }
}

bool QuarantineManager::deleteFile(const std::string& quarantinedFilePath) {
    try {
        if (!fs::exists(quarantinedFilePath)) {
            return false;
        }
        
        fs::remove(quarantinedFilePath);
        std::cout << "✓ Deleted from quarantine: " << quarantinedFilePath << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Failed to delete file: " << e.what() << std::endl;
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
