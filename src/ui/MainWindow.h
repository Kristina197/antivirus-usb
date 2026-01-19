#pragma once

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QThread>
#include <memory>

#include "../usecases/IVirusScanner.h"
#include "../usecases/VirusScanner.h"
#include "../usecases/IUsbMonitor.h"
#include "../interfaces/IDeviceRepository.h"
#include "../usecases/QuarantineManager.h"
#include "../entities/DeviceInfo.h"
#include "../entities/ScanResult.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<VirusScanner> scanner,
               std::shared_ptr<IUsbMonitor> usbMonitor,
               std::shared_ptr<IDeviceRepository> deviceRepo,
               std::shared_ptr<QuarantineManager> quarantineManager,
               QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDeviceConnected(const DeviceInfo& device);
    void onDeviceDisconnected(const DeviceInfo& device);
    void onScanButtonClicked();
    void onQuarantineButtonClicked();
    void onSettingsButtonClicked();
    void onScanFinished(const DeviceInfo& device, const std::vector<ScanResult>& results, int totalFiles);
    void showThreatSummary(int threatsFound, int totalFiles);
    void handleInfectedFile(const QString& filePath, const QString& virusName);

private:
    void setupConnections();
    void setupDeviceTable();
    void setupResultsTable();
    void scanDevice(const DeviceInfo& device);
    void updateDeviceStatus(const QString& deviceName, int threatsCount);

    Ui::MainWindow *ui;
    std::shared_ptr<VirusScanner> scanner_;
    std::shared_ptr<IUsbMonitor> usbMonitor_;
    std::shared_ptr<IDeviceRepository> deviceRepo_;
    std::shared_ptr<QuarantineManager> quarantineManager_;
    std::vector<DeviceInfo> connectedDevices_;
    QThread* scanThread_;
    DeviceInfo currentScannedDevice_;
};
