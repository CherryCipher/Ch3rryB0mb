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
 * The structure contains only information required by Ch3rryB0mb
 * features and does not expose the underlying NimBLE API.
 */
struct BLEDeviceInfo
{
    String name;
    String address;
    int8_t rssi = -127;
    int8_t txPower = 0;
    bool hasTxPower = false;
    uint32_t lastSeen = 0;
};

/**
 * @class BLEManager
 * @brief Manages Bluetooth Low Energy scanning.
 *
 * BLEManager provides a small abstraction around the ESP32 BLE radio.
 *
 * It continuously receives BLE advertisements without blocking the
 * application loop. Discovered devices are stored internally and can
 * later be used by BLE features such as the BLE Explorer and Fox Hunt.
 *
 * Applications should not directly use the NimBLE API. BLE operations
 * should be performed through BLEManager.
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
     * @brief Initializes the BLE subsystem.
     *
     * Initializes NimBLE and configures the BLE scanner.
     * Scanning is not automatically started.
     *
     * @return true if initialization succeeded.
     */
    bool start();

    /**
     * @brief Stops BLE scanning and disables the manager.
     *
     * @return true when the manager has stopped.
     */
    bool stop();

    /**
     * @brief Returns whether the BLEManager is running.
     *
     * @return true if initialized and available.
     * @return false otherwise.
     */
    bool isRunning() const;

    /**
     * @brief Starts continuous BLE scanning.
     *
     * Existing scan results are cleared before the scan starts.
     *
     * @return true if scanning started successfully.
     * @return false if the manager is unavailable.
     */
    bool startScan();

    /**
     * @brief Stops the active BLE scan.
     */
    void stopScan();

    /**
     * @brief Returns whether a BLE scan is active.
     *
     * @return true if scanning.
     * @return false otherwise.
     */
    bool isScanning() const;

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
     * @return Index of the device or -1 when not found.
     */
    int findDevice(const String& address) const;

    /**
     * @brief Completely shuts down the BLE subsystem.
     *
     * Stops scanning, releases the BLE device cache and deinitializes
     * NimBLE so its runtime memory becomes available to other subsystems
     * such as Wi-Fi.
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
     * @brief Reference to the application's Logger.
     */
    Logger& logger;

    /**
     * @brief NimBLE scanner instance.
     */
    NimBLEScan* scanner = nullptr;

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
     * @brief Processes a received BLE advertisement.
     *
     * Called by NimBLE whenever a complete advertisement result
     * becomes available.
     *
     * @param advertisedDevice BLE advertisement received by NimBLE.
     */
    void onResult(const NimBLEAdvertisedDevice* advertisedDevice) override;

    /**
     * @brief Stores or updates a discovered BLE device.
     *
     * Existing devices are matched using their BLE address.
     *
     * @param advertisedDevice BLE advertisement to process.
     */
    void updateDevice(const NimBLEAdvertisedDevice* advertisedDevice);
};