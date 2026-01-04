#pragma once
#include <string>
#include <cstdint>
struct Signature {
    int id{0}; std::string virusName; std::string signatureHash;
    uint32_t fileOffset{0}; uint32_t signatureLength{0}; int severity{1};
    Signature() = default;
    Signature(const std::string& name, const std::string& hash, uint32_t offset, uint32_t length, int sev = 1)
        : virusName(name), signatureHash(hash), fileOffset(offset), signatureLength(length), severity(sev) {}
};
