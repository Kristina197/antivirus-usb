#pragma once
#include <QMainWindow>
#include <QTimer>
#include <memory>
#include "../usecases/IVirusScanner.h"
#include "../usecases/IUsbMonitor.h"
#include "../interfaces/IDeviceRepository.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(std::shared_ptr<IVirusScanner> scanner, std::shared_ptr<IUsbMonitor> usbMonitor,
               std::shared_ptr<IDeviceRepository> deviceRepo, QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onScanButtonClicked();
    void onRefreshDevicesClicked();
    void onDeviceConnected(const DeviceInfo& device);
private:
    void setupConnections();
    void updateDeviceList();
    void scanDevice(const DeviceInfo& device);
    void addLogMessage(const QString& message);
    Ui::MainWindow *ui;
    std::shared_ptr<IVirusScanner> scanner_;
    std::shared_ptr<IUsbMonitor> usbMonitor_;
    std::shared_ptr<IDeviceRepository> deviceRepo_;
    QTimer* refreshTimer_;
};
