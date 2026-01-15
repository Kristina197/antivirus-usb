#pragma once

#include "IUsbMonitor.h"
#include <libudev.h>
#include <thread>
#include <atomic>

class UsbMonitor : public IUsbMonitor {
    Q_OBJECT

public:
    UsbMonitor();
    ~UsbMonitor();

    void startMonitoring() override;
    void stopMonitoring() override;

private:
    void monitorLoop();
    std::string getMountPoint(const std::string& devicePath);

    struct udev* udev_;
    struct udev_monitor* monitor_;
    std::thread monitorThread_;
    std::atomic<bool> running_;
};
