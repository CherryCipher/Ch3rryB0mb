/**
 * @file cc1101scanner.h
 * @brief Declaration of the CC1101 Sub-GHz scanner feature.
 */

#pragma once

#include <Arduino.h>

#include "services/services.h"

/**
 * @class CC1101Scanner
 * @brief Application feature for continuous Sub-GHz spectrum scanning.
 *
 * The scanner configures the requested frequency range and delegates
 * incremental RF measurements to the CC1101Manager.
 */
class CC1101Scanner
{
public:
    /**
     * @brief Constructs a new CC1101Scanner.
     *
     * @param services Reference to the application's Services container.
     */
    explicit CC1101Scanner(Services& services);

    /**
     * @brief Starts continuous Sub-GHz scanning.
     *
     * @return true if scanning started successfully.
     * @return false otherwise.
     */
    bool start();

    /**
     * @brief Stops continuous Sub-GHz scanning.
     *
     * @return true when scanning has stopped.
     */
    bool stop();

    /**
     * @brief Updates the active spectrum scan.
     *
     * Performs one frequency measurement through the CC1101Manager.
     *
     * @return true when a complete sweep has just finished.
     * @return false while the sweep is still in progress.
     */
    bool update();

    /**
     * @brief Returns whether the scanner feature is available.
     *
     * @return true if the CC1101Manager is running.
     * @return false otherwise.
     */
    bool isAvailable() const;

    /**
     * @brief Returns whether scanning is active.
     *
     * @return true if scanning.
     * @return false otherwise.
     */
    bool isRunning() const;

    /**
     * @brief Sets the center frequency.
     *
     * @param frequencyMHz Center frequency in MHz.
     */
    void setCenterFrequency(float frequencyMHz);

    /**
     * @brief Returns the center frequency.
     *
     * @return Center frequency in MHz.
     */
    float getCenterFrequency() const;

    /**
     * @brief Sets the scan range around the center frequency.
     *
     * @param rangeMHz Range on either side of the center in MHz.
     */
    void setRange(float rangeMHz);

    /**
     * @brief Returns the configured scan range.
     *
     * @return Range in MHz.
     */
    float getRange() const;

    /**
     * @brief Returns the scan start frequency.
     *
     * @return Start frequency in MHz.
     */
    float getStartFrequency() const;

    /**
     * @brief Returns the scan end frequency.
     *
     * @return End frequency in MHz.
     */
    float getEndFrequency() const;

    /**
     * @brief Returns the latest RSSI results.
     *
     * @return Pointer to the RSSI result array.
     */
    const int16_t* getResults() const;

    /**
     * @brief Returns the number of scan points.
     *
     * @return Number of RSSI measurements.
     */
    uint8_t getResultCount() const;

    /**
     * @brief Returns the frequency of the strongest measured signal.
     *
     * Searches the latest completed spectrum sweep for the highest RSSI
     * value and converts its scan index into the corresponding frequency.
     *
     * @return Frequency of the strongest measured signal in MHz.
     */
    float getPeakFrequency() const;

    /**
     * @brief Returns the RSSI of the strongest measured signal.
     *
     * @return Strongest RSSI value from the latest spectrum sweep in dBm.
     */
    int16_t getPeakRSSI() const;

private:
    /** @brief Application services used by the scanner. */
    Services& services;

    /** @brief Indicates whether scanning is active. */
    bool running = false;

    /** @brief Center frequency in MHz. */
    float centerFrequencyMHz = 433.92f;

    /** @brief Range on either side of the center frequency in MHz. */
    float rangeMHz = 0.5f;

    /** @brief Latest RSSI measurements. */
    int16_t results[CC1101Manager::SCAN_POINT_COUNT] = {};
};