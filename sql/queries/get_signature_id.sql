-- Получение ID сигнатуры по имени
SELECT signature_id 
FROM virus_signatures 
WHERE virus_name = ?
