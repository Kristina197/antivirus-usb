#!/bin/bash

# Логирование
LOG_FILE="/var/log/antivirus_usb_autoscan.log"
ANTIVIRUS_PATH="/home/kristina/antivirus-usb/build/AntivirusUSB"
QUARANTINE_PATH="/home/kristina/antivirus-usb/quarantine"
DB_PATH="/home/kristina/antivirus-usb/build/antivirus.db"

# Получаем точку монтирования USB
USB_MOUNT_POINT=$(findmnt -rno TARGET /dev/$DEVNAME 2>/dev/null)

if [ -z "$USB_MOUNT_POINT" ]; then
    # Ждём монтирования (до 5 секунд)
    for i in {1..10}; do
        sleep 0.5
        USB_MOUNT_POINT=$(findmnt -rno TARGET /dev/$DEVNAME 2>/dev/null)
        [ -n "$USB_MOUNT_POINT" ] && break
    done
fi

# Логируем событие
echo "==================================================" >> $LOG_FILE
echo "$(date '+%Y-%m-%d %H:%M:%S') - USB Device Inserted" >> $LOG_FILE
echo "Device: $DEVNAME" >> $LOG_FILE
echo "Mount point: $USB_MOUNT_POINT" >> $LOG_FILE

if [ -z "$USB_MOUNT_POINT" ]; then
    echo "ERROR: Device not mounted" >> $LOG_FILE
    exit 1
fi

# Проверяем что это директория и доступна
if [ ! -d "$USB_MOUNT_POINT" ]; then
    echo "ERROR: Mount point is not a directory" >> $LOG_FILE
    exit 1
fi

echo "Starting automatic scan..." >> $LOG_FILE

# Запускаем сканирование через CLI (создадим позже)
# Временно используем прямое сканирование
export DISPLAY=:0
export XAUTHORITY=/home/kristina/.Xauthority

# Отправляем уведомление пользователю
sudo -u kristina DISPLAY=:0 DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus \
    notify-send "USB Antivirus" "Scanning USB device: $USB_MOUNT_POINT" -i dialog-information

# Запускаем приложение в режиме автосканирования
sudo -u kristina DISPLAY=:0 $ANTIVIRUS_PATH --auto-scan "$USB_MOUNT_POINT" >> $LOG_FILE 2>&1

SCAN_RESULT=$?

if [ $SCAN_RESULT -eq 0 ]; then
    echo "✓ Scan completed successfully" >> $LOG_FILE
    sudo -u kristina DISPLAY=:0 DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus \
        notify-send "USB Antivirus" "Scan completed. No threats found." -i security-high
else
    echo "✗ Threats detected!" >> $LOG_FILE
    sudo -u kristina DISPLAY=:0 DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus \
        notify-send "USB Antivirus" "⚠️ THREATS DETECTED! Check quarantine." -u critical -i dialog-error
fi

echo "==================================================" >> $LOG_FILE
