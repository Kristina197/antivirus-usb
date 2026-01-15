-- name: insert_or_update_device
INSERT INTO usb_devices (device_name, mount_point) 
VALUES (?, ?)
ON CONFLICT(device_uuid) DO UPDATE SET 
    device_name = excluded.device_name,
    mount_point = excluded.mount_point;

-- name: insert_scan_result
INSERT INTO scan_history (mount_point, scan_status) 
VALUES (?, 'COMPLETED');

-- name: get_all_devices
SELECT device_name, mount_point FROM usb_devices 
ORDER BY first_seen DESC;
