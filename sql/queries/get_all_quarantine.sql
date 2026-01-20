-- Получение всех файлов в карантине
SELECT 
    id,
    file_path,
    original_path,
    virus_name,
    quarantine_date,
    file_hash
FROM quarantine
ORDER BY quarantine_date DESC;
