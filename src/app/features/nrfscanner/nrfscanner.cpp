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
 * Resets the scanner position and enables continuous scanning.
 */
void NRFScanner::start()
{
    if (running)
        return;

    resetScanPosition();

    lastScan = 0;
    running = true;

    services.logger.info("NRF Scanner started.");
}

/**
 * @brief Stops continuous NRF scanning.
 *
 * Stops new measurements while keeping the latest results available.
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
 * Performs one small scan step when scanning is active and the minimum
 * scan interval has elapsed.
 */
void NRFScanner::update()
{
    if (!running)
        return;

    uint32_t now = millis();

    if (lastScan != 0 && now - lastScan < SCAN_INTERVAL_MS)
        return;

    lastScan = now;

    if (!scanStep())
    {
        services.logger.error("NRF Scanner: Scan step failed.");
        stop();
    }
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
    resetScanPosition();
}

/**
 * @brief Returns the current scanner operating mode.
 *
 * @return Current scanner operating mode.
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

    if (mode == ScanMode::NrfChannel)
        resetScanPosition();

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
 * @brief Sets the selected Wi-Fi channel.
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

    if (mode == ScanMode::WifiBand)
        resetScanPosition();

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
 * @return Pointer to the Wi-Fi band result array.
 */
const uint8_t* NRFScanner::getWifiResults() const
{
    return wifiResults;
}

/**
 * @brief Returns the first NRF24 channel represented by the Wi-Fi results.
 *
 * @return First NRF24 channel in the Wi-Fi result array.
 */
uint8_t NRFScanner::getWifiStartChannel() const
{
    return wifiStartChannel;
}

/**
 * @brief Performs one incremental RF scan step.
 *
 * Full spectrum and Wi-Fi scans process one NRF24 channel per call.
 * Individual NRF channel mode samples the selected channel once per call.
 *
 * @return true if the scan step completed successfully.
 * @return false otherwise.
 */
bool NRFScanner::scanStep()
{
    uint8_t activity = 0;

    switch (mode)
    {
        case ScanMode::FullSpectrum:
        {
            if (!services.nrf.scanNrfChannel(currentScanChannel, activity, SAMPLES_PER_STEP))
                return false;

            spectrumResults[currentScanChannel] = activity;

            currentScanChannel++;

            if (currentScanChannel >= NRFManager::NRF_CHANNEL_COUNT)
                currentScanChannel = 0;

            return true;
        }

        case ScanMode::NrfChannel:
        {
            return services.nrf.scanNrfChannel(
                nrfChannel,
                nrfChannelActivity,
                SAMPLES_PER_STEP
            );
        }

        case ScanMode::WifiBand:
        {
            uint8_t endChannel = wifiStartChannel + NRFManager::WIFI_SCAN_WIDTH;

            if (currentScanChannel < wifiStartChannel || currentScanChannel >= endChannel)
                currentScanChannel = wifiStartChannel;

            if (!services.nrf.scanNrfChannel(currentScanChannel, activity, SAMPLES_PER_STEP))
                return false;

            wifiResults[currentScanChannel - wifiStartChannel] = activity;

            currentScanChannel++;

            if (currentScanChannel >= endChannel)
                currentScanChannel = wifiStartChannel;

            return true;
        }
    }

    return false;
}

/**
 * @brief Resets the current scan position.
 *
 * Selects the appropriate starting NRF24 channel for the current
 * operating mode.
 */
void NRFScanner::resetScanPosition()
{
    switch (mode)
    {
        case ScanMode::FullSpectrum:
            currentScanChannel = 0;
            break;

        case ScanMode::NrfChannel:
            currentScanChannel = nrfChannel;
            break;

        case ScanMode::WifiBand:
        {
            uint8_t centerChannel = 12 + ((wifiChannel - 1) * 5);

            wifiStartChannel = centerChannel - (NRFManager::WIFI_SCAN_WIDTH / 2);
            currentScanChannel = wifiStartChannel;

            break;
        }
    }
}