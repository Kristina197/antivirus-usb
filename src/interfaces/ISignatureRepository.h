#pragma once
#include <vector>
#include "../entities/Signature.h"

class ISignatureRepository {
public:
    virtual ~ISignatureRepository() = default;
    virtual std::vector<Signature> getAllSignatures() = 0;
};
