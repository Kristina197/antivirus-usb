#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "ScanWorker.h"
#include "QuarantineDialog.h"
#include "SettingsDialog.h"
#include "ResultDialog.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(std::shared_ptr<VirusScanner> scanner,
                       std::shared_ptr<IUsbMonitor> usbMonitor,
                       std::shared_ptr<IDeviceRepository> deviceRepo,
                       std::shared_ptr<QuarantineManager> quarantineManager,
                       QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scanner_(scanner)
    , usbMonitor_(usbMonitor)
    , deviceRepo_(deviceRepo)
    , quarantineManager_(quarantineManager)
    , scanThread_(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("USB Антивирус");
    ui->scanButton->setText("Сканировать выбранное устройство");
    ui->settingsButton->setText("Настройки");
    ui->quarantineButton->setText("Карантин");

    setupConnections();
    setupDeviceTable();
    setupResultsTable();

    ui->statusLabel->setText("Готов. Ожидание USB устройств...");
    ui->progressBar->setVisible(false);

    qDebug() << "MainWindow initialized";
}

MainWindow::~MainWindow() {
    if (scanThread_ && scanThread_->isRunning()) {
        scanThread_->quit();
        scanThread_->wait();
    }
    delete ui;
}

void MainWindow::setupConnections() {
    connect(usbMonitor_.get(), &IUsbMonitor::deviceConnected,
            this, &MainWindow::onDeviceConnected, Qt::QueuedConnection);
    connect(usbMonitor_.get(), &IUsbMonitor::deviceDisconnected,
            this, &MainWindow::onDeviceDisconnected, Qt::QueuedConnection);
    connect(ui->scanButton, &QPushButton::clicked,
            this, &MainWindow::onScanButtonClicked);
    connect(ui->quarantineButton, &QPushButton::clicked,
            this, &MainWindow::onQuarantineButtonClicked);
    connect(ui->settingsButton, &QPushButton::clicked,
            this, &MainWindow::onSettingsButtonClicked);

    usbMonitor_->startMonitoring();
}

void MainWindow::setupDeviceTable() {
    ui->deviceTable->setColumnCount(4);
    ui->deviceTable->setHorizontalHeaderLabels({"Устройство", "Путь монтирования", "Статус", "Последнее сканирование"});
    ui->deviceTable->horizontalHeader()->setStretchLastSection(true);
    ui->deviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->deviceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->deviceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::setupResultsTable() {
    ui->resultsTable->setColumnCount(3);
    ui->resultsTable->setHorizontalHeaderLabels({"Путь к файлу", "Статус", "Название вируса"});
    ui->resultsTable->horizontalHeader()->setStretchLastSection(true);
    ui->resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::onDeviceConnected(const DeviceInfo& device) {
    qDebug() << "Device connected:" << QString::fromStdString(device.deviceName);

    // Проверка на дубликаты
    for (int row = 0; row < ui->deviceTable->rowCount(); ++row) {
        QTableWidgetItem* mountItem = ui->deviceTable->item(row, 1);
        if (mountItem && mountItem->text().toStdString() == device.mountPoint) {
            qDebug() << "Device already in table, skipping";
            return;
        }
    }
    

    int row = ui->deviceTable->rowCount();
    ui->deviceTable->insertRow(row);
    ui->deviceTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(device.deviceName)));
    ui->deviceTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(device.mountPoint)));
    ui->deviceTable->setItem(row, 2, new QTableWidgetItem("Подключено"));
    ui->deviceTable->setItem(row, 3, new QTableWidgetItem("Не сканировалось"));

    connectedDevices_.push_back(device);

    ui->statusLabel->setText(QString("USB устройство подключено: %1").arg(QString::fromStdString(device.deviceName)));

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("USB устройство обнаружено");
    msgBox.setText(QString("USB устройство '%1' было подключено.\nХотите просканировать его сейчас?")
        .arg(QString::fromStdString(device.deviceName)));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.button(QMessageBox::Yes)->setText("Да");
    msgBox.button(QMessageBox::No)->setText("Нет");
    msgBox.setDefaultButton(QMessageBox::Yes);
    
    int reply = msgBox.exec();

    if (reply == QMessageBox::Yes) {
        scanDevice(device);
    }
}

void MainWindow::onDeviceDisconnected(const DeviceInfo& device) {
    qDebug() << "Device disconnected:" << QString::fromStdString(device.devicePath);
    
    // Ищем устройство в векторе connectedDevices_ по devicePath
    int foundRow = -1;
    for (size_t i = 0; i < connectedDevices_.size(); ++i) {
        // Проверяем совпадение по devicePath (например /dev/sdb)
        if (connectedDevices_[i].devicePath == device.devicePath ||
            connectedDevices_[i].devicePath.find(device.devicePath) != std::string::npos ||
            device.devicePath.find(connectedDevices_[i].devicePath) != std::string::npos) {
            foundRow = i;
            qDebug() << "Found device in vector at position" << i;
            break;
        }
    }
    
    if (foundRow >= 0 && foundRow < ui->deviceTable->rowCount()) {
        qDebug() << "Removing device from table at row" << foundRow;
        ui->deviceTable->removeRow(foundRow);
        connectedDevices_.erase(connectedDevices_.begin() + foundRow);
    } else {
        qDebug() << "Device not found in table";
    }
    
    ui->statusLabel->setText(QString("USB устройство отключено: %1").arg(QString::fromStdString(device.devicePath)));
}


void MainWindow::onScanButtonClicked() {
    int currentRow = ui->deviceTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Не выбрано устройство", "Пожалуйста, выберите устройство для сканирования.");
        return;
    }

    if (currentRow >= connectedDevices_.size()) return;

    DeviceInfo device = connectedDevices_[currentRow];
    scanDevice(device);
}

void MainWindow::onQuarantineButtonClicked() {
    QuarantineDialog dialog(quarantineManager_, this);
    dialog.exec();
}

void MainWindow::onSettingsButtonClicked() {
    ScanConfig config = scanner_->getScanConfig();
    SettingsDialog dialog(config, this);
    if (dialog.exec() == QDialog::Accepted) {
        ui->statusLabel->setText("Настройки сканирования обновлены");
    }
}

void MainWindow::scanDevice(const DeviceInfo& device) {
    if (scanThread_ && scanThread_->isRunning()) {
        QMessageBox::warning(this, "Сканирование выполняется",
                           "Сканирование уже запущено. Пожалуйста, дождитесь завершения.");
        return;
    }

    currentScannedDevice_ = device;

    ui->resultsTable->setRowCount(0);
    ui->progressBar->setVisible(true);
    ui->progressBar->setRange(0, 0);
    ui->scanButton->setEnabled(false);
    ui->statusLabel->setText(QString("Сканирование %1...").arg(QString::fromStdString(device.deviceName)));

    scanThread_ = new QThread;
    ScanWorker* worker = new ScanWorker(scanner_, device);
    worker->moveToThread(scanThread_);

    connect(scanThread_, &QThread::started, worker, &ScanWorker::doScan);
    connect(worker, &ScanWorker::scanFinished, this,
            [this, device](const std::vector<ScanResult>& results, int totalFiles) {
                onScanFinished(device, results, totalFiles);
            });
    connect(worker, &ScanWorker::scanFinished, scanThread_, &QThread::quit);
    connect(scanThread_, &QThread::finished, worker, &QObject::deleteLater);
    connect(scanThread_, &QThread::finished, scanThread_, &QObject::deleteLater);

    scanThread_->start();
}

void MainWindow::onScanFinished(const DeviceInfo& device, const std::vector<ScanResult>& results, int totalFiles) {
    ui->progressBar->setVisible(false);
    ui->scanButton->setEnabled(true);

    int threatsFound = 0;
    std::vector<ScanResult> infectedFiles;

    for (const auto& result : results) {
        if (result.isInfected) {
            infectedFiles.push_back(result);
            threatsFound++;

            int row = ui->resultsTable->rowCount();
            ui->resultsTable->insertRow(row);
            ui->resultsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(result.filePath)));
            ui->resultsTable->setItem(row, 1, new QTableWidgetItem("Заражен"));
            ui->resultsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(result.virusName)));
        }
    }

    for (int row = 0; row < ui->deviceTable->rowCount(); ++row) {
        QTableWidgetItem* deviceItem = ui->deviceTable->item(row, 0);
        if (deviceItem && deviceItem->text().toStdString() == device.deviceName) {
            QString status = threatsFound > 0 ?
                QString("%1 угроз").arg(threatsFound) : "Чисто";
            ui->deviceTable->setItem(row, 2, new QTableWidgetItem(status));
            ui->deviceTable->setItem(row, 3, new QTableWidgetItem(
                QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
            break;
        }
    }

    if (threatsFound > 0) {
        ui->statusLabel->setText(QString("Сканирование завершено: найдено %1 угроз!").arg(threatsFound));

        ResultDialog::showInfectedResult(this, threatsFound, totalFiles);

        for (const auto& result : infectedFiles) {
            handleInfectedFile(QString::fromStdString(result.filePath),
                             QString::fromStdString(result.virusName));
        }
    } else {
        ui->statusLabel->setText("Сканирование завершено: угроз не найдено");
        ResultDialog::showCleanResult(this, totalFiles);
    }

    scanThread_ = nullptr;
}

void MainWindow::showThreatSummary(int threatsFound, int totalFiles) {
}

void MainWindow::handleInfectedFile(const QString& filePath, const QString& virusName) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Угроза обнаружена");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(QString("Обнаружен вирус: %1").arg(virusName));
    msgBox.setInformativeText(QString("Файл: %1\n\nЧто сделать с этим файлом?").arg(filePath));

    QPushButton* quarantineBtn = msgBox.addButton("Поместить в карантин", QMessageBox::AcceptRole);
    QPushButton* ignoreBtn = msgBox.addButton("Игнорировать", QMessageBox::RejectRole);
    msgBox.setDefaultButton(quarantineBtn);

    msgBox.exec();

    if (msgBox.clickedButton() == quarantineBtn) {
        if (quarantineManager_->quarantineFile(filePath.toStdString(), virusName.toStdString())) {
            int remainingThreats = 0;
            for (int i = 0; i < ui->resultsTable->rowCount(); ++i) {
                QTableWidgetItem* fileItem = ui->resultsTable->item(i, 0);
                if (fileItem && fileItem->text() != filePath) {
                    remainingThreats++;
                }
            }

            updateDeviceStatus(QString::fromStdString(currentScannedDevice_.deviceName), remainingThreats);

            for (int i = 0; i < ui->resultsTable->rowCount(); ++i) {
                QTableWidgetItem* item = ui->resultsTable->item(i, 0);
                if (item && item->text() == filePath) {
                    ui->resultsTable->removeRow(i);
                    break;
                }
            }

            QMessageBox::information(this, "Успешно",
                QString("Файл %1 помещен в карантин").arg(QFileInfo(filePath).fileName()));
        } else {
            QMessageBox::critical(this, "Ошибка",
                "Не удалось поместить файл в карантин");
        }
    } else {
        qDebug() << "User chose to ignore infected file:" << filePath;
        QMessageBox::warning(this, "Предупреждение",
            "Файл остался на диске и может быть опасен!");
    }
}

void MainWindow::updateDeviceStatus(const QString& deviceName, int threatsCount) {
    for (int row = 0; row < ui->deviceTable->rowCount(); ++row) {
        QTableWidgetItem* deviceItem = ui->deviceTable->item(row, 0);
        if (deviceItem && deviceItem->text() == deviceName) {
            QString status = threatsCount > 0 ?
                QString("%1 угроз").arg(threatsCount) : "Чисто";
            ui->deviceTable->setItem(row, 2, new QTableWidgetItem(status));
            break;
        }
    }
}
