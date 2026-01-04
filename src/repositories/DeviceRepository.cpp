#include "DeviceRepository.h"
#include "../infrastructure/SqlQueryLoader.h"
#include <QSqlQuery>
#include <iostream>
DeviceRepository::DeviceRepository(QSqlDatabase& db) : db_(db) {}
bool DeviceRepository::recordDeviceScan(const DeviceInfo& device) {
    auto& loader = SqlQueryLoader::getInstance();
    QSqlQuery query(db_);
    query.prepare(QString::fromStdString(loader.getQuery("insert_or_update_device")));
    query.addBindValue(QString::fromStdString(device.deviceName));
    query.addBindValue(QString::fromStdString(device.mountPoint));
    return query.exec();
}
bool DeviceRepository::recordScanResult(const std::string& devicePath, const ScanResult& result) {
    auto& loader = SqlQueryLoader::getInstance();
    QSqlQuery query(db_);
    query.prepare(QString::fromStdString(loader.getQuery("insert_scan_result")));
    query.addBindValue(QString::fromStdString(devicePath));
    query.addBindValue(QString::fromStdString(result.filePath));
    query.addBindValue(QString::fromStdString(result.virusName));
    return query.exec();
}
std::vector<DeviceInfo> DeviceRepository::getAllDevices() {
    std::vector<DeviceInfo> devices;
    auto& loader = SqlQueryLoader::getInstance();
    QSqlQuery query(db_);
    if (!query.exec(QString::fromStdString(loader.getQuery("get_all_devices")))) return devices;
    while (query.next()) {
        DeviceInfo dev;
        dev.deviceName = query.value(1).toString().toStdString();
        dev.mountPoint = query.value(2).toString().toStdString();
        devices.push_back(dev);
    }
    return devices;
}
