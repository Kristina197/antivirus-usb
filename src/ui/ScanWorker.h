#pragma once
#include <QObject>
#include <QThread>
#include <memory>
#include "../usecases/IVirusScanner.h"
#include "../entities/DeviceInfo.h"
#include "../entities/ScanResult.h"

class ScanWorker : public QObject {
    Q_OBJECT
    
public:
    ScanWorker(std::shared_ptr<IVirusScanner> scanner, const DeviceInfo& device)
        : scanner_(scanner), device_(device) {}
    
public slots:
    void doScan() {
        emit progressChanged(10);
        
        // ВРЕМЕННАЯ ЗАДЕРЖКА ДЛЯ ТЕСТА
        QThread::sleep(2);
        emit progressChanged(30);
        
        QThread::sleep(2);
        emit progressChanged(50);
        
        // Реальное сканирование
        auto results = scanner_->scanDirectory(device_.mountPoint);
        
        emit progressChanged(80);
        QThread::sleep(1);
        
        emit progressChanged(100);
        emit scanFinished(results);
    }
    
signals:
    void progressChanged(int value);
    void scanFinished(const std::vector<ScanResult>& results);
    
private:
    std::shared_ptr<IVirusScanner> scanner_;
    DeviceInfo device_;
};
