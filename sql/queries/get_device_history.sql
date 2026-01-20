-- Получение истории сканирования устройства
SELECT 
    device_name,
    mount_point,
    scan_date,
    threats_found,
    files_scanned
FROM scanned_devices
WHERE device_path = ?
ORDER BY scan_date DESC;
