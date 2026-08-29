/**
 * @file blemanager.h
 * @brief Declaration of the BLE manager.
 *
 * Provides shared Bluetooth Low Energy functionality including scanning,
 * advertising, GATT server hosting and GATT client communication.
 */

#pragma once

#include <Arduino.h>
#include <NimBLEDevice.h>

#include "../logger/logger.h"

/**
 * @struct BLEDeviceInfo
 * @brief Contains information about a discovered BLE device.
 *
 * Represents one BLE advertiser discovered by BLEManager.
 * Only application-relevant information is exposed so higher-level
 * features do not depend directly on NimBLE objects.
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
 * @brief Callback invoked when data is written to a local BLE characteristic.
 *
 * @param characteristicUUID UUID of the written characteristic.
 * @param data Pointer to the received data.
 * @param length Number of received bytes.
 */
using BLEWriteCallback = void (*)(const String& characteristicUUID, const uint8_t* data, size_t length);

/**
 * @class BLEManager
 * @brief Manages shared Bluetooth Low Energy functionality.
 *
 * BLEManager owns the NimBLE lifecycle and provides generic BLE
 * functionality to both Ch3rryB0mb and Ch3rryN0de.
 *
 * Supported functionality includes device scanning, advertising,
 * local GATT server creation, remote GATT client connections and
 * characteristic value access.
 *
 * Higher-level features are responsible for interpreting BLE data
 * and implementing application-specific protocols.
 */
class BLEManager : private NimBLEScanCallbacks
{
public:
    /**
     * @brief Maximum number of discovered BLE devices stored by the manager.
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
     * Marks the manager as available without immediately initializing
     * the NimBLE stack. NimBLE is initialized lazily when required.
     *
     * @return true when the manager is ready.
     */
    bool start();

    /**
     * @brief Stops the BLE manager.
     *
     * Stops all active BLE functionality and deinitializes NimBLE.
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
     * Active advertising is stopped before the scan begins and
     * previously discovered devices are cleared.
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
     * @return true when scanning is active.
     */
    bool isScanning() const;

    /**
     * @brief Starts BLE advertising.
     *
     * Active scanning is stopped before advertising begins.
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
     * @return true when advertising is active.
     */
    bool isAdvertising() const;

    /**
     * @brief Clears all stored BLE scan results.
     */
    void clearDevices();

    /**
     * @brief Returns the number of discovered BLE devices.
     *
     * @return Number of stored BLE devices.
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
     * @brief Finds a discovered BLE device by address.
     *
     * @param address BLE address to search for.
     *
     * @return Device index or -1 when not found.
     */
    int findDevice(const String& address) const;

    /**
     * @brief Creates a local GATT server and service.
     *
     * @param serviceUUID UUID of the service to create.
     *
     * @return true when the service was created successfully.
     */
    bool createServer(const String& serviceUUID);

    /**
     * @brief Adds a characteristic to the current local GATT service.
     *
     * The characteristic is stored internally so its value can later
     * be updated through setServerCharacteristicValue().
     *
     * @param characteristicUUID UUID of the characteristic.
     * @param properties NimBLE characteristic property flags.
     * @param writeCallback Optional callback invoked when data is written.
     *
     * @return true when the characteristic was created successfully.
     */
    bool addServerCharacteristic(const String& characteristicUUID, uint32_t properties, BLEWriteCallback writeCallback = nullptr);

    /**
     * @brief Updates the value of a local GATT characteristic.
     *
     * @param characteristicUUID UUID of the characteristic to update.
     * @param data Pointer to the new characteristic value.
     * @param length Number of bytes in the value.
     *
     * @return true when the characteristic value was updated successfully.
     */
    bool setServerCharacteristicValue(const String& characteristicUUID, const uint8_t* data, size_t length);

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
     * @brief Returns whether a BLE client connection is active.
     *
     * @return true when connected to a remote BLE server.
     */
    bool isConnected() const;

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
    bool writeCharacteristic(const String& serviceUUID, const String& characteristicUUID, const uint8_t* data, size_t length);

    /**
     * @brief Completely shuts down the BLE subsystem.
     *
     * Stops scanning and advertising, disconnects the active client,
     * releases local state and deinitializes NimBLE.
     *
     * BLEManager remains available and can initialize NimBLE again
     * when scanning, advertising or another BLE operation is requested.
     */
    void shutdown();

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
     * @class CharacteristicCallbacks
     * @brief Routes local characteristic writes to an application callback.
     */
    class CharacteristicCallbacks : public NimBLECharacteristicCallbacks
    {
    public:
        /**
         * @brief Constructs a characteristic callback router.
         *
         * @param callback Application callback invoked when data is written.
         */
        explicit CharacteristicCallbacks(BLEWriteCallback callback);

        /**
         * @brief Handles a remote write to a local characteristic.
         *
         * @param characteristic Characteristic that received the write.
         * @param connInfo Information about the connected peer.
         */
        void onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo& connInfo) override;

    private:
        /**
         * @brief Application callback invoked on characteristic writes.
         */
        BLEWriteCallback callback;
    };

    
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
     * @brief NimBLE GATT server instance.
     */
    NimBLEServer* server = nullptr;

    /**
     * @brief Current local GATT service.
     */
    NimBLEService* serverService = nullptr;

    /**
     * @brief Active NimBLE client instance.
     */
    NimBLEClient* client = nullptr;

    /**
     * @brief Devices discovered during the current scan.
     */
    BLEDeviceInfo* devices = nullptr;

    /**
     * @brief Number of valid discovered BLE devices.
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
};