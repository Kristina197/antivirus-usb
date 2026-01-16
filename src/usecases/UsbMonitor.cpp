#include "UsbMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <mntent.h>
#include <chrono>

UsbMonitor::UsbMonitor() : udev_(nullptr), monitor_(nullptr), running_(false) {
    udev_ = udev_new();
    if (!udev_) {
        std::cerr << "✗ Failed to create udev context" << std::endl;
        return;
    }

    monitor_ = udev_monitor_new_from_netlink(udev_, "udev");
    if (!monitor_) {
        std::cerr << "✗ Failed to create udev monitor" << std::endl;
        return;
    }

    udev_monitor_filter_add_match_subsystem_devtype(monitor_, "block", "disk");
    udev_monitor_enable_receiving(monitor_);
    std::cout << "✓ USB Monitor initialized" << std::endl;
}

UsbMonitor::~UsbMonitor() {
    stopMonitoring();
    if (monitor_) {
        udev_monitor_unref(monitor_);
    }
    if (udev_) {
        udev_unref(udev_);
    }
}

void UsbMonitor::startMonitoring() {
    if (running_) return;
    running_ = true;
    monitorThread_ = std::thread(&UsbMonitor::monitorLoop, this);
    std::cout << "✓ USB monitoring started" << std::endl;
}

void UsbMonitor::stopMonitoring() {
    if (!running_) return;
    running_ = false;
    if (monitorThread_.joinable()) {
        monitorThread_.join();
    }
    std::cout << "✓ USB monitoring stopped" << std::endl;
}

std::string UsbMonitor::getMountPoint(const std::string& devicePath) {
    FILE* mountsFile = setmntent("/proc/mounts", "r");
    if (!mountsFile) {
        std::cerr << "✗ Failed to open /proc/mounts" << std::endl;
        return "";
    }

    struct mntent* entry;
    std::string mountPoint;

    while ((entry = getmntent(mountsFile)) != nullptr) {
        if (std::string(entry->mnt_fsname) == devicePath) {
            mountPoint = entry->mnt_dir;
            break;
        }
        
        if (devicePath.find(entry->mnt_fsname) != std::string::npos ||
            std::string(entry->mnt_fsname).find(devicePath) != std::string::npos) {
            mountPoint = entry->mnt_dir;
            break;
        }
    }

    endmntent(mountsFile);
    
    if (mountPoint.empty()) {
        std::cerr << "⚠ Mount point not found for device: " << devicePath << std::endl;
    }
    
    return mountPoint;
}

void UsbMonitor::monitorLoop() {
    int fd = udev_monitor_get_fd(monitor_);
    
    while (running_) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int ret = select(fd + 1, &fds, nullptr, nullptr, &tv);
        if (ret > 0 && FD_ISSET(fd, &fds)) {
            struct udev_device* dev = udev_monitor_receive_device(monitor_);
            if (!dev) continue;

            const char* action = udev_device_get_action(dev);
            const char* devnode = udev_device_get_devnode(dev);
            const char* id_bus = udev_device_get_property_value(dev, "ID_BUS");

            if (action && devnode && id_bus && strcmp(id_bus, "usb") == 0) {
                DeviceInfo device;
                device.deviceName = devnode;
                
                if (strcmp(action, "add") == 0) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    
                    device.mountPoint = getMountPoint(devnode);
                    
                    if (device.mountPoint.empty()) {
                        std::string partition = std::string(devnode) + "1";
                        device.mountPoint = getMountPoint(partition);
                    }
                    
                    if (!device.mountPoint.empty()) {
                        std::cout << "✓ USB device connected: " << devnode 
                                  << " mounted at " << device.mountPoint << std::endl;
                        emit deviceConnected(device);
                    } else {
                        std::cerr << "⚠ USB device connected but not mounted: " << devnode << std::endl;
                    }
                    
                } else if (strcmp(action, "remove") == 0) {
                    device.mountPoint = "";
                    std::cout << "✓ USB device disconnected: " << devnode << std::endl;
                    emit deviceDisconnected(device);
                }
            }

            udev_device_unref(dev);
        }
    }
}
