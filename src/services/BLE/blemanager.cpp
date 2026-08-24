/**
 * @file blemanager.cpp
 * @brief Implementation of the BLE manager.
 */

#include "blemanager.h"

/**
 * @brief Constructs a new BLEManager.
 *
 * @param logger Reference to the application's Logger.
 */
BLEManager::BLEManager(Logger& logger)
    : logger(logger)
{
}

/**
 * @brief Starts the BLE manager.
 *
 * @return true when the manager is ready.
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
 * @brief Stops the BLE manager.
 *
 * @return true when the manager has stopped.
 */
bool BLEManager::stop()
{
    if (!running) return true;

    shutdown();

    running = false;

    logger.info("BLEManager stopped.");

    return true;
}

/**
 * @brief Returns whether the BLE manager is running.
 *
 * @return true when the manager is available.
 */
bool BLEManager::isRunning() const
{
    return running;
}

/**
 * @brief Initializes NimBLE when required.
 *
 * @return true when NimBLE is available.
 */
bool BLEManager::ensureInitialized()
{
    if (initialized) return true;

    if (!running) {
        logger.error("BLEManager is not running.");
        return false;
    }

    logger.info("Initializing NimBLE.");

    NimBLEDevice::init("");

    initialized = true;

    logger.info("NimBLE initialized.");

    return true;
}

/**
 * @brief Starts continuous BLE scanning.
 *
 * @return true when scanning started successfully.
 */
bool BLEManager::startScan()
{
    if (!running) {
        logger.error("BLEManager is not running.");
        return false;
    }

    if (!ensureInitialized()) return false;

    stopAdvertising();

    if (scanner == nullptr) {
        scanner = NimBLEDevice::getScan();

        if (scanner == nullptr) {
            logger.error("Failed to create BLE scanner.");
            return false;
        }

        scanner->setScanCallbacks(this, true);
        scanner->setActiveScan(true);
        scanner->setInterval(SCAN_INTERVAL_MS);
        scanner->setWindow(SCAN_WINDOW_MS);
        scanner->setMaxResults(0);
    }

    if (devices == nullptr) {
        devices = new BLEDeviceInfo[MAX_DEVICES];

        if (devices == nullptr) {
            logger.error("Failed to allocate BLE device cache.");
            return false;
        }
    }

    if (scanner->isScanning()) scanner->stop();

    clearDevices();

    logger.info("Starting BLE scan.");

    if (!scanner->start(0, false, true)) {
        logger.error("Failed to start BLE scan.");
        return false;
    }

    return true;
}

/**
 * @brief Stops the active BLE scan.
 */
void BLEManager::stopScan()
{
    if (scanner == nullptr || !scanner->isScanning()) return;

    scanner->stop();

    logger.info("BLE scan stopped.");
}

/**
 * @brief Returns whether BLE scanning is active.
 *
 * @return true when scanning.
 */
bool BLEManager::isScanning() const
{
    return scanner != nullptr && scanner->isScanning();
}

/**
 * @brief Starts BLE advertising.
 *
 * @param name Device name included in the advertisement.
 * @param serviceUUID Optional service UUID to advertise.
 *
 * @return true when advertising started successfully.
 */
bool BLEManager::startAdvertising(const String& name, const String& serviceUUID)
{
    if (!running) {
        logger.error("BLEManager is not running.");
        return false;
    }

    if (!ensureInitialized()) return false;

    stopScan();

    advertiser = NimBLEDevice::getAdvertising();

    if (advertiser == nullptr) {
        logger.error("Failed to create BLE advertiser.");
        return false;
    }

    if (advertiser->isAdvertising()) advertiser->stop();

    advertiser->clearData();

    if (!name.isEmpty() && !advertiser->setName(name.c_str())) {
        logger.error("Failed to set BLE advertisement name.");
        return false;
    }

    if (!serviceUUID.isEmpty() && !advertiser->addServiceUUID(serviceUUID.c_str())) {
        logger.error("Failed to add BLE service UUID.");
        return false;
    }

    logger.info(String("Starting BLE advertising as ") + name + ".");

    if (!advertiser->start()) {
        logger.error("Failed to start BLE advertising.");
        return false;
    }

    return true;
}

/**
 * @brief Stops BLE advertising.
 */
void BLEManager::stopAdvertising()
{
    if (advertiser == nullptr || !advertiser->isAdvertising()) return;

    advertiser->stop();

    logger.info("BLE advertising stopped.");
}

/**
 * @brief Returns whether BLE advertising is active.
 *
 * @return true when advertising.
 */
bool BLEManager::isAdvertising() const
{
    return advertiser != nullptr && advertiser->isAdvertising();
}

/**
 * @brief Completely shuts down the BLE subsystem.
 */
void BLEManager::shutdown()
{
    stopScan();
    stopAdvertising();

    delete[] devices;
    devices = nullptr;
    deviceCount = 0;

    if (!initialized) return;

    logger.info("Deinitializing NimBLE.");

    NimBLEDevice::deinit(true);

    scanner = nullptr;
    advertiser = nullptr;
    initialized = false;

    logger.info("NimBLE deinitialized.");
}

/**
 * @brief Clears all stored BLE devices.
 */
void BLEManager::clearDevices()
{
    deviceCount = 0;

    if (devices == nullptr) return;

    for (uint8_t i = 0; i < MAX_DEVICES; i++) devices[i] = BLEDeviceInfo();
}

/**
 * @brief Returns the number of stored BLE devices.
 *
 * @return Number of stored devices.
 */
uint8_t BLEManager::getDeviceCount() const
{
    return deviceCount;
}

/**
 * @brief Returns information about a discovered BLE device.
 *
 * @param index Index of the requested device.
 *
 * @return Constant reference to the BLE device information.
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
        if (devices[i].address == address) return i;

    return -1;
}

/**
 * @brief Processes a received BLE advertisement.
 *
 * @param advertisedDevice BLE advertisement received by NimBLE.
 */
void BLEManager::onResult(const NimBLEAdvertisedDevice* advertisedDevice)
{
    if (!running || advertisedDevice == nullptr) return;

    updateDevice(advertisedDevice);
}

/**
 * @brief Stores or updates a discovered BLE device.
 *
 * @param advertisedDevice BLE advertisement to process.
 */
void BLEManager::updateDevice(const NimBLEAdvertisedDevice* advertisedDevice)
{
    String address = advertisedDevice->getAddress().toString().c_str();

    int existingIndex = findDevice(address);

    BLEDeviceInfo* device = nullptr;

    if (existingIndex >= 0) {
        device = &devices[existingIndex];
    } else {
        if (deviceCount >= MAX_DEVICES) return;

        device = &devices[deviceCount++];
        device->address = address;
    }

    device->serviceUUID = "";

    if (advertisedDevice->getServiceUUIDCount() > 0)
        device->serviceUUID = advertisedDevice->getServiceUUID(0).toString().c_str();

    if (advertisedDevice->haveName()) device->name = advertisedDevice->getName().c_str();

    device->rssi = advertisedDevice->getRSSI();
    device->lastSeen = millis();

    if (advertisedDevice->haveTXPower()) {
        device->txPower = advertisedDevice->getTXPower();
        device->hasTxPower = true;
    } else {
        device->txPower = 0;
        device->hasTxPower = false;
    }
}