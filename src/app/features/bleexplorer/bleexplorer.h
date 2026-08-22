/**
 * @file bleexplorer.h
 * @brief BLE Explorer feature interface.
 */

#pragma once

#include "../../../services/ble/blemanager.h"

class Services;

/**
 * @class BLEExplorer
 * @brief Controls BLE discovery and device selection.
 *
 * BLEExplorer provides application-level BLE discovery functionality.
 * Low-level BLE operations are delegated to BLEManager.
 *
 * Discovered devices can be selected as targets for other BLE features,
 * such as BLE Fox Hunt.
 */
class BLEExplorer
{
public:
    /**
     * @brief Constructs the BLE Explorer feature.
     *
     * @param services Reference to the shared application services.
     */
    explicit BLEExplorer(Services& services);

    /**
     * @brief Starts BLE device scanning.
     *
     * @return true if scanning started successfully.
     */
    bool startScan();

    /**
     * @brief Stops BLE device scanning.
     */
    void stopScan();

    /**
     * @brief Returns whether BLE scanning is active.
     *
     * @return true when scanning.
     */
    bool isScanning() const;

    /**
     * @brief Returns the number of discovered BLE devices.
     *
     * @return Number of discovered devices.
     */
    uint8_t getDeviceCount() const;

    /**
     * @brief Returns a discovered BLE device.
     *
     * @param index Index of the requested device.
     *
     * @return Constant reference to the discovered device.
     */
    const BLEDeviceInfo& getDevice(uint8_t index) const;

    /**
     * @brief Selects a discovered BLE device.
     *
     * @param index Index of the device to select.
     *
     * @return true if the device was selected.
     */
    bool selectDevice(uint8_t index);

    /**
     * @brief Returns whether a BLE target has been selected.
     *
     * @return true when a target is selected.
     */
    bool hasSelectedDevice() const;

    /**
     * @brief Returns the address of the selected BLE device.
     *
     * @return BLE address of the selected target.
     */
    const String& getSelectedAddress() const;

    /**
     * @brief Completely shuts down BLE Explorer resources.
     */
    void shutdown();

private:
    /**
     * @brief Reference to the shared application services.
     */
    Services& services;

    /**
     * @brief BLE address selected for another BLE feature.
     */
    String selectedAddress;

    /**
     * @brief Indicates whether a BLE target has been selected.
     */
    bool deviceSelected = false;
};