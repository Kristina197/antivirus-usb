-- @name: create_signatures_table
CREATE TABLE IF NOT EXISTS signatures (
    id INTEGER PRIMARY KEY AUTOINCREMENT, virus_name TEXT NOT NULL UNIQUE,
    signature_hash TEXT NOT NULL, file_offset INTEGER NOT NULL DEFAULT 0,
    signature_length INTEGER NOT NULL, severity INTEGER NOT NULL DEFAULT 1,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
-- @name: create_scan_history_table
CREATE TABLE IF NOT EXISTS scan_history (
    id INTEGER PRIMARY KEY AUTOINCREMENT, device_path TEXT NOT NULL,
    file_path TEXT NOT NULL, virus_name TEXT NOT NULL,
    scan_date DATETIME DEFAULT CURRENT_TIMESTAMP
);
-- @name: create_devices_table
CREATE TABLE IF NOT EXISTS devices (
    id INTEGER PRIMARY KEY AUTOINCREMENT, device_name TEXT NOT NULL,
    mount_point TEXT NOT NULL, last_scan DATETIME
);
-- @name: insert_signature
INSERT INTO signatures (virus_name, signature_hash, file_offset, signature_length, severity) VALUES (?, ?, ?, ?, ?);
-- @name: get_all_signatures
SELECT id, virus_name, signature_hash, file_offset, signature_length, severity FROM signatures;
-- @name: get_signature_by_name
SELECT id, virus_name, signature_hash, file_offset, signature_length, severity FROM signatures WHERE virus_name = ?;
-- @name: delete_signature
DELETE FROM signatures WHERE id = ?;
-- @name: insert_scan_result
INSERT INTO scan_history (device_path, file_path, virus_name, scan_date) VALUES (?, ?, ?, datetime('now'));
-- @name: get_scan_history
SELECT id, device_path, file_path, virus_name, scan_date FROM scan_history ORDER BY scan_date DESC LIMIT 100;
-- @name: insert_or_update_device
INSERT INTO devices (device_name, mount_point, last_scan) VALUES (?, ?, datetime('now')) ON CONFLICT(mount_point) DO UPDATE SET last_scan = datetime('now');
-- @name: get_all_devices
SELECT id, device_name, mount_point, last_scan FROM devices;
