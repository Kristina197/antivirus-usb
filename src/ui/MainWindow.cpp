#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTableWidgetItem>
MainWindow::MainWindow(std::shared_ptr<IVirusScanner> scanner, std::shared_ptr<IUsbMonitor> usbMonitor,
                       std::shared_ptr<IDeviceRepository> deviceRepo, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), scanner_(scanner), usbMonitor_(usbMonitor), deviceRepo_(deviceRepo) {
    ui->setupUi(this);
    setWindowTitle("USB Antivirus Scanner");
    ui->resultsTable->setColumnCount(3);
    ui->resultsTable->setHorizontalHeaderLabels({"File Path", "Virus Name", "Severity"});
    ui->resultsTable->horizontalHeader()->setStretchLastSection(true);
    setupConnections();
    if (scanner_->loadSignatures()) addLogMessage("Signatures loaded successfully");
    else addLogMessage("Warning: No signatures loaded");
    updateDeviceList();
    usbMonitor_->setOnDeviceConnectedCallback([this](const DeviceInfo& device) {
        QMetaObject::invokeMethod(this, [this, device]() { onDeviceConnected(device); }, Qt::QueuedConnection);
    });
    usbMonitor_->startMonitoring();
    refreshTimer_ = new QTimer(this);
    connect(refreshTimer_, &QTimer::timeout, this, &MainWindow::updateDeviceList);
    refreshTimer_->start(5000);
}
MainWindow::~MainWindow() { usbMonitor_->stopMonitoring(); delete ui; }
void MainWindow::setupConnections() {
    connect(ui->scanButton, &QPushButton::clicked, this, &MainWindow::onScanButtonClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshDevicesClicked);
}
void MainWindow::updateDeviceList() {
    ui->deviceList->clear();
    auto devices = usbMonitor_->getConnectedDevices();
    for (const auto& device : devices) {
        QString itemText = QString::fromStdString(device.deviceName + " (" + device.mountPoint + ")");
        ui->deviceList->addItem(itemText);
    }
    if (devices.empty()) ui->deviceList->addItem("No USB devices detected");
}
void MainWindow::onScanButtonClicked() {
    auto selectedItems = ui->deviceList->selectedItems();
    if (selectedItems.isEmpty()) { QMessageBox::warning(this, "Warning", "Please select a device to scan"); return; }
    QString deviceText = selectedItems[0]->text();
    if (deviceText == "No USB devices detected") return;
    int start = deviceText.indexOf('(') + 1;
    int end = deviceText.indexOf(')');
    QString mountPoint = deviceText.mid(start, end - start);
    DeviceInfo device("", mountPoint.toStdString());
    scanDevice(device);
}
void MainWindow::onRefreshDevicesClicked() { updateDeviceList(); addLogMessage("Device list refreshed"); }
void MainWindow::onDeviceConnected(const DeviceInfo& device) {
    QString msg = QString("USB device connected: %1").arg(QString::fromStdString(device.mountPoint));
    addLogMessage(msg); updateDeviceList();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Auto Scan", "USB device detected. Scan now?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) scanDevice(device);
}
void MainWindow::scanDevice(const DeviceInfo& device) {
    ui->resultsTable->setRowCount(0);
    addLogMessage(QString("Scanning: %1").arg(QString::fromStdString(device.mountPoint)));
    ui->scanButton->setEnabled(false);
    ui->progressBar->setValue(0);
    auto results = scanner_->scanDirectory(device.mountPoint);
    ui->progressBar->setValue(100);
    deviceRepo_->recordDeviceScan(device);
    if (results.empty()) {
        addLogMessage("Scan complete: No threats found");
        QMessageBox::information(this, "Scan Complete", "No threats detected!");
    } else {
        addLogMessage(QString("Scan complete: %1 threats found").arg(results.size()));
        for (const auto& result : results) {
            int row = ui->resultsTable->rowCount();
            ui->resultsTable->insertRow(row);
            ui->resultsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(result.filePath)));
            ui->resultsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(result.virusName)));
            ui->resultsTable->setItem(row, 2, new QTableWidgetItem(QString::number(result.severity)));
            deviceRepo_->recordScanResult(device.mountPoint, result);
        }
        QMessageBox::warning(this, "Threats Detected", QString("Found %1 infected files!").arg(results.size()));
    }
    ui->scanButton->setEnabled(true);
}
void MainWindow::addLogMessage(const QString& message) {
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->logText->append(QString("[%1] %2").arg(timestamp, message));
}
