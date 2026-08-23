/**
 * @file blemanager.cpp
 * @brief Implementation of the BLE manager.
 */

#include "blemanager.h"

/**
 * @brief Constructs a new BLEManager.
 *
 * Stores a reference to the application's Logger.
 *
 * @param logger Reference to the application's Logger.
 */
BLEManager::BLEManager(Logger& logger)
    : logger(logger)
{
}

/**
 * @brief Initializes the BLE manager.
 *
 * Prepares the BLEManager for later use without starting the NimBLE stack.
 *
 * Classic Bluetooth controller memory is released because Ch3rryB0mb only
 * uses Bluetooth Low Energy. This makes the unused controller memory
 * available to other ESP32 subsystems such as Wi-Fi.
 *
 * @return true when the BLE manager is ready for use.
 */
bool BLEManager::start()
{
    if (running) return true;

    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

    running = true;
    logger.info("BLEManager started.");

    return true;
}

/**
 * @brief Stops the active BLE scan.
 *
 * Stops BLE advertisement scanning without deinitializing the BLE
 * subsystem. The BLE stack and discovered device cache remain available
 * so scanning can be resumed or another BLE feature can reuse them.
 */
void BLEManager::stopScan()
{
    if (scanner != nullptr && scanner->isScanning())
        scanner->stop();

    logger.info("BLE scan stopped.");
}

/**
 * @brief Completely shuts down the BLE subsystem.
 *
 * Stops any active BLE scan, releases the dynamically allocated
 * device cache and deinitializes NimBLE.
 *
 * Deinitializing NimBLE releases runtime memory used by the BLE stack,
 * allowing other ESP32 subsystems such as Wi-Fi to initialize reliably.
 *
 * After shutdown, BLE can be initialized again by calling startScan().
 */
void BLEManager::shutdown()
{
    if (scanner != nullptr && scanner->isScanning())
        scanner->stop();

    delete[] devices;
    devices = nullptr;
    deviceCount = 0;

    if (initialized)
    {
        logger.info("Deinitializing NimBLE.");

        NimBLEDevice::deinit(true);

        scanner = nullptr;
        initialized = false;

        logger.info("NimBLE deinitialized.");
    }
}

/**
 * @brief Returns whether BLEManager is running.
 */
bool BLEManager::isRunning() const
{
    return running;
}

/**
 * @brief Starts continuous BLE scanning.
 *
 * Scanning runs asynchronously through NimBLE callbacks, allowing the
 * application and UI loops to remain responsive.
 *
 * @return true if scanning started successfully.
 */
bool BLEManager::startScan()
{
    if (!running)
    {
        logger.error("BLEManager is not running.");
        return false;
    }

    if (!initialized)
    {
        logger.info("Initializing NimBLE.");

        NimBLEDevice::init("");

        scanner = NimBLEDevice::getScan();

        if (scanner == nullptr)
        {
            logger.error("Failed to create BLE scanner.");
            return false;
        }

        scanner->setScanCallbacks(this, true);
        scanner->setActiveScan(true);
        scanner->setInterval(SCAN_INTERVAL_MS);
        scanner->setWindow(SCAN_WINDOW_MS);
        scanner->setMaxResults(0);

        initialized = true;

        logger.info("NimBLE initialized.");
    }

    if (devices == nullptr)
    {
        devices = new BLEDeviceInfo[MAX_DEVICES];

        if (devices == nullptr)
        {
            logger.error("Failed to allocate BLE device cache.");
            return false;
        }
    }

    if (scanner->isScanning())
        scanner->stop();

    clearDevices();

    logger.info("Starting BLE scan.");

    if (!scanner->start(0, false, true))
    {
        logger.error("Failed to start BLE scan.");
        return false;
    }

    return true;
}

/**
 * @brief Returns whether BLE scanning is active.
 */
bool BLEManager::isScanning() const
{
    return scanner != nullptr && scanner->isScanning();
}

/**
 * @brief Clears all stored BLE devices.
 */
void BLEManager::clearDevices()
{
    deviceCount = 0;

    if (devices == nullptr)
        return;

    for (uint8_t i = 0; i < MAX_DEVICES; i++)
        devices[i] = BLEDeviceInfo();
}

/**
 * @brief Returns the number of stored BLE devices.
 */
uint8_t BLEManager::getDeviceCount() const
{
    return deviceCount;
}

/**
 * @brief Returns information about a discovered BLE device.
 *
 * @param index Index of the requested device.
 */
const BLEDeviceInfo& BLEManager::getDevice(uint8_t index) const
{
    return devices[index];
}

/**
 * @brief Finds a BLE device by address.
 *
 * @param address BLE address to search for.
 *
 * @return Device index or -1 when not found.
 */
int BLEManager::findDevice(const String& address) const
{
    for (uint8_t i = 0; i < deviceCount; i++)
        if (devices[i].address == address)
            return i;

    return -1;
}

/**
 * @brief Processes a received BLE advertisement.
 *
 * @param advertisedDevice BLE advertisement received by NimBLE.
 */
void BLEManager::onResult(const NimBLEAdvertisedDevice* advertisedDevice)
{
    if (!running || advertisedDevice == nullptr)
        return;

    updateDevice(advertisedDevice);
}

/**
 * @brief Stores or updates a discovered BLE device.
 *
 * Devices are matched using their BLE address. Repeated advertisements
 * update the RSSI and advertisement information instead of creating
 * duplicate entries.
 *
 * @param advertisedDevice BLE advertisement to process.
 */
void BLEManager::updateDevice(const NimBLEAdvertisedDevice* advertisedDevice)
{
    String address = advertisedDevice->getAddress().toString().c_str();

    int index = findDevice(address);

    if (index < 0)
    {
        if (deviceCount >= MAX_DEVICES)
            return;

        index = deviceCount++;
        devices[index].address = address;
    }

    BLEDeviceInfo& device = devices[index];

    device.rssi = advertisedDevice->getRSSI();
    device.lastSeen = millis();

    if (advertisedDevice->haveName())
        device.name = advertisedDevice->getName().c_str();

    device.hasTxPower = advertisedDevice->haveTXPower();

    if (device.hasTxPower)
        device.txPower = advertisedDevice->getTXPower();
}