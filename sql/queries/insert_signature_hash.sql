-- Добавление хеша для сигнатуры
INSERT OR IGNORE INTO signature_hashes 
(signature_id, hash_value, file_offset, signature_length) 
VALUES (?, ?, ?, ?)
