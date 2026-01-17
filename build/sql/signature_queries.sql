-- name: get_all_signatures
SELECT 
    v.signature_id,
    v.virus_name,
    h.hash_value as signature_hash,
    h.file_offset,
    h.signature_length,
    v.threat_level
FROM virus_signatures v
INNER JOIN signature_hashes h ON v.signature_id = h.signature_id
ORDER BY v.virus_name;

-- name: find_signature_by_hash
SELECT v.virus_name, h.hash_value, h.file_offset, h.signature_length
FROM virus_signatures v
INNER JOIN signature_hashes h ON v.signature_id = h.signature_id
WHERE h.hash_value = ?;

-- name: add_virus_signature
INSERT OR IGNORE INTO virus_signatures (virus_name, threat_level) VALUES (?, ?);

-- name: add_signature_hash
INSERT OR IGNORE INTO signature_hashes (signature_id, hash_value, file_offset, signature_length) VALUES (?, ?, ?, ?);

-- name: get_signature_id_by_name
SELECT signature_id FROM virus_signatures WHERE virus_name = ?;
