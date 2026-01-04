#include "VirusScanner.h"
#include <fstream>
#include <filesystem>
#include <openssl/md5.h>
#include <iomanip>
#include <sstream>
#include <iostream>
namespace fs = std::filesystem;
VirusScanner::VirusScanner(std::shared_ptr<ISignatureRepository> sigRepo) : sigRepo_(sigRepo) {}
bool VirusScanner::loadSignatures() {
    signatures_ = sigRepo_->getAllSignatures();
    std::cout << "Loaded " << signatures_.size() << " signatures\n";
    return !signatures_.empty();
}
std::vector<ScanResult> VirusScanner::scanDirectory(const std::string& path) {
    std::vector<ScanResult> results;
    if (!fs::exists(path)) return results;
    try {
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (entry.is_regular_file()) scanFile(entry.path().string(), results);
        }
    } catch (const fs::filesystem_error& e) { std::cerr << e.what() << "\n"; }
    return results;
}
bool VirusScanner::scanFile(const std::string& filePath, std::vector<ScanResult>& results) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) return false;
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.close();
    for (const auto& sig : signatures_) {
        if (fileSize < sig.fileOffset + sig.signatureLength) continue;
        std::string hash = calculateMD5(filePath, sig.fileOffset, sig.signatureLength);
        if (hash == sig.signatureHash) {
            ScanResult result(filePath, sig.virusName, sig.severity);
            results.push_back(result);
            std::cout << "THREAT: " << filePath << " -> " << sig.virusName << "\n";
            return true;
        }
    }
    return false;
}
std::string VirusScanner::calculateMD5(const std::string& filePath, uint32_t offset, uint32_t length) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) return "";
    file.seekg(offset);
    std::vector<unsigned char> buffer(length);
    file.read(reinterpret_cast<char*>(buffer.data()), length);
    file.close();
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(buffer.data(), buffer.size(), digest);
    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    return ss.str();
}
