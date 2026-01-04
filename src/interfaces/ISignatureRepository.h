#pragma once
#include "../entities/Signature.h"
#include <vector>
class ISignatureRepository {
public:
    virtual ~ISignatureRepository() = default;
    virtual bool addSignature(const Signature& sig) = 0;
    virtual std::vector<Signature> getAllSignatures() = 0;
    virtual Signature getSignatureByName(const std::string& name) = 0;
    virtual bool deleteSignature(int id) = 0;
};
