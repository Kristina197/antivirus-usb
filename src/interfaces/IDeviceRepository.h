#pragma once
#include "../entities/DeviceInfo.h"
#include "../entities/ScanResult.h"
#include <vector>
class IDeviceRepository {
public:
    virtual ~IDeviceRepository() = default;
    virtual bool recordDeviceScan(const DeviceInfo& device) = 0;
    virtual bool recordScanResult(const std::string& devicePath, const ScanResult& result) = 0;
    virtual std::vector<DeviceInfo> getAllDevices() = 0;
};
