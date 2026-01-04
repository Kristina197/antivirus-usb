#pragma once
#include "IUsbMonitor.h"
#include <libudev.h>
#include <thread>
#include <atomic>
class UsbMonitor : public IUsbMonitor {
public:
    UsbMonitor(); ~UsbMonitor();
    std::vector<DeviceInfo> getConnectedDevices() override;
    void startMonitoring() override;
    void stopMonitoring() override;
    void setOnDeviceConnectedCallback(std::function<void(const DeviceInfo&)> callback) override;
private:
    void monitorLoop();
    std::string getMountPoint(const std::string& devPath);
    struct udev* udev_;
    struct udev_monitor* monitor_;
    std::thread monitorThread_;
    std::atomic<bool> running_{false};
    std::function<void(const DeviceInfo&)> onDeviceConnected_;
};
