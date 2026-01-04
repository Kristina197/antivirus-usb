#pragma once
#include "../entities/DeviceInfo.h"
#include <vector>
#include <functional>
class IUsbMonitor {
public:
    virtual ~IUsbMonitor() = default;
    virtual std::vector<DeviceInfo> getConnectedDevices() = 0;
    virtual void startMonitoring() = 0;
    virtual void stopMonitoring() = 0;
    virtual void setOnDeviceConnectedCallback(std::function<void(const DeviceInfo&)> callback) = 0;
};
