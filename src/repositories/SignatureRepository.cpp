#include "SignatureRepository.h"
#include "../infrastructure/SqlQueryLoader.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QCoreApplication>

SignatureRepository::SignatureRepository(QSqlDatabase& database)
    : db(database) {
    loadQueries();
}

void SignatureRepository::loadQueries() {
    SqlQueryLoader& loader = SqlQueryLoader::getInstance();
    
    QString basePath = QCoreApplication::applicationDirPath() + "/../";
    
    sqlGetAll = loader.loadSingleQuery(basePath + "sql/queries/get_all_signatures.sql");
    sqlInsertVirus = loader.loadSingleQuery(basePath + "sql/queries/insert_virus_signature.sql");
    sqlGetSignatureId = loader.loadSingleQuery(basePath + "sql/queries/get_signature_id.sql");
    sqlInsertHash = loader.loadSingleQuery(basePath + "sql/queries/insert_signature_hash.sql");
    
    qDebug() << "SQL queries loaded, GetAll length:" << sqlGetAll.length();
}

std::vector<Signature> SignatureRepository::getAllSignatures() {
    std::vector<Signature> signatures;
    QSqlQuery query(db);
    
    if (sqlGetAll.isEmpty()) {
        qCritical() << "SQL query is empty";
        return signatures;
    }
    
    if (!query.exec(sqlGetAll)) {
        qWarning() << "Failed to get signatures:" << query.lastError().text();
        return signatures;
    }
    
    while (query.next()) {
        Signature sig;
        sig.virusName = query.value(0).toString().toStdString();
        sig.signatureHash = query.value(1).toString().toStdString();
        sig.fileOffset = query.value(2).toUInt();
        sig.signatureLength = query.value(3).toUInt();
        signatures.push_back(sig);
    }
    
    qDebug() << "Loaded" << signatures.size() << "signatures from database";
    return signatures;
}

bool SignatureRepository::addSignature(const std::string& name, 
                                       const std::string& md5Hash,
                                       int fileOffset, 
                                       int patternSize,
                                       int threatLevel) {
    QSqlQuery query(db);
    
    query.prepare(sqlInsertVirus);
    query.bindValue(0, QString::fromStdString(name));
    query.bindValue(1, threatLevel == 1 ? "high" : "medium");
    
    if (!query.exec()) {
        qWarning() << "Failed to insert virus:" << query.lastError().text();
        return false;
    }
    
    query.prepare(sqlGetSignatureId);
    query.bindValue(0, QString::fromStdString(name));
    
    if (!query.exec() || !query.next()) {
        qWarning() << "Failed to get signature ID:" << query.lastError().text();
        return false;
    }
    
    int signatureId = query.value(0).toInt();
    
    query.prepare(sqlInsertHash);
    query.bindValue(0, signatureId);
    query.bindValue(1, QString::fromStdString(md5Hash));
    query.bindValue(2, fileOffset);
    query.bindValue(3, patternSize);
    
    if (!query.exec()) {
        qWarning() << "Failed to insert hash:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "Added signature:" << QString::fromStdString(name);
    return true;
}
