-- Таблица вирусных сигнатур
CREATE TABLE IF NOT EXISTS virus_signatures (
    signature_id INTEGER PRIMARY KEY AUTOINCREMENT,
    virus_name VARCHAR(255) NOT NULL UNIQUE,
    threat_level INTEGER CHECK(threat_level BETWEEN 1 AND 10) DEFAULT 5,
    description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Таблица хешей сигнатур (один вирус - много сигнатур)
CREATE TABLE IF NOT EXISTS signature_hashes (
    hash_id INTEGER PRIMARY KEY AUTOINCREMENT,
    signature_id INTEGER NOT NULL,
    hash_value VARCHAR(64) NOT NULL,
    hash_algorithm VARCHAR(20) DEFAULT 'MD5',
    file_offset INTEGER DEFAULT 0,
    signature_length INTEGER NOT NULL,
    FOREIGN KEY (signature_id) REFERENCES virus_signatures(signature_id) ON DELETE CASCADE,
    UNIQUE(hash_value, file_offset)
);

-- Таблица устройств
CREATE TABLE IF NOT EXISTS usb_devices (
    device_id INTEGER PRIMARY KEY AUTOINCREMENT,
    device_name VARCHAR(255) NOT NULL,
    device_uuid VARCHAR(255) UNIQUE,
    vendor_id VARCHAR(50),
    product_id VARCHAR(50),
    first_seen TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Таблица сканирований
CREATE TABLE IF NOT EXISTS scan_history (
    scan_id INTEGER PRIMARY KEY AUTOINCREMENT,
    device_id INTEGER,
    mount_point VARCHAR(512) NOT NULL,
    scan_start TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    scan_end TIMESTAMP,
    files_scanned INTEGER DEFAULT 0,
    threats_found INTEGER DEFAULT 0,
    scan_status VARCHAR(20) CHECK(scan_status IN ('IN_PROGRESS', 'COMPLETED', 'FAILED')) DEFAULT 'IN_PROGRESS',
    FOREIGN KEY (device_id) REFERENCES usb_devices(device_id) ON DELETE SET NULL
);

-- Таблица обнаруженных угроз
CREATE TABLE IF NOT EXISTS detected_threats (
    threat_id INTEGER PRIMARY KEY AUTOINCREMENT,
    scan_id INTEGER NOT NULL,
    file_path VARCHAR(1024) NOT NULL,
    virus_name VARCHAR(255) NOT NULL,
    action_taken VARCHAR(50) CHECK(action_taken IN ('QUARANTINED', 'DELETED', 'IGNORED')) DEFAULT 'QUARANTINED',
    quarantine_path VARCHAR(1024),
    detected_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (scan_id) REFERENCES scan_history(scan_id) ON DELETE CASCADE
);

-- Индексы для производительности
CREATE INDEX IF NOT EXISTS idx_hash_value ON signature_hashes(hash_value);
CREATE INDEX IF NOT EXISTS idx_device_uuid ON usb_devices(device_uuid);
CREATE INDEX IF NOT EXISTS idx_scan_device ON scan_history(device_id);
CREATE INDEX IF NOT EXISTS idx_threat_scan ON detected_threats(scan_id);

-- Триггер для обновления updated_at
CREATE TRIGGER IF NOT EXISTS update_signature_timestamp 
AFTER UPDATE ON virus_signatures
BEGIN
    UPDATE virus_signatures SET updated_at = CURRENT_TIMESTAMP 
    WHERE signature_id = NEW.signature_id;
END;
