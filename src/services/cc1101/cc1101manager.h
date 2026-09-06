/**
 * @file cc1101manager.h
 * @brief Declaration of the CC1101 radio manager.
 */

#pragma once

#include <Arduino.h>
#include <RadioLib.h>

#include "../logger/logger.h"
#include "../spi/spimanager.h"

/**
 * @class CC1101Manager
 * @brief Manages CC1101 Sub-GHz radio functionality.
 *
 * The CC1101Manager is responsible for initializing and controlling the
 * CC1101 radio used by Ch3rryB0mb.
 *
 * Spectrum scanning is implemented incrementally. Each scan update
 * measures only one frequency so control can quickly return to the
 * application and UI.
 */
class CC1101Manager
{
public:
    /**
     * @brief Number of measurement points used for a frequency range scan.
     */
    static constexpr uint8_t SCAN_POINT_COUNT = 100;

    /**
     * @brief Constructs a new CC1101Manager.
     *
     * @param logger Reference to the application's Logger.
     * @param spiManager Reference to the application's SPIManager.
     */
    CC1101Manager(Logger& logger, SPIManager& spiManager);

    /**
     * @brief Initializes the CC1101 radio.
     *
     * @return true if initialization succeeded.
     * @return false otherwise.
     */
    bool start();

    /**
     * @brief Stops the CC1101Manager.
     *
     * @return true when the manager has stopped.
     */
    bool stop();

    /**
     * @brief Returns whether the CC1101Manager is running.
     *
     * @return true if running.
     * @return false otherwise.
     */
    bool isRunning() const;

    /**
     * @brief Starts an incremental frequency range scan.
     *
     * Initializes the scan state but does not perform any measurements.
     * Measurements are performed individually through updateScan().
     *
     * @param startMHz Start frequency in MHz.
     * @param endMHz End frequency in MHz.
     *
     * @return true if the scan was initialized successfully.
     * @return false if the manager is unavailable or the range is invalid.
     */
    bool startScan(float startMHz, float endMHz);

    /**
     * @brief Performs one step of the active frequency scan.
     *
     * Measures exactly one frequency and stores its RSSI value in the
     * supplied result array.
     *
     * @param results Array receiving RSSI measurements in dBm.
     *
     * @return true if a complete sweep has just been completed.
     * @return false if the sweep is still in progress or no scan is active.
     */
    bool updateScan(int16_t results[SCAN_POINT_COUNT]);

    /**
     * @brief Stops the active frequency scan.
     */
    void stopScan();

    /**
     * @brief Returns whether a frequency scan is active.
     *
     * @return true if scanning.
     * @return false otherwise.
     */
    bool isScanning() const;

private:
    /**
     * @brief Receiver settling time before reading live RSSI.
     */
    static constexpr uint16_t RSSI_SETTLE_US = 1000;

    /** @brief Reference to the application's Logger. */
    Logger& logger;

    /** @brief Reference to the shared SPI manager. */
    SPIManager& spiManager;

    /** @brief RadioLib module configuration. */
    Module module;

    /** @brief RadioLib CC1101 instance. */
    CC1101 radio;

    /** @brief Indicates whether the manager is running. */
    bool running = false;

    /** @brief Indicates whether an incremental scan is active. */
    bool scanning = false;

    /** @brief Current measurement index in the active scan. */
    uint8_t scanIndex = 0;

    /** @brief Start frequency of the active scan in MHz. */
    float scanStartMHz = 0.0f;

    /** @brief End frequency of the active scan in MHz. */
    float scanEndMHz = 0.0f;

    /** @brief Frequency step between scan points in MHz. */
    float scanStepMHz = 0.0f;

    /**
     * @brief Measures live RSSI at a single frequency.
     *
     * @param frequencyMHz Frequency to measure in MHz.
     * @param rssi Reference receiving RSSI in dBm.
     *
     * @return true if the measurement succeeded.
     * @return false otherwise.
     */
    bool measureFrequency(float frequencyMHz, int16_t& rssi);
};