#ifndef SIGNATUREREPOSITORY_H
#define SIGNATUREREPOSITORY_H

#include "../interfaces/ISignatureRepository.h"
#include <QSqlDatabase>
#include <QString>

class SignatureRepository : public ISignatureRepository {
public:
    explicit SignatureRepository(QSqlDatabase& database);
    
    // Реализация интерфейса
    std::vector<Signature> getAllSignatures() override;
    
    // Дополнительные методы
    bool addSignature(const std::string& name, 
                     const std::string& md5Hash,
                     int fileOffset, 
                     int patternSize,
                     int threatLevel);

private:
    QSqlDatabase& db;
    
    // SQL запросы загружаются из файлов
    QString sqlGetAll;
    QString sqlInsertVirus;
    QString sqlGetSignatureId;
    QString sqlInsertHash;
    
    void loadQueries();
};

#endif // SIGNATUREREPOSITORY_H
