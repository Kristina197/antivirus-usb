#include <QApplication>
#include <QDebug>
#include <QDir>
#include <iostream>
#include <memory>

#include "ui/MainWindow.h"
#include "infrastructure/DatabaseConnection.h"
#include "usecases/VirusScanner.h"
#include "repositories/SignatureRepository.h"
#include "repositories/DeviceRepository.h"
#include "usecases/QuarantineManager.h"
#include "usecases/UsbMonitor.h"

void printUsage() {
    std::cout << "Usage:\n"
              << "  AntivirusUSB                    - Start GUI mode\n"
              << "  AntivirusUSB --auto-scan <path> - Auto-scan mode (for udev)\n"
              << std::endl;
}

void initializeSignatures(std::shared_ptr<SignatureRepository> sigRepo) {
    sigRepo->addSignature("EICAR-Test-File", "44d88612fea8a8f36de82e1278abb02f", 0, 68, 1);
    qDebug() << "Signatures initialized";
}

int autoScanMode(const QString& scanPath) {
    std::cout << "\n=== USB ANTIVIRUS AUTO-SCAN MODE ===" << std::endl;
    std::cout << "Scan target: " << scanPath.toStdString() << std::endl;
    
    DatabaseConnection dbConn;
    QSqlDatabase db = dbConn.getDatabase();
    
    if (!db.isOpen()) {
        std::cerr << "Failed to open database" << std::endl;
        return 1;
    }
    
    auto sigRepo = std::make_shared<SignatureRepository>(db);
    initializeSignatures(sigRepo);
    auto quarantineManager = std::make_shared<QuarantineManager>(
        QDir::homePath().toStdString() + "/antivirus-usb/quarantine"
    );
    
    auto scanner = std::make_shared<VirusScanner>(sigRepo, quarantineManager);
    
    std::cout << "Starting scan..." << std::endl;
    
    int threatsFound = 0;
    std::vector<ScanResult> results;
    scanner->scanDirectory(scanPath.toStdString(), results);
    
    std::cout << "\n=== SCAN RESULTS ===" << std::endl;
    std::cout << "Files scanned: " << results.size() << std::endl;
    
    for (const auto& result : results) {
        if (result.isInfected) {
            std::cout << "\n THREAT DETECTED:" << std::endl;
            std::cout << "   Virus: " << result.virusName << std::endl;
            std::cout << "   File: " << result.filePath << std::endl;
            std::cout << "   [INFO] File not quarantined in auto-scan mode" << std::endl;
            std::cout << "   [INFO] Use GUI to quarantine files manually" << std::endl;
            
            threatsFound++;
        }
    }
    
    std::cout << "\n=== SUMMARY ===" << std::endl;
    std::cout << "Total threats found: " << threatsFound << std::endl;
    
    if (threatsFound > 0) {
        std::cout << "\n WARNING: Infected files detected!" << std::endl;
        std::cout << "Run GUI mode to review and quarantine threats:" << std::endl;
        std::cout << "  ./AntivirusUSB" << std::endl;
    } else {
        std::cout << "No threats detected" << std::endl;
    }
    
    return threatsFound > 0 ? 1 : 0;
}

int main(int argc, char *argv[]) {
    if (argc == 3 && QString(argv[1]) == "--auto-scan") {
        QCoreApplication app(argc, argv);
        QString scanPath = argv[2];
        return autoScanMode(scanPath);
    }
    
    if (argc > 1 && (QString(argv[1]) == "--help" || QString(argv[1]) == "-h")) {
        printUsage();
        return 0;
    }
    
    QApplication app(argc, argv);
    app.setApplicationName("USB Antivirus Scanner");
    app.setOrganizationName("AntivirusUSB");
    
    DatabaseConnection dbConn;
    QSqlDatabase db = dbConn.getDatabase();
    
    if (!db.isOpen()) {
        qCritical() << "Failed to open database";
        return 1;
    }
    
    auto sigRepo = std::make_shared<SignatureRepository>(db);
    initializeSignatures(sigRepo);
    auto deviceRepo = std::make_shared<DeviceRepository>(db);
    auto quarantineManager = std::make_shared<QuarantineManager>(
        QDir::homePath().toStdString() + "/antivirus-usb/quarantine"
    );
    
    auto scanner = std::make_shared<VirusScanner>(sigRepo, quarantineManager);
    auto usbMonitor = std::make_shared<UsbMonitor>();
    
    MainWindow window(scanner, usbMonitor, deviceRepo, quarantineManager);
    window.show();
    usbMonitor->startMonitoring();
    
    return app.exec();
}
