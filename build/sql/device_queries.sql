-- name: insert_or_update_device
INSERT INTO usb_devices (device_name, device_uuid) 
VALUES (?, ?) 
ON CONFLICT(device_uuid) DO UPDATE SET device_name = excluded.device_name;

-- name: get_all_devices
SELECT device_id, device_name, device_uuid, vendor_id, product_id, first_seen 
FROM usb_devices;

-- name: insert_scan_result
INSERT INTO scan_history (device_id, mount_point, scan_start, scan_end, files_scanned, threats_found, scan_status)
VALUES (?, ?, ?, ?, ?, ?, ?);
