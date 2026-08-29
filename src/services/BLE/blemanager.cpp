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

    static bool classicMemoryReleased = false;

    if (!classicMemoryReleased) {
        esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
        classicMemoryReleased = true;
    }

    running = true;

    logger.info("BLEManager started.");
    return true;
}

/**
 * @brief Stops the complete BLE subsystem.
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
 * @brief Initializes the NimBLE stack when required.
 *
 * On Ch3rryN0de the ESP32-S3 BLE transmitter is configured with a
 * normal high transmit power while keeping the default BLE PHY.
 *
 * @return true when NimBLE is initialized and available.
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

#ifdef C3N0_S3
    NimBLEDevice::setPower(ESP_PWR_LVL_P6);
#endif

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
    disconnect();

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
 * @return true when scanning is active.
 */
bool BLEManager::isScanning() const
{
    return scanner != nullptr && scanner->isScanning();
}

/**
 * @brief Starts legacy connectable BLE advertising.
 *
 * Configures conservative legacy advertising suitable for both
 * BLE 4.x ESP32 centrals and BLE 5 capable ESP32-S3 devices.
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

    advertiser->reset();

    if (!advertiser->setConnectableMode(BLE_GAP_CONN_MODE_UND)) {
        logger.error("Failed to configure connectable BLE advertising.");
        return false;
    }

    if (!advertiser->setDiscoverableMode(BLE_GAP_DISC_MODE_GEN)) {
        logger.error("Failed to configure discoverable BLE advertising.");
        return false;
    }

#ifdef C3N0_S3
    advertiser->setMinInterval(160);
    advertiser->setMaxInterval(240);
#endif

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

    logger.info("BLE advertising started.");
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
 * @return true when advertising is active.
 */
bool BLEManager::isAdvertising() const
{
    return advertiser != nullptr && advertiser->isAdvertising();
}

/**
 * @brief Clears all stored BLE scan results.
 */
void BLEManager::clearDevices()
{
    deviceCount = 0;

    if (devices == nullptr) return;

    for (uint8_t i = 0; i < MAX_DEVICES; i++) devices[i] = BLEDeviceInfo();
}

/**
 * @brief Returns the number of discovered BLE devices.
 *
 * @return Number of stored BLE devices.
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
 * @brief Finds a discovered BLE device by address.
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
 * Existing advertisement information is preserved when later advertisement
 * or scan response packets do not contain the same optional fields.
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

    if (advertisedDevice->getServiceUUIDCount() > 0) {
        for (size_t i = 0; i < advertisedDevice->getServiceUUIDCount(); i++) {
            String serviceUUID = advertisedDevice->getServiceUUID(i).toString().c_str();

            if (!serviceUUID.isEmpty()) {
                device->serviceUUID = serviceUUID;
                break;
            }
        }
    }

    if (advertisedDevice->haveName()) {
        String name = advertisedDevice->getName().c_str();
        if (!name.isEmpty()) device->name = name;
    }

    device->rssi = advertisedDevice->getRSSI();
    device->lastSeen = millis();
    device->addressType = advertisedDevice->getAddressType();

    if (advertisedDevice->haveTXPower()) {
        device->txPower = advertisedDevice->getTXPower();
        device->hasTxPower = true;
    }
}

/**
 * @brief Constructs a characteristic callback router.
 *
 * @param callback Application callback invoked on writes.
 */
BLEManager::CharacteristicCallbacks::CharacteristicCallbacks(BLEWriteCallback callback)
    : callback(callback)
{
}

/**
 * @brief Handles a remote write to a local characteristic.
 *
 * @param characteristic Characteristic that received the write.
 * @param connInfo Information about the connected peer.
 */
void BLEManager::CharacteristicCallbacks::onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo& connInfo)
{
    if (characteristic == nullptr || callback == nullptr) return;

    NimBLEAttValue value = characteristic->getValue();
    String uuid = characteristic->getUUID().toString().c_str();

    callback(uuid, value.data(), value.size());
}

/**
 * @brief Creates a local GATT server and service.
 *
 * Installs server callbacks when the server is first created so
 * connection and disconnection events can be tracked.
 *
 * @param serviceUUID UUID of the service to create.
 *
 * @return true when the service was created successfully.
 */
bool BLEManager::createServer(const String& serviceUUID)
{
    if (!running) {
        logger.error("BLEManager is not running.");
        return false;
    }

    if (!ensureInitialized()) return false;

    if (server == nullptr) {
        server = NimBLEDevice::createServer();

        if (server == nullptr) {
            logger.error("Failed to create BLE server.");
            return false;
        }

        serverCallbacks = new ServerCallbacks(*this);
        server->setCallbacks(serverCallbacks);
    }

    serverService = server->createService(serviceUUID.c_str());

    if (serverService == nullptr) {
        logger.error("Failed to create BLE service.");
        return false;
    }

    logger.info(String("BLE GATT service created: ") + serviceUUID);
    return true;
}

/**
 * @brief Adds a characteristic to the current local GATT service.
 *
 * @param characteristicUUID UUID of the characteristic.
 * @param properties NimBLE characteristic property flags.
 * @param writeCallback Optional callback invoked when data is written.
 *
 * @return true when the characteristic was created successfully.
 */
bool BLEManager::addServerCharacteristic(const String& characteristicUUID, uint32_t properties, BLEWriteCallback writeCallback)
{
    if (serverService == nullptr) {
        logger.error("BLE server service is not configured.");
        return false;
    }

    NimBLECharacteristic* characteristic = serverService->createCharacteristic(characteristicUUID.c_str(), properties);

    if (characteristic == nullptr) {
        logger.error(String("Failed to create BLE characteristic: ") + characteristicUUID);
        return false;
    }

    if (writeCallback != nullptr) characteristic->setCallbacks(new CharacteristicCallbacks(writeCallback));

    logger.info(String("BLE characteristic created: ") + characteristicUUID);
    return true;
}

/**
 * @brief Updates the value of a local GATT characteristic.
 *
 * @param characteristicUUID UUID of the characteristic to update.
 * @param data Pointer to the new characteristic value.
 * @param length Number of bytes in the value.
 *
 * @return true when the characteristic value was updated successfully.
 */
bool BLEManager::setServerCharacteristicValue(const String& characteristicUUID, const uint8_t* data, size_t length)
{
    if (data == nullptr || length == 0) {
        logger.error("Invalid BLE characteristic value.");
        return false;
    }

    if (serverService == nullptr) {
        logger.error("BLE server service is not configured.");
        return false;
    }

    NimBLECharacteristic* characteristic = serverService->getCharacteristic(characteristicUUID.c_str());

    if (characteristic == nullptr) {
        logger.error(String("BLE server characteristic not found: ") + characteristicUUID);
        return false;
    }

    characteristic->setValue(data, length);
    return true;
}

/**
 * @brief Starts the configured local GATT server.
 *
 * @return true when the server started successfully.
 */
bool BLEManager::startServer()
{
    if (server == nullptr) {
        logger.error("BLE server is not configured.");
        return false;
    }

    if (!server->start()) {
        logger.error("Failed to start BLE GATT server.");
        return false;
    }

    logger.info("BLE GATT server started.");
    return true;
}

/**
 * @brief Connects to a remote BLE device.
 *
 * Stops active BLE discovery before creating a fresh client and explicitly
 * connects to the discovered peer address.
 *
 * @param address BLE device address.
 * @param addressType BLE address type discovered during scanning.
 *
 * @return true when connected successfully.
 */
bool BLEManager::connect(const String& address, uint8_t addressType)
{
    if (!running) {
        logger.error("BLEManager is not running.");
        return false;
    }

    if (!ensureInitialized()) return false;

    stopScan();
    stopAdvertising();
    disconnect();

    NimBLEAddress bleAddress(address.c_str(), addressType);

    client = NimBLEDevice::createClient();

    if (client == nullptr) {
        logger.error("Failed to create BLE client.");
        return false;
    }

    client->setConnectTimeout(10);

    logger.info(String("Connecting to BLE device ") + address + ".");

    if (!client->connect(bleAddress, true, false, false)) {
        int error = client->getLastError();

        logger.error(String("BLE connection failed, error: ") + error);

        NimBLEDevice::deleteClient(client);
        client = nullptr;

        return false;
    }

    logger.info("BLE connection established.");

    return true;
}

/**
 * @brief Disconnects and removes the active BLE client.
 */
void BLEManager::disconnect()
{
    if (client == nullptr) return;

    if (client->isConnected()) client->disconnect();

    NimBLEDevice::deleteClient(client);
    client = nullptr;

    logger.info("BLE client disconnected.");
}

/**
 * @brief Returns whether a BLE client connection is active.
 *
 * @return true when connected to a remote BLE server.
 */
bool BLEManager::isConnected() const
{
    return client != nullptr && client->isConnected();
}

/**
 * @brief Writes raw data to a remote GATT characteristic.
 *
 * @param serviceUUID UUID of the remote service.
 * @param characteristicUUID UUID of the remote characteristic.
 * @param data Pointer to the data to write.
 * @param length Number of bytes to write.
 *
 * @return true when the characteristic was written successfully.
 */
bool BLEManager::writeCharacteristic(const String& serviceUUID, const String& characteristicUUID, const uint8_t* data, size_t length)
{
    if (!isConnected() || data == nullptr || length == 0) {
        logger.error("BLE characteristic write requested without an active connection.");
        return false;
    }

    NimBLERemoteService* service = client->getService(serviceUUID.c_str());

    if (service == nullptr) {
        logger.error(String("Remote BLE service not found: ") + serviceUUID);
        return false;
    }

    NimBLERemoteCharacteristic* characteristic = service->getCharacteristic(characteristicUUID.c_str());

    if (characteristic == nullptr) {
        logger.error(String("Remote BLE characteristic not found: ") + characteristicUUID);
        return false;
    }

    if (!characteristic->canWrite()) {
        logger.error("Remote BLE characteristic is not writable.");
        return false;
    }

    if (!characteristic->writeValue(data, length, true)) {
        logger.error("Failed to write BLE characteristic.");
        return false;
    }

    logger.info(String("BLE characteristic written: ") + characteristicUUID);
    return true;
}

/**
 * @brief Completely shuts down the BLE subsystem.
 *
 * Stops active BLE operations, releases local BLE state and
 * fully deinitializes NimBLE.
 */
void BLEManager::shutdown()
{
    stopScan();
    stopAdvertising();
    disconnect();

    delete[] devices;
    devices = nullptr;
    deviceCount = 0;

    delete serverCallbacks;
    serverCallbacks = nullptr;

    if (!initialized) return;

    logger.info("Deinitializing NimBLE.");

    NimBLEDevice::deinit(true);

    scanner = nullptr;
    advertiser = nullptr;
    server = nullptr;
    serverService = nullptr;
    client = nullptr;
    initialized = false;

    logger.info("NimBLE deinitialized.");
}

/**
 * @brief Constructs BLE server callbacks.
 *
 * @param manager BLEManager that owns the server.
 */
BLEManager::ServerCallbacks::ServerCallbacks(BLEManager& manager)
    : manager(manager)
{
}

/**
 * @brief Handles an incoming BLE client connection.
 *
 * @param server Local NimBLE server.
 * @param connInfo Information about the connected peer.
 */
void BLEManager::ServerCallbacks::onConnect(NimBLEServer* server, NimBLEConnInfo& connInfo)
{
    manager.logger.info(String("BLE client connected: ") + connInfo.getAddress().toString().c_str());
}

/**
 * @brief Handles a BLE client disconnection.
 *
 * Restarts advertising so Ch3rryN0de remains available for
 * configuration after a client disconnects.
 *
 * @param server Local NimBLE server.
 * @param connInfo Information about the disconnected peer.
 * @param reason BLE disconnection reason.
 */
void BLEManager::ServerCallbacks::onDisconnect(NimBLEServer* server, NimBLEConnInfo& connInfo, int reason)
{
    manager.logger.info(String("BLE client disconnected, reason: ") + reason);

    if (manager.advertiser != nullptr && !manager.advertiser->isAdvertising()) {
        if (manager.advertiser->start())
            manager.logger.info("BLE advertising restarted.");
        else
            manager.logger.error("Failed to restart BLE advertising.");
    }
}