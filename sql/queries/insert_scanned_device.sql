-- Сохранение результатов сканирования устройства
INSERT INTO scanned_devices (device_name, device_path, mount_point, threats_found, files_scanned)
VALUES (?, ?, ?, ?, ?);
