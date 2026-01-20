-- Основные таблицы для вирусных сигнатур
CREATE TABLE IF NOT EXISTS virus_signatures (
    signature_id INTEGER PRIMARY KEY AUTOINCREMENT,
    virus_name TEXT NOT NULL UNIQUE,
    threat_level TEXT DEFAULT 'medium',
    description TEXT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS signature_hashes (
    hash_id INTEGER PRIMARY KEY AUTOINCREMENT,
    signature_id INTEGER NOT NULL,
    hash_value TEXT NOT NULL,
    file_offset INTEGER DEFAULT 0,
    signature_length INTEGER,
    FOREIGN KEY (signature_id) REFERENCES virus_signatures(signature_id)
);

-- Таблица для истории сканирования
CREATE TABLE IF NOT EXISTS scanned_devices (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    device_name TEXT NOT NULL,
    device_path TEXT NOT NULL,
    mount_point TEXT,
    scan_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    threats_found INTEGER DEFAULT 0,
    files_scanned INTEGER DEFAULT 0
);

-- Таблица карантина
CREATE TABLE IF NOT EXISTS quarantine (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    file_path TEXT NOT NULL,
    original_path TEXT NOT NULL,
    virus_name TEXT NOT NULL,
    quarantine_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    file_hash TEXT
);

-- Индексы для оптимизации
CREATE INDEX IF NOT EXISTS idx_virus_name ON virus_signatures(virus_name);
CREATE INDEX IF NOT EXISTS idx_hash_value ON signature_hashes(hash_value);
CREATE INDEX IF NOT EXISTS idx_scan_date ON scanned_devices(scan_date);
CREATE INDEX IF NOT EXISTS idx_quarantine_date ON quarantine(quarantine_date);
