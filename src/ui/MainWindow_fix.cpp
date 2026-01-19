// Находим строку onDeviceDisconnected и заменяем логику
void MainWindow::onDeviceDisconnected(const DeviceInfo& device) {
    qDebug() << "Device disconnected:" << QString::fromStdString(device.deviceName);
    
    // ИСПРАВЛЕНО: Ищем по mountPoint или devicePath, а не только по deviceName
    for (int row = 0; row < ui->deviceTable->rowCount(); ++row) {
        QTableWidgetItem* mountItem = ui->deviceTable->item(row, 1);  // Колонка "Путь монтирования"
        
        if (mountItem) {
            std::string tableMountPoint = mountItem->text().toStdString();
            
            // Проверяем совпадение по пути монтирования или devicePath
            if (!device.mountPoint.empty() && tableMountPoint == device.mountPoint) {
                qDebug() << "Removing device by mountPoint:" << QString::fromStdString(tableMountPoint);
                ui->deviceTable->removeRow(row);
                
                // Удаляем из вектора connectedDevices_
                if (row < connectedDevices_.size()) {
                    connectedDevices_.erase(connectedDevices_.begin() + row);
                }
                break;
            }
            
            // Дополнительно проверяем по devicePath
            if (!device.devicePath.empty() && tableMountPoint.find(device.devicePath) != std::string::npos) {
                qDebug() << "Removing device by devicePath:" << QString::fromStdString(device.devicePath);
                ui->deviceTable->removeRow(row);
                
                if (row < connectedDevices_.size()) {
                    connectedDevices_.erase(connectedDevices_.begin() + row);
                }
                break;
            }
        }
    }
    
    ui->statusLabel->setText(QString("USB устройство отключено: %1").arg(QString::fromStdString(device.deviceName)));
}
