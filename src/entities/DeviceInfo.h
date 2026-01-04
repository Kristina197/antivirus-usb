#pragma once
#include <string>
struct DeviceInfo {
    std::string deviceName; std::string mountPoint; std::string devicePath;
    DeviceInfo() = default;
    DeviceInfo(const std::string& name, const std::string& mount, const std::string& path = "")
        : deviceName(name), mountPoint(mount), devicePath(path) {}
};
