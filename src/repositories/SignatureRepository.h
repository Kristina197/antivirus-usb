#pragma once
#include "../interfaces/ISignatureRepository.h"
#include <QSqlDatabase>
#include <memory>

class SignatureRepository : public ISignatureRepository {
public:
    explicit SignatureRepository(QSqlDatabase& db);
    
    std::vector<Signature> getAllSignatures() override;
    bool addSignature(const std::string& virusName, const std::string& hash, 
                     uint32_t offset, uint32_t length, int threatLevel = 5);
    bool findByHash(const std::string& hash, Signature& outSignature);
    
private:
    QSqlDatabase& db_;
    
    struct PreparedStatements {
        std::unique_ptr<QSqlQuery> getAllSignatures;
        std::unique_ptr<QSqlQuery> findByHash;
    };
};
