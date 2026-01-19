#pragma once
#include <QObject>
#include <QString>
#include <memory>
#include "../usecases/VirusScanner.h"
#include "../entities/DeviceInfo.h"
#include "../entities/ScanResult.h"
class ScanWorker : public QObject {
    Q_OBJECT
public:
    ScanWorker(std::shared_ptr<VirusScanner> scanner, const DeviceInfo& device)
        : scanner_(scanner), device_(device) {}
public slots:
    void doScan() {
        std::vector<ScanResult> results;
        int totalFiles = 0;
        scanner_->scanDirectory(device_.mountPoint, results, totalFiles);
        emit scanFinished(results, totalFiles);
    }
signals:
    void scanFinished(const std::vector<ScanResult>& results, int totalFiles);
private:
    std::shared_ptr<VirusScanner> scanner_;
    DeviceInfo device_;
};
