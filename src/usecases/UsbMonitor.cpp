#include "UsbMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>
UsbMonitor::UsbMonitor() { udev_ = udev_new(); if (!udev_) std::cerr << "Failed to create udev\n"; }
UsbMonitor::~UsbMonitor() { stopMonitoring(); if (udev_) udev_unref(udev_); }
std::vector<DeviceInfo> UsbMonitor::getConnectedDevices() {
    std::vector<DeviceInfo> devices;
    if (!udev_) return devices;
    struct udev_enumerate* enumerate = udev_enumerate_new(udev_);
    udev_enumerate_add_match_subsystem(enumerate, "block");
    udev_enumerate_scan_devices(enumerate);
    struct udev_list_entry* devices_list = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry* entry;
    udev_list_entry_foreach(entry, devices_list) {
        const char* path = udev_list_entry_get_name(entry);
        struct udev_device* dev = udev_device_new_from_syspath(udev_, path);
        const char* devtype = udev_device_get_devtype(dev);
        if (devtype && strcmp(devtype, "partition") == 0) {
            const char* id_bus = udev_device_get_property_value(dev, "ID_BUS");
            if (id_bus && strcmp(id_bus, "usb") == 0) {
                const char* devnode = udev_device_get_devnode(dev);
                if (devnode) {
                    std::string mountPoint = getMountPoint(devnode);
                    if (!mountPoint.empty()) {
                        DeviceInfo info(devnode, mountPoint, devnode);
                        devices.push_back(info);
                    }
                }
            }
        }
        udev_device_unref(dev);
    }
    udev_enumerate_unref(enumerate);
    return devices;
}
void UsbMonitor::startMonitoring() {
    if (running_ || !udev_) return;
    monitor_ = udev_monitor_new_from_netlink(udev_, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(monitor_, "block", nullptr);
    udev_monitor_enable_receiving(monitor_);
    running_ = true;
    monitorThread_ = std::thread(&UsbMonitor::monitorLoop, this);
}
void UsbMonitor::stopMonitoring() {
    if (!running_) return;
    running_ = false;
    if (monitorThread_.joinable()) monitorThread_.join();
    if (monitor_) { udev_monitor_unref(monitor_); monitor_ = nullptr; }
}
void UsbMonitor::setOnDeviceConnectedCallback(std::function<void(const DeviceInfo&)> callback) {
    onDeviceConnected_ = callback;
}
void UsbMonitor::monitorLoop() {
    int fd = udev_monitor_get_fd(monitor_);
    while (running_) {
        fd_set fds; FD_ZERO(&fds); FD_SET(fd, &fds);
        struct timeval tv; tv.tv_sec = 1; tv.tv_usec = 0;
        int ret = select(fd + 1, &fds, nullptr, nullptr, &tv);
        if (ret > 0 && FD_ISSET(fd, &fds)) {
            struct udev_device* dev = udev_monitor_receive_device(monitor_);
            if (dev) {
                const char* action = udev_device_get_action(dev);
                if (action && strcmp(action, "add") == 0) {
                    const char* id_bus = udev_device_get_property_value(dev, "ID_BUS");
                    if (id_bus && strcmp(id_bus, "usb") == 0) {
                        const char* devnode = udev_device_get_devnode(dev);
                        if (devnode) {
                            sleep(2);
                            std::string mountPoint = getMountPoint(devnode);
                            if (!mountPoint.empty() && onDeviceConnected_) {
                                DeviceInfo info(devnode, mountPoint, devnode);
                                onDeviceConnected_(info);
                            }
                        }
                    }
                }
                udev_device_unref(dev);
            }
        }
    }
}
std::string UsbMonitor::getMountPoint(const std::string& devPath) {
    std::ifstream mtab("/proc/mounts");
    std::string line;
    while (std::getline(mtab, line)) {
        std::istringstream iss(line);
        std::string device, mountPoint;
        iss >> device >> mountPoint;
        if (device == devPath) return mountPoint;
    }
    return "";
}
