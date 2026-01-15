-- @name: GetAllSignatures
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

-- @name: FindSignatureByHash
SELECT 
    v.virus_name,
    h.hash_value,
    h.file_offset,
    h.signature_length
FROM virus_signatures v
INNER JOIN signature_hashes h ON v.signature_id = h.signature_id
WHERE h.hash_value = ?;

-- @name: AddSignature
INSERT INTO virus_signatures (virus_name, threat_level, description)
VALUES (?, ?, ?);

-- @name: AddSignatureHash
INSERT INTO signature_hashes (signature_id, hash_value, file_offset, signature_length)
VALUES (?, ?, ?, ?);

-- @name: DeleteSignature
DELETE FROM virus_signatures WHERE signature_id = ?;
