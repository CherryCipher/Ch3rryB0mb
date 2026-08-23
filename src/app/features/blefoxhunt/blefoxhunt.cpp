/**
 * @file blefoxhunt.cpp
 * @brief Implementation of the BLE Fox Hunt feature.
 */

#include "blefoxhunt.h"

#include "../../../services/services.h"

/**
 * @brief Constructs the BLE Fox Hunt feature.
 *
 * @param services Reference to the shared application services.
 */
BLEFoxHunt::BLEFoxHunt(Services& services) : services(services)
{
}

/**
 * @brief Sets the BLE device that should be tracked.
 *
 * @param address BLE address of the target device.
 * @param name Display name of the target device.
 */
void BLEFoxHunt::setTarget(const String& address, const String& name)
{
    targetAddress = address;
    targetName = name.length() > 0 ? name : "< unknown >";

    currentRSSI = -127;
    peakRSSI = -127;
    lastSeen = 0;

    services.logger.info("BLE Fox Hunt: Target set to " + targetAddress);
}

/**
 * @brief Starts BLE target tracking.
 */
bool BLEFoxHunt::start()
{
    if (!hasTarget())
    {
        services.logger.error("BLE Fox Hunt: No target selected.");
        return false;
    }

    currentRSSI = -127;
    peakRSSI = -127;
    lastSeen = 0;

    services.logger.info("BLE Fox Hunt: Starting hunt for " + targetAddress);

    if (!services.ble.startScan())
    {
        services.logger.error("BLE Fox Hunt: Failed to start BLE scan.");
        return false;
    }

    return true;
}

/**
 * @brief Stops active BLE scanning.
 *
 * Stops the BLE advertisement scan without releasing the underlying
 * NimBLE runtime resources.
 */
void BLEFoxHunt::stop()
{
    if (!services.ble.isScanning())
        return;

    services.ble.stopScan();

    services.logger.info("BLE Fox Hunt: Scan stopped.");
}

/**
 * @brief Completely shuts down BLE Fox Hunt resources.
 *
 * Stops BLE activity and releases the NimBLE runtime resources so they
 * become available to other ESP32 subsystems.
 */
void BLEFoxHunt::shutdown()
{
    services.ble.shutdown();

    services.logger.info("BLE Fox Hunt: BLE subsystem released.");
}

/**
 * @brief Updates the current target measurements.
 */
bool BLEFoxHunt::update()
{
    if (!hasTarget())
        return false;

    int index = services.ble.findDevice(targetAddress);

    if (index < 0)
        return false;

    const BLEDeviceInfo& device = services.ble.getDevice(index);

    currentRSSI = device.rssi;
    lastSeen = device.lastSeen;

    if (peakRSSI == -127 || currentRSSI > peakRSSI)
        peakRSSI = currentRSSI;

    return true;
}

/**
 * @brief Returns whether a target has been configured.
 */
bool BLEFoxHunt::hasTarget() const
{
    return targetAddress.length() > 0;
}

/**
 * @brief Returns whether BLE scanning is currently active.
 */
bool BLEFoxHunt::isRunning() const
{
    return services.ble.isScanning();
}

/**
 * @brief Returns the selected target address.
 */
const String& BLEFoxHunt::getTargetAddress() const
{
    return targetAddress;
}

/**
 * @brief Returns the selected target name.
 */
const String& BLEFoxHunt::getTargetName() const
{
    return targetName;
}

/**
 * @brief Returns the latest RSSI measurement.
 */
int8_t BLEFoxHunt::getRSSI() const
{
    return currentRSSI;
}

/**
 * @brief Returns the strongest RSSI observed during the current hunt.
 */
int8_t BLEFoxHunt::getPeakRSSI() const
{
    return peakRSSI;
}

/**
 * @brief Returns when the target was last observed.
 */
uint32_t BLEFoxHunt::getLastSeen() const
{
    return lastSeen;
}

/**
 * @brief Returns target signal strength as a percentage.
 */
uint8_t BLEFoxHunt::getSignalPercent() const
{
    if (currentRSSI <= -100)
        return 0;

    if (currentRSSI >= -40)
        return 100;

    return static_cast<uint8_t>(
        ((static_cast<int16_t>(currentRSSI) + 100) * 100) / 60
    );
}

/**
 * @brief Returns a human-readable signal strength description.
 */
const char* BLEFoxHunt::getSignalText() const
{
    if (currentRSSI == -127)
        return "SEARCHING";

    if (currentRSSI >= -55)
        return "VERY CLOSE";

    if (currentRSSI >= -67)
        return "STRONG";

    if (currentRSSI >= -75)
        return "MEDIUM";

    if (currentRSSI >= -85)
        return "WEAK";

    return "VERY WEAK";
}