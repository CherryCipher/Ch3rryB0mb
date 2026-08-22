/**
 * @file blefoxhunt.h
 * @brief BLE Fox Hunt feature interface.
 */

#pragma once

#include <Arduino.h>

class Services;

/**
 * @class BLEFoxHunt
 * @brief Tracks the signal strength of a selected BLE device.
 *
 * BLEFoxHunt continuously scans for a previously selected BLE device
 * and exposes its live RSSI, peak RSSI and last-seen information.
 *
 * Low-level BLE operations are delegated to BLEManager.
 */
class BLEFoxHunt
{
public:
    /**
     * @brief Constructs the BLE Fox Hunt feature.
     *
     * @param services Reference to the shared application services.
     */
    explicit BLEFoxHunt(Services& services);

    /**
     * @brief Sets the BLE device that should be tracked.
     *
     * The address and name are copied so the target remains available
     * after the BLE Explorer releases its scan cache.
     *
     * @param address BLE address of the target device.
     * @param name Display name of the target device.
     */
    void setTarget(const String& address, const String& name);

    /**
     * @brief Starts tracking the selected BLE target.
     *
     * Starts a continuous BLE advertisement scan. Incoming advertisements
     * are stored by BLEManager and matched against the selected target.
     *
     * @return true if tracking started successfully.
     */
    bool start();

    /**
     * @brief Stops active BLE scanning without releasing the BLE subsystem.
     */
    void stop();

    /**
     * @brief Completely shuts down BLE Fox Hunt resources.
     *
     * Stops scanning and releases the NimBLE runtime resources so they
     * become available to other ESP32 subsystems.
     */
    void shutdown();

    /**
     * @brief Updates the current target measurements.
     *
     * Searches the BLEManager scan cache for the selected target and
     * updates RSSI, peak RSSI and last-seen information when found.
     *
     * @return true if the target is currently present in the scan cache.
     */
    bool update();

    /**
     * @brief Returns whether a target has been configured.
     *
     * @return true when a BLE target is available.
     */
    bool hasTarget() const;

    /**
     * @brief Returns whether BLE scanning is currently active.
     *
     * @return true while the BLE scan is running.
     */
    bool isRunning() const;

    /**
     * @brief Returns the selected BLE target address.
     *
     * @return Target BLE address.
     */
    const String& getTargetAddress() const;

    /**
     * @brief Returns the selected BLE target name.
     *
     * @return Target display name.
     */
    const String& getTargetName() const;

    /**
     * @brief Returns the latest RSSI measurement.
     *
     * @return RSSI in dBm.
     */
    int8_t getRSSI() const;

    /**
     * @brief Returns the strongest RSSI observed during the current hunt.
     *
     * @return Peak RSSI in dBm.
     */
    int8_t getPeakRSSI() const;

    /**
     * @brief Returns when the target was last observed.
     *
     * @return millis() timestamp of the last advertisement.
     */
    uint32_t getLastSeen() const;

    /**
     * @brief Returns target signal strength as a percentage.
     *
     * RSSI values from approximately -100 dBm to -40 dBm are mapped
     * to a display range from 0 to 100 percent.
     *
     * @return Signal strength percentage.
     */
    uint8_t getSignalPercent() const;

    /**
     * @brief Returns a human-readable signal strength description.
     *
     * @return Signal strength label.
     */
    const char* getSignalText() const;

private:
    /**
     * @brief Shared application services.
     */
    Services& services;

    /**
     * @brief BLE address of the selected target.
     */
    String targetAddress;

    /**
     * @brief Display name of the selected target.
     */
    String targetName;

    /**
     * @brief Latest target RSSI.
     */
    int8_t currentRSSI = -127;

    /**
     * @brief Strongest RSSI observed during the current hunt.
     */
    int8_t peakRSSI = -127;

    /**
     * @brief Timestamp of the most recent target advertisement.
     */
    uint32_t lastSeen = 0;
};