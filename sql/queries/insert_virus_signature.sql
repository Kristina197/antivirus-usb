-- Добавление новой сигнатуры вируса
INSERT OR IGNORE INTO virus_signatures (virus_name, threat_level) 
VALUES (?, ?)
