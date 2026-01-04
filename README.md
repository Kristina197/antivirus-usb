# USB Antivirus Scanner

Antivirus for USB devices with signature-based scanning.

## Installation (Ubuntu)

Install dependencies:
sudo apt install build-essential cmake qt6-base-dev libqt6sql6-sqlite libudev-dev libssl-dev

## Build

mkdir build && cd build
cmake ..
make

## Run

./AntivirusUSB
