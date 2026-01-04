#include <QApplication>
#include "ui/MainWindow.h"
#include "infrastructure/SqlQueryLoader.h"
#include "infrastructure/DatabaseConnection.h"
#include "repositories/SignatureRepository.h"
#include "repositories/DeviceRepository.h"
#include "usecases/VirusScanner.h"
#include "usecases/UsbMonitor.h"
#include <iostream>
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    auto& queryLoader = SqlQueryLoader::getInstance();
    if (!queryLoader.loadQueriesFromFile("sql/queries.sql")) { std::cerr << "Failed to load SQL queries\n"; return 1; }
    auto& dbConn = DatabaseConnection::getInstance();
    if (!dbConn.initialize("signatures.db")) { std::cerr << "Failed to initialize database\n"; return 1; }
    auto sigRepo = std::make_shared<SignatureRepository>(dbConn.getDatabase());
    auto devRepo = std::make_shared<DeviceRepository>(dbConn.getDatabase());
    Signature eicar("EICAR-Test-File", "44d88612fea8a8f36de82e1278abb02f", 0, 68, 3);
    sigRepo->addSignature(eicar);
    auto scanner = std::make_shared<VirusScanner>(sigRepo);
    auto usbMonitor = std::make_shared<UsbMonitor>();
    MainWindow mainWindow(scanner, usbMonitor, devRepo);
    mainWindow.show();
    return app.exec();
}
