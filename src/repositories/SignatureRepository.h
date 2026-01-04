#pragma once
#include "../interfaces/ISignatureRepository.h"
#include <QSqlDatabase>
class SignatureRepository : public ISignatureRepository {
public:
    explicit SignatureRepository(QSqlDatabase& db);
    bool addSignature(const Signature& sig) override;
    std::vector<Signature> getAllSignatures() override;
    Signature getSignatureByName(const std::string& name) override;
    bool deleteSignature(int id) override;
private:
    QSqlDatabase& db_;
};
