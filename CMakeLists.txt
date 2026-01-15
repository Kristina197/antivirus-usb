cmake_minimum_required(VERSION 3.16)
project(AntivirusUSB VERSION 2.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)
find_package(PkgConfig REQUIRED)
pkg_check_modules(UDEV REQUIRED libudev)

# Файл ресурсов
set(RESOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/resources.qrc)
file(WRITE ${RESOURCE_FILE} 
"<RCC>
    <qresource prefix=\"/\">
        <file>resources/shield-clean.jpg</file>
        <file>resources/shield-infected.jpg</file>
        <file></file>
    </qresource>
</RCC>")

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/entities/Signature.h 
    src/entities/ScanResult.h 
    src/entities/DeviceInfo.h
    src/entities/ScanConfig.h
    src/entities/ScanConfig.cpp
    src/usecases/IVirusScanner.h 
    src/usecases/VirusScanner.cpp 
    src/usecases/VirusScanner.h
    src/usecases/IUsbMonitor.h 
    src/usecases/UsbMonitor.cpp 
    src/usecases/UsbMonitor.h
    src/usecases/QuarantineManager.cpp
    src/usecases/QuarantineManager.h
    src/interfaces/ISignatureRepository.h 
    src/interfaces/IDeviceRepository.h
    src/repositories/SignatureRepository.cpp 
    src/repositories/SignatureRepository.h
    src/repositories/DeviceRepository.cpp 
    src/repositories/DeviceRepository.h
    src/infrastructure/SqlQueryLoader.cpp 
    src/infrastructure/SqlQueryLoader.h
    src/infrastructure/DatabaseConnection.cpp 
    src/infrastructure/DatabaseConnection.h
    src/ui/MainWindow.cpp 
    src/ui/MainWindow.h 
    src/ui/MainWindow.ui
    src/ui/ScanWorker.h
    src/ui/RestoreWorker.h
    src/ui/QuarantineDialog.cpp
    src/ui/QuarantineDialog.h
    src/ui/SettingsDialog.cpp
    src/ui/SettingsDialog.h
    src/ui/ResultDialog.h
    ${RESOURCE_FILE}
)

target_link_libraries(${PROJECT_NAME} 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Sql 
    ${UDEV_LIBRARIES} 
    crypto 
    pthread
)

target_include_directories(${PROJECT_NAME} PRIVATE ${UDEV_INCLUDE_DIRS})
file(COPY ${CMAKE_SOURCE_DIR}/sql DESTINATION ${CMAKE_BINARY_DIR})
