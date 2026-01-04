#include "SignatureRepository.h"
#include "../infrastructure/SqlQueryLoader.h"
#include <QSqlQuery>
#include <QSqlError>
#include <iostream>
SignatureRepository::SignatureRepository(QSqlDatabase& db) : db_(db) {}
bool SignatureRepository::addSignature(const Signature& sig) {
    auto& loader = SqlQueryLoader::getInstance();
    QSqlQuery query(db_);
    query.prepare(QString::fromStdString(loader.getQuery("insert_signature")));
    query.addBindValue(QString::fromStdString(sig.virusName));
    query.addBindValue(QString::fromStdString(sig.signatureHash));
    query.addBindValue(static_cast<int>(sig.fileOffset));
    query.addBindValue(static_cast<int>(sig.signatureLength));
    query.addBindValue(sig.severity);
    return query.exec();
}
std::vector<Signature> SignatureRepository::getAllSignatures() {
    std::vector<Signature> signatures;
    auto& loader = SqlQueryLoader::getInstance();
    QSqlQuery query(db_);
    if (!query.exec(QString::fromStdString(loader.getQuery("get_all_signatures")))) return signatures;
    while (query.next()) {
        Signature sig;
        sig.id = query.value(0).toInt();
        sig.virusName = query.value(1).toString().toStdString();
        sig.signatureHash = query.value(2).toString().toStdString();
        sig.fileOffset = query.value(3).toUInt();
        sig.signatureLength = query.value(4).toUInt();
        sig.severity = query.value(5).toInt();
        signatures.push_back(sig);
    }
    return signatures;
}
Signature SignatureRepository::getSignatureByName(const std::string& name) {
    Signature sig;
    auto& loader = SqlQueryLoader::getInstance();
    QSqlQuery query(db_);
    query.prepare(QString::fromStdString(loader.getQuery("get_signature_by_name")));
    query.addBindValue(QString::fromStdString(name));
    if (query.exec() && query.next()) {
        sig.id = query.value(0).toInt();
        sig.virusName = query.value(1).toString().toStdString();
        sig.signatureHash = query.value(2).toString().toStdString();
        sig.fileOffset = query.value(3).toUInt();
        sig.signatureLength = query.value(4).toUInt();
        sig.severity = query.value(5).toInt();
    }
    return sig;
}
bool SignatureRepository::deleteSignature(int id) {
    auto& loader = SqlQueryLoader::getInstance();
    QSqlQuery query(db_);
    query.prepare(QString::fromStdString(loader.getQuery("delete_signature")));
    query.addBindValue(id);
    return query.exec();
}
