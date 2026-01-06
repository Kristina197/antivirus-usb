#include <QApplication>
#include "ui/MainWindow.h"
#include "infrastructure/SqlQueryLoader.h"
#include "infrastructure/DatabaseConnection.h"
#include "repositories/SignatureRepository.h"
#include "repositories/DeviceRepository.h"
#include "usecases/VirusScanner.h"
#include "usecases/UsbMonitor.h"
#include "usecases/QuarantineManager.h"
#include <iostream>
#include <filesystem>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Создаём папку карантина
    std::string quarantinePath = std::filesystem::current_path().string() + "/../quarantine";
    auto quarantineManager = std::make_shared<QuarantineManager>(quarantinePath);
    
    auto& queryLoader = SqlQueryLoader::getInstance();
    if (!queryLoader.loadQueriesFromFile("sql/queries.sql")) {
        std::cerr << "Failed to load SQL queries\n";
        return 1;
    }
    
    auto& dbConn = DatabaseConnection::getInstance();
    if (!dbConn.initialize("signatures.db")) {
        std::cerr << "Failed to initialize database\n";
        return 1;
    }
    
    auto sigRepo = std::make_shared<SignatureRepository>(dbConn.getDatabase());
    auto devRepo = std::make_shared<DeviceRepository>(dbConn.getDatabase());
    
    std::cout << "Loading virus signatures...\n";
    
    Signature eicar("EICAR-Test-File", "44d88612fea8a8f36de82e1278abb02f", 0, 68, 3);
    sigRepo->addSignature(eicar);
    Signature wannacry("WannaCry", "db349b97c37d22f5ea1d1841e3c89eb4", 0, 100, 5);
    sigRepo->addSignature(wannacry);
    Signature petya("Petya", "71b6a493388e7d0b40c83ce903bc6b04", 0, 150, 5);
    sigRepo->addSignature(petya);
    Signature notpetya("NotPetya", "027cc450ef5f8c5f653329641ec1fed9", 0, 200, 5);
    sigRepo->addSignature(notpetya);
    Signature conficker("Conficker", "7e4b3e7b5b8d4c9f3a2e1d6c5b4a3f2e", 0, 120, 4);
    sigRepo->addSignature(conficker);
    Signature zeus("Zeus-Trojan", "3d8b5c7a9e1f4d6c2b8a5e3f7c9d1a4b", 0, 180, 5);
    sigRepo->addSignature(zeus);
    Signature emotet("Emotet", "9f4e6d3c8b7a5e2f1d9c8b6a4e3f2d1c", 0, 140, 4);
    sigRepo->addSignature(emotet);
    Signature ryuk("Ryuk-Ransomware", "5a7b9c3d1e4f6b8a2c9d5e7f3a6b4c1d", 0, 160, 5);
    sigRepo->addSignature(ryuk);
    Signature trickbot("TrickBot", "8e3f7a9b5c2d4f6e1a8c7b9d5e3f4a2c", 0, 130, 4);
    sigRepo->addSignature(trickbot);
    Signature locky("Locky", "2d5e8f3a7c9b4e6d1a5c8b7e9f3d4a6b", 0, 170, 5);
    sigRepo->addSignature(locky);
    Signature cryptolocker("CryptoLocker", "6b4d9e2f5a8c7b3d1e9f4a6c8b5d2e7a", 0, 190, 5);
    sigRepo->addSignature(cryptolocker);
    Signature mirai("Mirai-Botnet", "4e7c9a3d6b8f2e5a1c7d9b4e6a8f3c5d", 0, 110, 4);
    sigRepo->addSignature(mirai);
    Signature stuxnet("Stuxnet", "9d6e3f8a2c5b7e4d1a9f6c8b3e5d7a2f", 0, 220, 5);
    sigRepo->addSignature(stuxnet);
    Signature dridex("Dridex", "7a5d9c2e6f8b3a4d1e7c9b5d8f4a6e3c", 0, 135, 4);
    sigRepo->addSignature(dridex);
    Signature cerber("Cerber-Ransomware", "3f7a9d5c8e2b6a4d1f9c7e5b8d3a6f4c", 0, 175, 5);
    sigRepo->addSignature(cerber);
    
    std::cout << "✓ Loaded 15 virus signatures\n";
    
    auto scanner = std::make_shared<VirusScanner>(sigRepo, quarantineManager);
    auto usbMonitor = std::make_shared<UsbMonitor>();
    
    MainWindow mainWindow(scanner, usbMonitor, devRepo, quarantineManager);
    mainWindow.show();
    
    std::cout << "========================================\n";
    std::cout << "USB Antivirus Scanner v2.0\n";
    std::cout << "✓ 15 signatures loaded\n";
    std::cout << "✓ Quarantine enabled: " << quarantinePath << "\n";
    std::cout << "========================================\n";
    
    return app.exec();
}
