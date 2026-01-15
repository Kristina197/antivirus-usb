#include "DeviceRepository.h"
#include "../infrastructure/SqlQueryLoader.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

DeviceRepository::DeviceRepository(QSqlDatabase& db) : db_(db) {}

bool DeviceRepository::recordDeviceScan(const DeviceInfo& device) {
    auto& loader = SqlQueryLoader::getInstance();
    QString sql = loader.getQuery("insert_or_update_device");
    
    QSqlQuery query(db_);
    query.prepare(sql);
    query.addBindValue(QString::fromStdString(device.deviceName));
    query.addBindValue(QString::fromStdString(device.mountPoint));
    
    if (!query.exec()) {
        qCritical() << "Failed to record device:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DeviceRepository::recordScanResult(const std::string& mountPoint, const ScanResult& result) {
    auto& loader = SqlQueryLoader::getInstance();
    QString sql = loader.getQuery("insert_scan_result");
    
    QSqlQuery query(db_);
    query.prepare(sql);
    query.addBindValue(QString::fromStdString(mountPoint));
    query.addBindValue(QString::fromStdString(result.filePath));
    query.addBindValue(result.isInfected);
    query.addBindValue(QString::fromStdString(result.virusName));
    
    if (!query.exec()) {
        qCritical() << "Failed to record scan result:" << query.lastError().text();
        return false;
    }
    return true;
}

std::vector<DeviceInfo> DeviceRepository::getAllDevices() {
    std::vector<DeviceInfo> devices;
    auto& loader = SqlQueryLoader::getInstance();
    QString sql = loader.getQuery("get_all_devices");
    
    QSqlQuery query(db_);
    if (!query.exec(sql)) {
        qCritical() << "Failed to get devices:" << query.lastError().text();
        return devices;
    }
    
    while (query.next()) {
        DeviceInfo device;
        device.deviceName = query.value(0).toString().toStdString();
        device.mountPoint = query.value(1).toString().toStdString();
        devices.push_back(device);
    }
    
    return devices;
}
