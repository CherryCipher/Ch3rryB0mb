/**
 * @file bleexplorer.cpp
 * @brief Implementation of the BLE Explorer feature.
 */

#include "bleexplorer.h"
#include "../../../services/services.h"

/**
 * @brief Constructs the BLE Explorer feature.
 */
BLEExplorer::BLEExplorer(Services& services) : services(services)
{
}

/**
 * @brief Starts BLE device scanning.
 */
bool BLEExplorer::startScan()
{
    services.logger.info("BLE Explorer: Starting scan.");

    if (!services.ble.startScan())
    {
        services.logger.error("BLE Explorer: Failed to start scan.");
        return false;
    }

    return true;
}

/**
 * @brief Stops BLE device scanning.
 */
void BLEExplorer::stopScan()
{
    if (!services.ble.isScanning())
        return;

    services.ble.stopScan();
    services.logger.info("BLE Explorer: Scan stopped.");
}

/**
 * @brief Returns whether BLE scanning is active.
 */
bool BLEExplorer::isScanning() const
{
    return services.ble.isScanning();
}

/**
 * @brief Returns the number of discovered BLE devices.
 */
uint8_t BLEExplorer::getDeviceCount() const
{
    return services.ble.getDeviceCount();
}

/**
 * @brief Returns a discovered BLE device.
 */
const BLEDeviceInfo& BLEExplorer::getDevice(uint8_t index) const
{
    return services.ble.getDevice(index);
}

/**
 * @brief Selects a discovered BLE device.
 */
bool BLEExplorer::selectDevice(uint8_t index)
{
    if (index >= getDeviceCount())
        return false;

    selectedAddress = getDevice(index).address;
    deviceSelected = true;

    services.logger.info("BLE Explorer: Selected " + selectedAddress);

    return true;
}

/**
 * @brief Returns whether a BLE target has been selected.
 */
bool BLEExplorer::hasSelectedDevice() const
{
    return deviceSelected;
}

/**
 * @brief Returns the selected BLE address.
 */
const String& BLEExplorer::getSelectedAddress() const
{
    return selectedAddress;
}

/**
 * @brief Shuts down BLE Explorer resources.
 *
 * Stops BLE activity through the shared BLEManager and releases the
 * underlying NimBLE runtime resources.
 *
 * This should be called when leaving the BLE Explorer so memory used
 * by the BLE stack becomes available to other ESP32 subsystems such
 * as Wi-Fi.
 */
void BLEExplorer::shutdown()
{
    services.ble.shutdown();
    services.logger.info("BLE Explorer: BLE subsystem released.");
}