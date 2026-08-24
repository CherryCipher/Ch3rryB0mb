/**
 * @file blemanager.h
 * @brief Declaration of the BLE manager.
 */

#pragma once

#include <Arduino.h>
#include <NimBLEDevice.h>

#include "../logger/logger.h"

/**
 * @struct BLEDeviceInfo
 * @brief Contains information about a discovered BLE device.
 *
 * Represents one BLE advertiser discovered by the BLEManager.
 * The structure contains only information required by application
 * features and does not expose the underlying NimBLE API.
 */
struct BLEDeviceInfo
{
    String name;
    String address;
    String serviceUUID;
    int8_t rssi = -127;
    int8_t txPower = 0;
    bool hasTxPower = false;
    uint32_t lastSeen = 0;
    uint8_t addressType = BLE_ADDR_PUBLIC;
};

/**
 * @brief Callback used when data is written to a local BLE characteristic.
 *
 * @param characteristicUUID UUID of the written characteristic.
 * @param data Pointer to the received data.
 * @param length Number of received bytes.
 */
using BLEWriteCallback = void (*)(const String& characteristicUUID, const uint8_t* data, size_t length);

/**
 * @class BLEManager
 * @brief Manages Bluetooth Low Energy scanning and advertising.
 *
 * BLEManager provides a shared abstraction around the ESP32 BLE radio.
 *
 * The manager owns the NimBLE lifecycle and exposes BLE scanning and
 * advertising functionality to both Ch3rryB0mb and Ch3rryN0de.
 *
 * Applications should not directly initialize or deinitialize NimBLE.
 */
class BLEManager : private NimBLEScanCallbacks
{
public:
    /**
     * @brief Maximum number of BLE devices stored by the manager.
     */
    static constexpr uint8_t MAX_DEVICES = 40;

    /**
     * @brief Constructs a new BLEManager.
     *
     * @param logger Reference to the application's Logger.
     */
    explicit BLEManager(Logger& logger);

    /**
     * @brief Starts the BLE manager.
     *
     * Prepares BLE for use without immediately initializing the NimBLE stack.
     * Scanning or advertising initialize NimBLE when first required.
     *
     * @return true when the manager is ready.
     */
    bool start();

    /**
     * @brief Stops the BLE manager.
     *
     * Stops scanning and advertising and completely deinitializes NimBLE.
     *
     * @return true when the manager has stopped.
     */
    bool stop();

    /**
     * @brief Returns whether the BLE manager is running.
     *
     * @return true when the manager is available.
     */
    bool isRunning() const;

    /**
     * @brief Starts continuous BLE scanning.
     *
     * Any active BLE advertisement is stopped before scanning begins.
     * Existing scan results are cleared.
     *
     * @return true when scanning started successfully.
     */
    bool startScan();

    /**
     * @brief Stops the active BLE scan.
     */
    void stopScan();

    /**
     * @brief Returns whether BLE scanning is active.
     *
     * @return true when scanning.
     */
    bool isScanning() const;

    /**
     * @brief Starts BLE advertising.
     *
     * Any active BLE scan is stopped before advertising begins.
     *
     * @param name Device name included in the advertisement.
     * @param serviceUUID Optional service UUID to advertise.
     *
     * @return true when advertising started successfully.
     */
    bool startAdvertising(const String& name, const String& serviceUUID = "");

    /**
     * @brief Stops BLE advertising.
     */
    void stopAdvertising();

    /**
     * @brief Returns whether BLE advertising is active.
     *
     * @return true when advertising.
     */
    bool isAdvertising() const;

    /**
     * @brief Clears all stored BLE devices.
     */
    void clearDevices();

    /**
     * @brief Returns the number of discovered BLE devices.
     *
     * @return Number of stored devices.
     */
    uint8_t getDeviceCount() const;

    /**
     * @brief Returns information about a discovered BLE device.
     *
     * @param index Index of the requested device.
     *
     * @return Constant reference to the BLE device information.
     */
    const BLEDeviceInfo& getDevice(uint8_t index) const;

    /**
     * @brief Finds a stored BLE device by address.
     *
     * @param address BLE address to search for.
     *
     * @return Device index or -1 when not found.
     */
    int findDevice(const String& address) const;

    /**
     * @brief Completely shuts down the BLE subsystem.
     *
     * Stops scanning and advertising, releases the device cache and
     * deinitializes NimBLE.
     *
     * The BLEManager itself remains available and NimBLE can later be
     * initialized again by starting a scan or advertisement.
     */
    void shutdown();

    /**
     * @brief Creates a local GATT service.
     *
     * @param serviceUUID UUID of the service to create.
     *
     * @return true when the service was created successfully.
     */
    bool createServer(const String& serviceUUID);

    /**
     * @brief Adds a characteristic to the current local GATT service.
     *
     * @param characteristicUUID UUID of the characteristic.
     * @param properties NimBLE characteristic property flags.
     * @param writeCallback Optional callback invoked when data is written.
     *
     * @return true when the characteristic was created successfully.
     */
    bool addServerCharacteristic(const String& characteristicUUID, uint32_t properties, BLEWriteCallback writeCallback = nullptr);

    /**
     * @brief Starts the configured local GATT server.
     *
     * @return true when the server started successfully.
     */
    bool startServer();

    /**
     * @brief Connects to a remote BLE device.
     *
     * @param address BLE device address.
     * @param addressType BLE address type discovered during scanning.
     *
     * @return true when connected successfully.
     */
    bool connect(const String& address, uint8_t addressType);

    /**
     * @brief Disconnects the active BLE client connection.
     */
    void disconnect();

    /**
     * @brief Returns whether the BLE client is connected.
     *
     * @return true when connected to a remote BLE server.
     */
    bool isConnected() const;

    /**
     * @brief Writes raw data to a remote GATT characteristic.
     *
     * @param serviceUUID UUID of the remote service.
     * @param characteristicUUID UUID of the remote characteristic.
     * @param data Pointer to the bytes to write.
     * @param length Number of bytes to write.
     *
     * @return true when the write completed successfully.
     */
    bool writeCharacteristic(const String& serviceUUID, const String& characteristicUUID, const uint8_t* data, size_t length);

private:
    /**
     * @brief BLE scan interval in milliseconds.
     */
    static constexpr uint16_t SCAN_INTERVAL_MS = 45;

    /**
     * @brief BLE scan window in milliseconds.
     */
    static constexpr uint16_t SCAN_WINDOW_MS = 30;

    /**
     * @brief Reference to the application's Logger.
     */
    Logger& logger;

    /**
     * @brief NimBLE scanner instance.
     */
    NimBLEScan* scanner = nullptr;

    /**
     * @brief NimBLE advertising instance.
     */
    NimBLEAdvertising* advertiser = nullptr;

    /**
     * @brief Devices discovered during the current scan.
     */
    BLEDeviceInfo* devices = nullptr;

    /**
     * @brief Number of valid devices currently stored.
     */
    uint8_t deviceCount = 0;

    /**
     * @brief Indicates whether BLEManager is available.
     */
    bool running = false;

    /**
     * @brief Indicates whether NimBLE has been initialized.
     */
    bool initialized = false;

    /**
     * @brief Initializes the NimBLE stack when required.
     *
     * @return true when NimBLE is available.
     */
    bool ensureInitialized();

    /**
     * @brief Processes a received BLE advertisement.
     *
     * @param advertisedDevice BLE advertisement received by NimBLE.
     */
    void onResult(const NimBLEAdvertisedDevice* advertisedDevice) override;

    /**
     * @brief Stores or updates a discovered BLE device.
     *
     * @param advertisedDevice BLE advertisement to process.
     */
    void updateDevice(const NimBLEAdvertisedDevice* advertisedDevice);

    /**
     * @class CharacteristicCallbacks
     * @brief Routes NimBLE characteristic writes back to BLEManager.
     */
    class CharacteristicCallbacks : public NimBLECharacteristicCallbacks
    {
    public:
        /**
         * @brief Constructs a characteristic callback router.
         *
         * @param manager BLEManager that owns the characteristic.
         * @param callback Application callback invoked on writes.
         */
        CharacteristicCallbacks(BLEManager& manager, BLEWriteCallback callback);

        /**
         * @brief Handles a remote write to a local characteristic.
         *
         * @param characteristic Written characteristic.
         * @param connInfo Information about the connected peer.
         */
        void onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo& connInfo) override;

    private:
        BLEManager& manager;
        BLEWriteCallback callback;
    };

    NimBLEServer* server = nullptr;
    NimBLEService* serverService = nullptr;
    NimBLEClient* client = nullptr;
};