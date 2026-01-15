#pragma once
#include <QObject>
#include "../entities/DeviceInfo.h"

class IUsbMonitor : public QObject {
    Q_OBJECT
    
public:
    virtual ~IUsbMonitor() = default;
    virtual void startMonitoring() = 0;
    virtual void stopMonitoring() = 0;
    
signals:
    void deviceConnected(const DeviceInfo& device);
    void deviceDisconnected(const DeviceInfo& device);
};
