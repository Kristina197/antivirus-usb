#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "ScanWorker.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTableWidgetItem>
#include <QThread>

MainWindow::MainWindow(std::shared_ptr<IVirusScanner> scanner,
                       std::shared_ptr<IUsbMonitor> usbMonitor,
                       std::shared_ptr<IDeviceRepository> deviceRepo,
                       std::shared_ptr<QuarantineManager> quarantineManager,
                       QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), 
      scanner_(scanner), usbMonitor_(usbMonitor), deviceRepo_(deviceRepo),
      quarantineManager_(quarantineManager), scanThread_(nullptr) {
    ui->setupUi(this);
    setWindowTitle("USB Antivirus Scanner v2.0");
    
    ui->resultsTable->setColumnCount(3);
    ui->resultsTable->setHorizontalHeaderLabels({"File Path", "Virus Name", "Severity"});
    ui->resultsTable->horizontalHeader()->setStretchLastSection(true);
    ui->resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->resultsTable->setSelectionMode(QAbstractItemView::MultiSelection);
    
    setupConnections();
    
    if (scanner_->loadSignatures()) {
        addLogMessage("Signatures loaded successfully");
    } else {
        addLogMessage("Warning: No signatures loaded");
    }
    
    updateDeviceList();
    
    usbMonitor_->setOnDeviceConnectedCallback([this](const DeviceInfo& device) {
        QMetaObject::invokeMethod(this, [this, device]() {
            onDeviceConnected(device);
        }, Qt::QueuedConnection);
    });
    
    usbMonitor_->startMonitoring();
    
    refreshTimer_ = new QTimer(this);
    connect(refreshTimer_, &QTimer::timeout, this, &MainWindow::updateDeviceList);
    refreshTimer_->start(5000);
}

MainWindow::~MainWindow() {
    if (scanThread_ && scanThread_->isRunning()) {
        scanThread_->quit();
        scanThread_->wait(3000);
    }
    usbMonitor_->stopMonitoring();
    delete ui;
}

void MainWindow::setupConnections() {
    connect(ui->scanButton, &QPushButton::clicked, this, &MainWindow::onScanButtonClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshDevicesClicked);
    connect(ui->quarantineSelectedButton, &QPushButton::clicked, this, &MainWindow::onQuarantineSelectedClicked);
    connect(ui->clearResultsButton, &QPushButton::clicked, this, &MainWindow::onClearResultsClicked);
    connect(ui->resultsTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::onResultsTableSelectionChanged);
}

void MainWindow::updateDeviceList() {
    ui->deviceList->clear();
    auto devices = usbMonitor_->getConnectedDevices();
    
    for (const auto& device : devices) {
        QString itemText = QString::fromStdString(device.deviceName + " (" + device.mountPoint + ")");
        ui->deviceList->addItem(itemText);
    }
    
    if (devices.empty()) {
        ui->deviceList->addItem("No USB devices detected");
    }
}

void MainWindow::onScanButtonClicked() {
    if (scanThread_ && scanThread_->isRunning()) {
        QMessageBox::warning(this, "Scan in Progress", "A scan is already running. Please wait.");
        return;
    }
    
    auto selectedItems = ui->deviceList->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a device to scan");
        return;
    }
    
    QString deviceText = selectedItems[0]->text();
    if (deviceText == "No USB devices detected") return;
    
    int start = deviceText.indexOf('(') + 1;
    int end = deviceText.indexOf(')');
    QString mountPoint = deviceText.mid(start, end - start);
    
    DeviceInfo device("", mountPoint.toStdString());
    scanDevice(device);
}

void MainWindow::onRefreshDevicesClicked() {
    updateDeviceList();
    addLogMessage("Device list refreshed");
}

void MainWindow::onDeviceConnected(const DeviceInfo& device) {
    QString msg = QString("USB device connected: %1").arg(QString::fromStdString(device.mountPoint));
    addLogMessage(msg);
    updateDeviceList();
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Auto Scan", "USB device detected. Scan now?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        scanDevice(device);
    }
}

void MainWindow::onQuarantineSelectedClicked() {
    auto selectedRows = ui->resultsTable->selectionModel()->selectedRows();
    
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select files to quarantine");
        return;
    }
    
    QString message = QString("Quarantine %1 selected file(s)?\n\n"
                             "Files will be moved to quarantine folder.")
                     .arg(selectedRows.count());
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Quarantine", message,
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply != QMessageBox::Yes) {
        return;
    }
    
    int quarantined = 0;
    int failed = 0;
    
    // Сортируем строки в обратном порядке для правильного удаления
    QList<int> rowsToRemove;
    for (const auto& index : selectedRows) {
        rowsToRemove.append(index.row());
    }
    std::sort(rowsToRemove.begin(), rowsToRemove.end(), std::greater<int>());
    
    for (int row : rowsToRemove) {
        QString filePath = ui->resultsTable->item(row, 0)->text();
        QString virusName = ui->resultsTable->item(row, 1)->text();
        
        if (quarantineManager_->quarantineFile(filePath.toStdString(), virusName.toStdString())) {
            quarantined++;
            addLogMessage(QString("✓ Quarantined: %1").arg(filePath));
            ui->resultsTable->removeRow(row);
        } else {
            failed++;
            addLogMessage(QString("✗ Failed to quarantine: %1").arg(filePath));
        }
    }
    
    QString summary = QString("Quarantine complete:\n✓ Success: %1\n✗ Failed: %2")
                     .arg(quarantined).arg(failed);
    QMessageBox::information(this, "Quarantine Complete", summary);
    
    // Отключаем кнопку если таблица пуста
    if (ui->resultsTable->rowCount() == 0) {
        ui->quarantineSelectedButton->setEnabled(false);
    }
}

void MainWindow::onClearResultsClicked() {
    if (ui->resultsTable->rowCount() == 0) {
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Clear Results", 
        "Clear all scan results from table?\n(Files will NOT be quarantined)",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        ui->resultsTable->setRowCount(0);
        ui->quarantineSelectedButton->setEnabled(false);
        addLogMessage("Scan results cleared");
    }
}

void MainWindow::onResultsTableSelectionChanged() {
    bool hasSelection = !ui->resultsTable->selectionModel()->selectedRows().isEmpty();
    ui->quarantineSelectedButton->setEnabled(hasSelection);
}

void MainWindow::scanDevice(const DeviceInfo& device) {
    ui->resultsTable->setRowCount(0);
    addLogMessage(QString("Starting background scan: %1").arg(QString::fromStdString(device.mountPoint)));
    ui->scanButton->setEnabled(false);
    ui->progressBar->setValue(0);
    
    scanThread_ = new QThread();
    ScanWorker* worker = new ScanWorker(scanner_, device);
    worker->moveToThread(scanThread_);
    
    connect(scanThread_, &QThread::started, worker, &ScanWorker::doScan);
    
    connect(worker, &ScanWorker::progressChanged, this, [this](int value) {
        ui->progressBar->setValue(value);
    });
    
    connect(worker, &ScanWorker::scanFinished, this, [this, device](const std::vector<ScanResult>& results) {
        onScanFinished(device, results);
    });
    
    connect(worker, &ScanWorker::scanFinished, scanThread_, &QThread::quit);
    connect(scanThread_, &QThread::finished, worker, &QObject::deleteLater);
    connect(scanThread_, &QThread::finished, scanThread_, &QObject::deleteLater);
    
    scanThread_->start();
    addLogMessage("Scan running in background thread...");
}

void MainWindow::onScanFinished(const DeviceInfo& device, const std::vector<ScanResult>& results) {
    ui->progressBar->setValue(100);
    
    deviceRepo_->recordDeviceScan(device);
    
    if (results.empty()) {
        addLogMessage("✓ Scan complete: No threats found");
        QMessageBox::information(this, "Scan Complete", "No threats detected!");
    } else {
        addLogMessage(QString("⚠ Scan complete: %1 threats found").arg(results.size()));
        
        // Добавляем результаты в таблицу
        for (const auto& result : results) {
            int row = ui->resultsTable->rowCount();
            ui->resultsTable->insertRow(row);
            ui->resultsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(result.filePath)));
            ui->resultsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(result.virusName)));
            ui->resultsTable->setItem(row, 2, new QTableWidgetItem(QString::number(result.severity)));
            
            deviceRepo_->recordScanResult(device.mountPoint, result);
        }
        
        QString message = QString("Found %1 infected file(s)!\n\n"
                                 "You can now:\n"
                                 "• Select files in the table\n"
                                 "• Click 'Quarantine Selected' to move them to quarantine")
                         .arg(results.size());
        
        QMessageBox::warning(this, "Threats Detected", message);
    }
    
    ui->scanButton->setEnabled(true);
    scanThread_ = nullptr;
}

void MainWindow::addLogMessage(const QString& message) {
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->logText->append(QString("[%1] %2").arg(timestamp, message));
}
