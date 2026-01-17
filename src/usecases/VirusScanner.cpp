#include "VirusScanner.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <openssl/evp.h>

namespace fs = std::filesystem;

VirusScanner::VirusScanner(std::shared_ptr<ISignatureRepository> signatureRepo,
                           std::shared_ptr<QuarantineManager> quarantineManager)
    : signatureRepo_(signatureRepo), quarantineManager_(quarantineManager) {}

void VirusScanner::scanDirectory(const std::string& path, std::vector<ScanResult>& results) {
    scanDirectoryRecursive(path, results, 0);
}

void VirusScanner::scanDirectoryRecursive(const std::string& path, std::vector<ScanResult>& results, int currentDepth) {
    try {
        if (currentDepth >= config_.maxDepth) {
            return;
        }

        if (!fs::exists(path) || !fs::is_directory(path)) {
            return;
        }

        auto dirOptions = fs::directory_options::skip_permission_denied;
        if (config_.followSymlinks) {
            dirOptions = dirOptions | fs::directory_options::follow_directory_symlink;
        }

        for (const auto& entry : fs::directory_iterator(path, dirOptions)) {
            try {
                if (entry.is_regular_file()) {
                    std::string filePath = entry.path().string();
                    if (config_.shouldScanFile(filePath)) {
                        scanFile(filePath, results);
                    }
                } else if (entry.is_directory()) {
                    scanDirectoryRecursive(entry.path().string(), results, currentDepth + 1);
                }
            } catch (const std::exception& e) {
                std::cerr << "Error processing entry: " << e.what() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
    }
}

bool VirusScanner::scanFile(const std::string& filePath, std::vector<ScanResult>& results) {
    std::vector<Signature> signatures = signatureRepo_->getAllSignatures();
    
    std::cout << "Scanning file: " << filePath << std::endl;
    std::cout << "Signatures available: " << signatures.size() << std::endl;
    
    for (const auto& sig : signatures) {
        std::cout << "   Checking signature: " << sig.virusName 
                  << " (hash: " << sig.signatureHash.substr(0, 8) << "...)" << std::endl;
        
        if (sig.signatureLength == 0) {
            std::cout << "Skipping - signatureLength is 0" << std::endl;
            continue;
        }
        
        std::string hash = calculateMD5(filePath, sig.fileOffset, sig.signatureLength);
        std::cout << "File hash: " << hash.substr(0, 8) << "..." << std::endl;
        std::cout << "Expected : " << sig.signatureHash.substr(0, 8) << "..." << std::endl;
        
        if (hash == sig.signatureHash) {
            ScanResult result(filePath, true, sig.virusName);
            results.push_back(result);
            
            std::cout << "VIRUS DETECTED: " << sig.virusName 
                      << " in file: " << filePath << std::endl;
            
            return true;
        }
    }
    
    return false;
}

bool VirusScanner::checkFileSignature(const std::string& filePath, const std::string& hash,
                                     std::vector<ScanResult>& results) {
    std::vector<Signature> signatures = signatureRepo_->getAllSignatures();
    
    for (const auto& sig : signatures) {
        if (sig.signatureHash == hash) {
            results.push_back(ScanResult(filePath, true, sig.virusName));
            return true;
        }
    }
    
    return false;
}

std::string VirusScanner::calculateMD5(const std::string& filePath, uint32_t offset, uint32_t length) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) return "";

    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (offset >= fileSize) return "";

    file.seekg(offset);
    uint32_t readLength = std::min(length, static_cast<uint32_t>(fileSize - offset));

    std::vector<char> buffer(readLength);
    if (!file.read(buffer.data(), readLength)) return "";

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) return "";

    unsigned char md5_result[EVP_MAX_MD_SIZE];
    unsigned int md5_len = 0;

    if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    if (EVP_DigestUpdate(ctx, buffer.data(), readLength) != 1) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    if (EVP_DigestFinal_ex(ctx, md5_result, &md5_len) != 1) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    EVP_MD_CTX_free(ctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < md5_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md5_result[i]);
    }

    return ss.str();
}
