/**
 * @file cc1101manager.cpp
 * @brief Implementation of the CC1101 radio manager.
 */

#include "cc1101manager.h"

#include "hardware/modulepins.h"

/**
 * @brief Constructs a new CC1101Manager.
 *
 * @param logger Reference to the application's Logger.
 * @param spiManager Reference to the application's SPIManager.
 */
CC1101Manager::CC1101Manager(Logger& logger, SPIManager& spiManager)
    : logger(logger),
      spiManager(spiManager),
      module(ModulePins::CC1101_CSN, ModulePins::CC1101_GDO0, RADIOLIB_NC, RADIOLIB_NC, spiManager.getHardwareBus()),
      radio(&module)
{
}

/**
 * @brief Initializes the CC1101 radio.
 *
 * @return true if initialization succeeded.
 * @return false otherwise.
 */
bool CC1101Manager::start()
{
    if (running) return true;

    logger.info("Starting CC1101Manager.");

    if (!spiManager.isRunning())
    {
        logger.error("SPIManager is not running.");
        return false;
    }

    const int16_t state = radio.begin();

    if (state != RADIOLIB_ERR_NONE)
    {
        logger.error("CC1101 radio not detected.");
        return false;
    }

    radio.standby();

    running = true;

    logger.info("CC1101 radio detected.");
    logger.info("CC1101Manager started.");

    return true;
}

/**
 * @brief Stops the CC1101Manager.
 *
 * Stops active scanning, restores packet mode and places the radio
 * into sleep mode.
 *
 * @return true when the manager has stopped.
 */
bool CC1101Manager::stop()
{
    if (!running) return true;

    stopScan();

    radio.packetMode();
    radio.sleep();

    running = false;

    logger.info("CC1101Manager stopped.");

    return true;
}

/**
 * @brief Returns whether the CC1101Manager is running.
 *
 * @return true if running.
 * @return false otherwise.
 */
bool CC1101Manager::isRunning() const
{
    return running;
}

/**
 * @brief Starts an incremental frequency range scan.
 *
 * Initializes the frequency range and resets the scan index.
 * No RF measurement is performed until updateScan() is called.
 *
 * @param startMHz Start frequency in MHz.
 * @param endMHz End frequency in MHz.
 *
 * @return true if the scan was initialized successfully.
 * @return false otherwise.
 */
bool CC1101Manager::startScan(float startMHz, float endMHz)
{
    if (!running)
    {
        logger.error("CC1101Manager is not running.");
        return false;
    }

    if (startMHz >= endMHz)
    {
        logger.error("Invalid CC1101 scan frequency range.");
        return false;
    }

    scanStartMHz = startMHz;
    scanEndMHz = endMHz;
    scanStepMHz = (scanEndMHz - scanStartMHz) / static_cast<float>(SCAN_POINT_COUNT - 1);
    scanIndex = 0;
    scanning = true;

    logger.info("CC1101 frequency scan started.");

    return true;
}

/**
 * @brief Performs one step of the active frequency scan.
 *
 * Exactly one frequency is measured per call. When the final point has
 * been measured, the scan index wraps back to zero so continuous
 * scanning can continue without blocking the application.
 *
 * @param results Array receiving RSSI measurements in dBm.
 *
 * @return true when a complete sweep has just finished.
 * @return false while the current sweep is still in progress.
 */
bool CC1101Manager::updateScan(int16_t results[SCAN_POINT_COUNT])
{
    if (!running || !scanning) return false;

    const float frequencyMHz = scanStartMHz + (scanStepMHz * scanIndex);

    if (!measureFrequency(frequencyMHz, results[scanIndex]))
    {
        logger.error("CC1101 frequency measurement failed.");
        stopScan();
        return false;
    }

    scanIndex++;

    if (scanIndex < SCAN_POINT_COUNT) return false;

    scanIndex = 0;

    return true;
}

/**
 * @brief Stops the active frequency scan.
 *
 * Resets scan state and returns the CC1101 to packet mode and standby.
 */
void CC1101Manager::stopScan()
{
    scanning = false;
    scanIndex = 0;

    radio.packetMode();
    radio.standby();
}

/**
 * @brief Returns whether a frequency scan is active.
 *
 * @return true if scanning.
 * @return false otherwise.
 */
bool CC1101Manager::isScanning() const
{
    return scanning;
}

/**
 * @brief Measures live RSSI at a single frequency.
 *
 * Tunes the CC1101 to the requested frequency, enters direct receive
 * mode, waits for RSSI to settle and reads the current RSSI value.
 *
 * @param frequencyMHz Frequency to measure in MHz.
 * @param rssi Reference receiving RSSI in dBm.
 *
 * @return true if the measurement succeeded.
 * @return false otherwise.
 */
bool CC1101Manager::measureFrequency(float frequencyMHz, int16_t& rssi)
{
    int16_t state = radio.setFrequency(frequencyMHz);
    if (state != RADIOLIB_ERR_NONE) return false;

    state = radio.receiveDirect();
    if (state != RADIOLIB_ERR_NONE) return false;

    delayMicroseconds(RSSI_SETTLE_US);

    rssi = static_cast<int16_t>(radio.getRSSI());

    radio.packetMode();
    radio.standby();

    return true;
}