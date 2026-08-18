#include "nrfscanner.h"
#include "../../../services/services.h"

/**
 * @brief Constructs the NRF Scanner feature.
 *
 * @param services Reference to the shared application services.
 */
NRFScanner::NRFScanner(Services& services) : services(services)
{
}

/**
 * @brief Starts continuous NRF scanning.
 *
 * Enables scanning and resets the scan timer so the first scan
 * is performed immediately during the next update.
 */
void NRFScanner::start()
{
    if (running)
        return;

    running = true;
    lastScan = 0;

    services.logger.info("NRF Scanner started.");
}

/**
 * @brief Stops continuous NRF scanning.
 *
 * Stops new scans while preserving the latest scan results.
 */
void NRFScanner::stop()
{
    if (!running)
        return;

    running = false;

    services.logger.info("NRF Scanner stopped.");
}

/**
 * @brief Updates the NRF Scanner feature.
 *
 * Performs a scan when continuous scanning is active and the configured
 * scan interval has elapsed.
 */
void NRFScanner::update()
{
    if (!running)
        return;

    uint32_t now = millis();

    if (lastScan != 0 && now - lastScan < scanInterval)
        return;

    lastScan = now;

    if (!scan())
        services.logger.error("NRF Scanner: Scan failed.");
}

/**
 * @brief Returns whether continuous scanning is active.
 *
 * @return true if scanning is active.
 * @return false otherwise.
 */
bool NRFScanner::isRunning() const
{
    return running;
}

/**
 * @brief Sets the scanner operating mode.
 *
 * @param mode Scan mode to use.
 */
void NRFScanner::setMode(ScanMode mode)
{
    this->mode = mode;
}

/**
 * @brief Returns the current scanner operating mode.
 *
 * @return Current ScanMode.
 */
NRFScanner::ScanMode NRFScanner::getMode() const
{
    return mode;
}

/**
 * @brief Sets the selected NRF24 channel.
 *
 * @param channel NRF24 channel between 0 and 125.
 *
 * @return true if the channel was accepted.
 * @return false otherwise.
 */
bool NRFScanner::setNrfChannel(uint8_t channel)
{
    if (channel >= NRFManager::NRF_CHANNEL_COUNT)
        return false;

    nrfChannel = channel;

    return true;
}

/**
 * @brief Returns the selected NRF24 channel.
 *
 * @return Selected NRF24 channel.
 */
uint8_t NRFScanner::getNrfChannel() const
{
    return nrfChannel;
}

/**
 * @brief Sets the selected 2.4 GHz Wi-Fi channel.
 *
 * @param channel Wi-Fi channel between 1 and 13.
 *
 * @return true if the channel was accepted.
 * @return false otherwise.
 */
bool NRFScanner::setWifiChannel(uint8_t channel)
{
    if (channel < 1 || channel > NRFManager::WIFI_CHANNEL_COUNT)
        return false;

    wifiChannel = channel;

    return true;
}

/**
 * @brief Returns the selected Wi-Fi channel.
 *
 * @return Selected Wi-Fi channel.
 */
uint8_t NRFScanner::getWifiChannel() const
{
    return wifiChannel;
}

/**
 * @brief Sets the interval between scans.
 *
 * @param interval Scan interval in milliseconds.
 */
void NRFScanner::setScanInterval(uint32_t interval)
{
    scanInterval = interval;
}

/**
 * @brief Returns the configured scan interval.
 *
 * @return Scan interval in milliseconds.
 */
uint32_t NRFScanner::getScanInterval() const
{
    return scanInterval;
}

/**
 * @brief Returns the latest full spectrum scan results.
 *
 * @return Pointer to the full spectrum result array.
 */
const uint8_t* NRFScanner::getSpectrumResults() const
{
    return spectrumResults;
}

/**
 * @brief Returns the latest activity value for the selected NRF24 channel.
 *
 * @return Activity percentage from 0 to 100.
 */
uint8_t NRFScanner::getNrfChannelActivity() const
{
    return nrfChannelActivity;
}

/**
 * @brief Returns the latest Wi-Fi band scan results.
 *
 * @return Pointer to the Wi-Fi scan result array.
 */
const uint8_t* NRFScanner::getWifiResults() const
{
    return wifiResults;
}

/**
 * @brief Returns the first NRF24 channel represented by the Wi-Fi results.
 *
 * @return First NRF24 channel in the Wi-Fi scan result array.
 */
uint8_t NRFScanner::getWifiStartChannel() const
{
    return wifiStartChannel;
}

/**
 * @brief Performs one scan using the current scanner mode.
 *
 * Delegates RF measurements to NRFManager and stores the latest
 * scan results inside the feature.
 *
 * @return true if the scan completed successfully.
 * @return false otherwise.
 */
bool NRFScanner::scan()
{
    switch (mode)
    {
        case ScanMode::FullSpectrum:
            return services.nrf.scanSpectrum(spectrumResults);

        case ScanMode::NrfChannel:
            return services.nrf.scanNrfChannel(nrfChannel, nrfChannelActivity);

        case ScanMode::WifiBand:
            return services.nrf.scanWifiChannel(wifiChannel, wifiResults, wifiStartChannel);
    }

    return false;
}