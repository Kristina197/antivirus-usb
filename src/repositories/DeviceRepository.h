#pragma once
#include "../interfaces/IDeviceRepository.h"
#include <QSqlDatabase>
class DeviceRepository : public IDeviceRepository {
public:
    explicit DeviceRepository(QSqlDatabase& db);
    bool recordDeviceScan(const DeviceInfo& device) override;
    bool recordScanResult(const std::string& devicePath, const ScanResult& result) override;
    std::vector<DeviceInfo> getAllDevices() override;
private:
    QSqlDatabase& db_;
};
