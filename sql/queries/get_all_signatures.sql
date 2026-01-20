-- Получение всех активных сигнатур с хешами
SELECT 
    vs.virus_name,
    sh.hash_value,
    sh.file_offset,
    sh.signature_length
FROM virus_signatures vs
INNER JOIN signature_hashes sh ON vs.signature_id = sh.signature_id
ORDER BY vs.virus_name
