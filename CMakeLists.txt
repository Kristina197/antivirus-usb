cmake_minimum_required(VERSION 3.16)
project(AntivirusUSB VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)
find_package(PkgConfig REQUIRED)
pkg_check_modules(UDEV REQUIRED libudev)
add_executable(${PROJECT_NAME}
    src/main.cpp src/entities/Signature.h src/entities/ScanResult.h src/entities/DeviceInfo.h
    src/usecases/IVirusScanner.h src/usecases/VirusScanner.cpp src/usecases/VirusScanner.h
    src/usecases/IUsbMonitor.h src/usecases/UsbMonitor.cpp src/usecases/UsbMonitor.h
    src/interfaces/ISignatureRepository.h src/interfaces/IDeviceRepository.h
    src/repositories/SignatureRepository.cpp src/repositories/SignatureRepository.h
    src/repositories/DeviceRepository.cpp src/repositories/DeviceRepository.h
    src/infrastructure/SqlQueryLoader.cpp src/infrastructure/SqlQueryLoader.h
    src/infrastructure/DatabaseConnection.cpp src/infrastructure/DatabaseConnection.h
    src/ui/MainWindow.cpp src/ui/MainWindow.h src/ui/MainWindow.ui
)
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets Qt6::Sql ${UDEV_LIBRARIES} crypto)
target_include_directories(${PROJECT_NAME} PRIVATE ${UDEV_INCLUDE_DIRS})
file(COPY ${CMAKE_SOURCE_DIR}/sql DESTINATION ${CMAKE_BINARY_DIR})
