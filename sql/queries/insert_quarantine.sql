-- Добавление файла в карантин
INSERT INTO quarantine (file_path, original_path, virus_name, file_hash)
VALUES (?, ?, ?, ?);
