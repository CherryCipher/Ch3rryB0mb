/**
 * @file cc1101scanner.cpp
 * @brief Implementation of the CC1101 Sub-GHz scanner feature.
 */

#include "cc1101scanner.h"

/**
 * @brief Constructs a new CC1101Scanner.
 *
 * @param services Reference to the application's Services container.
 */
CC1101Scanner::CC1101Scanner(Services& services)
    : services(services)
{
}

/**
 * @brief Starts continuous Sub-GHz scanning.
 *
 * Configures the CC1101Manager with the current frequency range and
 * starts an incremental spectrum scan.
 *
 * @return true if scanning started successfully.
 * @return false otherwise.
 */
bool CC1101Scanner::start()
{
    if (running) return true;

    if (!isAvailable())
    {
        services.logger.error("CC1101Scanner unavailable.");
        return false;
    }

    if (!services.cc1101.startScan(
        getStartFrequency(),
        getEndFrequency()
    ))
    {
        services.logger.error("Failed to start CC1101 scan.");
        return false;
    }

    running = true;

    services.logger.info("CC1101Scanner started.");

    return true;
}

/**
 * @brief Stops continuous Sub-GHz scanning.
 *
 * @return true when scanning has stopped.
 */
bool CC1101Scanner::stop()
{
    if (!running) return true;

    services.cc1101.stopScan();

    running = false;

    services.logger.info("CC1101Scanner stopped.");

    return true;
}

/**
 * @brief Updates the active spectrum scan.
 *
 * Performs one frequency measurement. A true return value indicates
 * that all scan points have been measured and a complete graph can
 * be rendered.
 *
 * @return true when a complete sweep has finished.
 * @return false otherwise.
 */
bool CC1101Scanner::update()
{
    if (!running || !isAvailable()) return false;

    return services.cc1101.updateScan(results);
}

/**
 * @brief Returns whether the scanner feature is available.
 *
 * @return true if the CC1101Manager is running.
 * @return false otherwise.
 */
bool CC1101Scanner::isAvailable() const
{
    return services.cc1101.isRunning();
}

/**
 * @brief Returns whether scanning is active.
 *
 * @return true if scanning.
 * @return false otherwise.
 */
bool CC1101Scanner::isRunning() const
{
    return running;
}

/**
 * @brief Sets the center frequency.
 *
 * @param frequencyMHz Center frequency in MHz.
 */
void CC1101Scanner::setCenterFrequency(float frequencyMHz)
{
    centerFrequencyMHz = frequencyMHz;
}

/**
 * @brief Returns the center frequency.
 *
 * @return Center frequency in MHz.
 */
float CC1101Scanner::getCenterFrequency() const
{
    return centerFrequencyMHz;
}

/**
 * @brief Sets the scan range.
 *
 * @param rangeMHz Range on either side of the center frequency in MHz.
 */
void CC1101Scanner::setRange(float rangeMHz)
{
    this->rangeMHz = rangeMHz;
}

/**
 * @brief Returns the configured scan range.
 *
 * @return Range in MHz.
 */
float CC1101Scanner::getRange() const
{
    return rangeMHz;
}

/**
 * @brief Returns the scan start frequency.
 *
 * @return Start frequency in MHz.
 */
float CC1101Scanner::getStartFrequency() const
{
    return centerFrequencyMHz - rangeMHz;
}

/**
 * @brief Returns the scan end frequency.
 *
 * @return End frequency in MHz.
 */
float CC1101Scanner::getEndFrequency() const
{
    return centerFrequencyMHz + rangeMHz;
}

/**
 * @brief Returns the latest RSSI results.
 *
 * @return Pointer to the RSSI result array.
 */
const int16_t* CC1101Scanner::getResults() const
{
    return results;
}

/**
 * @brief Returns the number of scan points.
 *
 * @return Number of RSSI measurements.
 */
uint8_t CC1101Scanner::getResultCount() const
{
    return CC1101Manager::SCAN_POINT_COUNT;
}

/**
 * @brief Returns the frequency of the strongest measured signal.
 *
 * Searches the latest spectrum sweep for the highest RSSI value and
 * calculates the frequency represented by that measurement point.
 *
 * @return Frequency of the strongest measured signal in MHz.
 */
float CC1101Scanner::getPeakFrequency() const
{
    uint8_t peakIndex = 0;

    for (uint8_t i = 1; i < CC1101Manager::SCAN_POINT_COUNT; i++)
    {
        if (results[i] > results[peakIndex])
            peakIndex = i;
    }

    const float stepMHz =
        (getEndFrequency() - getStartFrequency()) /
        static_cast<float>(CC1101Manager::SCAN_POINT_COUNT - 1);

    return getStartFrequency() + (stepMHz * peakIndex);
}

/**
 * @brief Returns the RSSI of the strongest measured signal.
 *
 * Searches the latest spectrum sweep for the highest RSSI value.
 *
 * @return Strongest RSSI value in dBm.
 */
int16_t CC1101Scanner::getPeakRSSI() const
{
    int16_t peakRSSI = results[0];

    for (uint8_t i = 1; i < CC1101Manager::SCAN_POINT_COUNT; i++)
    {
        if (results[i] > peakRSSI)
            peakRSSI = results[i];
    }

    return peakRSSI;
}