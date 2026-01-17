#include "SignatureRepository.h"
#include "../infrastructure/SqlQueryLoader.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <iostream>
#include <sstream>

SignatureRepository::SignatureRepository(QSqlDatabase& db) : db_(db) {}

std::vector<Signature> SignatureRepository::getAllSignatures() {
    std::vector<Signature> signatures;

    QString sql = R"(
        SELECT 
            v.signature_id,
            v.virus_name,
            h.hash_value as signature_hash,
            h.file_offset,
            h.signature_length,
            v.threat_level
        FROM virus_signatures v
        INNER JOIN signature_hashes h ON v.signature_id = h.signature_id
        ORDER BY v.virus_name
    )";

    QSqlQuery query(db_);
    if (!query.exec(sql)) {
        qCritical() << "Не удалось загрузить сигнатуры:" << query.lastError().text();
        return signatures;
    }

    while (query.next()) {
        Signature sig;
        sig.virusName = query.value(1).toString().toStdString();
        sig.signatureHash = query.value(2).toString().toStdString();
        sig.fileOffset = query.value(3).toUInt();
        sig.signatureLength = query.value(4).toUInt();
        signatures.push_back(sig);
    }

    return signatures;
}

bool SignatureRepository::findByHash(const std::string& hash, Signature& outSignature) {
    QString sql = R"(
        SELECT v.virus_name, h.hash_value, h.file_offset, h.signature_length
        FROM virus_signatures v
        INNER JOIN signature_hashes h ON v.signature_id = h.signature_id
        WHERE h.hash_value = ?
    )";

    QSqlQuery query(db_);
    query.prepare(sql);
    query.addBindValue(QString::fromStdString(hash));

    if (!query.exec()) {
        qCritical() << "Ошибка поиска по хешу:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        outSignature.virusName = query.value(0).toString().toStdString();
        outSignature.signatureHash = hash;
        outSignature.fileOffset = query.value(2).toUInt();
        outSignature.signatureLength = query.value(3).toUInt();
        return true;
    }

    return false;
}

bool SignatureRepository::addSignature(const std::string& virusName,
                                      const std::string& hash,
                                      uint32_t offset,
                                      uint32_t length,
                                      int threatLevel) {
    if (virusName.empty() || hash.empty() || hash.length() != 32) {
        qWarning() << "Некорректные данные сигнатуры";
        return false;
    }

    if (threatLevel < 1 || threatLevel > 10) {
        qWarning() << "Некорректный уровень угрозы";
        return false;
    }

    db_.transaction();

    QSqlQuery query(db_);
    query.prepare("INSERT OR IGNORE INTO virus_signatures (virus_name, threat_level) VALUES (?, ?)");
    query.addBindValue(QString::fromStdString(virusName));
    query.addBindValue(threatLevel);

    if (!query.exec()) {
        db_.rollback();
        qCritical() << "Не удалось добавить сигнатуру:" << query.lastError().text();
        return false;
    }

    int signatureId = query.lastInsertId().toInt();
    if (signatureId == 0) {
        query.prepare("SELECT signature_id FROM virus_signatures WHERE virus_name = ?");
        query.addBindValue(QString::fromStdString(virusName));
        if (query.exec() && query.next()) {
            signatureId = query.value(0).toInt();
        }
    }

    query.prepare("INSERT OR IGNORE INTO signature_hashes (signature_id, hash_value, file_offset, signature_length) VALUES (?, ?, ?, ?)");
    query.addBindValue(signatureId);
    query.addBindValue(QString::fromStdString(hash));
    query.addBindValue(offset);
    query.addBindValue(length);

    if (!query.exec()) {
        db_.rollback();
        return false;
    }

    db_.commit();
    return true;
}
