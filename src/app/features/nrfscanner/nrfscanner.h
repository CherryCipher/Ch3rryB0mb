#pragma once

#include <Arduino.h>
#include "../../../services/nrf/nrfmanager.h"

class Services;

/**
 * @class NRFScanner
 * @brief Controls the Ch3rryB0mb NRF Scanner feature.
 *
 * NRFScanner provides the application logic for continuously scanning
 * RF activity using the NRF24 radio.
 *
 * The feature supports full spectrum scanning, individual NRF24 channel
 * monitoring and Wi-Fi channel band scanning.
 *
 * NRFScanner does not communicate directly with the RF24 library.
 * All radio operations are performed through the NRFManager provided
 * by the shared Services container.
 */
class NRFScanner
{
public:
    /**
     * @enum ScanMode
     * @brief Available NRF Scanner operating modes.
     */
    enum class ScanMode : uint8_t
    {
        FullSpectrum = 0,
        NrfChannel,
        WifiBand
    };

    /**
     * @brief Constructs the NRF Scanner feature.
     *
     * @param services Reference to the shared application services.
     */
    explicit NRFScanner(Services& services);

    /**
     * @brief Starts continuous NRF scanning.
     *
     * Enables scanning using the currently selected scan mode
     * and channel configuration.
     */
    void start();

    /**
     * @brief Stops continuous NRF scanning.
     *
     * Stops new scans from being performed. Existing scan results
     * remain available.
     */
    void stop();

    /**
     * @brief Updates the NRF Scanner feature.
     *
     * Performs a new scan when scanning is active and the configured
     * scan interval has elapsed.
     *
     * This function should be called regularly from the application
     * update loop.
     */
    void update();

    /**
     * @brief Returns whether continuous scanning is active.
     *
     * @return true if scanning is active.
     * @return false otherwise.
     */
    bool isRunning() const;

    /**
     * @brief Sets the scanner operating mode.
     *
     * @param mode Scan mode to use.
     */
    void setMode(ScanMode mode);

    /**
     * @brief Returns the current scanner operating mode.
     *
     * @return Current ScanMode.
     */
    ScanMode getMode() const;

    /**
     * @brief Sets the selected NRF24 channel.
     *
     * Valid NRF24 channels range from 0 through 125.
     *
     * @param channel NRF24 channel.
     *
     * @return true if the channel was accepted.
     * @return false if the channel is invalid.
     */
    bool setNrfChannel(uint8_t channel);

    /**
     * @brief Returns the selected NRF24 channel.
     *
     * @return Selected NRF24 channel.
     */
    uint8_t getNrfChannel() const;

    /**
     * @brief Sets the selected 2.4 GHz Wi-Fi channel.
     *
     * Valid Wi-Fi channels range from 1 through 13.
     *
     * @param channel Wi-Fi channel.
     *
     * @return true if the channel was accepted.
     * @return false if the channel is invalid.
     */
    bool setWifiChannel(uint8_t channel);

    /**
     * @brief Returns the selected Wi-Fi channel.
     *
     * @return Selected Wi-Fi channel.
     */
    uint8_t getWifiChannel() const;

    /**
     * @brief Sets the interval between scans.
     *
     * @param interval Scan interval in milliseconds.
     */
    void setScanInterval(uint32_t interval);

    /**
     * @brief Returns the configured scan interval.
     *
     * @return Scan interval in milliseconds.
     */
    uint32_t getScanInterval() const;

    /**
     * @brief Returns the latest full spectrum scan results.
     *
     * The returned array contains one activity percentage for each
     * NRF24 channel from 0 through 125.
     *
     * @return Pointer to the full spectrum result array.
     */
    const uint8_t* getSpectrumResults() const;

    /**
     * @brief Returns the latest activity value for the selected NRF24 channel.
     *
     * @return Activity percentage from 0 to 100.
     */
    uint8_t getNrfChannelActivity() const;

    /**
     * @brief Returns the latest Wi-Fi band scan results.
     *
     * The returned array contains activity percentages across the
     * NRF24 channels overlapping the selected Wi-Fi channel.
     *
     * @return Pointer to the Wi-Fi scan result array.
     */
    const uint8_t* getWifiResults() const;

    /**
     * @brief Returns the first NRF24 channel represented by the Wi-Fi results.
     *
     * @return First NRF24 channel in the Wi-Fi scan result array.
     */
    uint8_t getWifiStartChannel() const;

private:
    /**
     * @brief Reference to the shared application services.
     */
    Services& services;

    /**
     * @brief Current scanner operating mode.
     */
    ScanMode mode = ScanMode::FullSpectrum;

    /**
     * @brief Selected NRF24 channel.
     */
    uint8_t nrfChannel = 0;

    /**
     * @brief Selected Wi-Fi channel.
     */
    uint8_t wifiChannel = 1;

    /**
     * @brief Latest full spectrum scan results.
     */
    uint8_t spectrumResults[NRFManager::NRF_CHANNEL_COUNT] = {0};

    /**
     * @brief Latest activity percentage for the selected NRF24 channel.
     */
    uint8_t nrfChannelActivity = 0;

    /**
     * @brief Latest Wi-Fi band scan results.
     */
    uint8_t wifiResults[NRFManager::WIFI_SCAN_WIDTH] = {0};

    /**
     * @brief First NRF24 channel represented by the Wi-Fi scan results.
     */
    uint8_t wifiStartChannel = 0;

    /**
     * @brief Indicates whether continuous scanning is active.
     */
    bool running = false;

    /**
     * @brief Timestamp of the most recent scan.
     */
    uint32_t lastScan = 0;

    /**
     * @brief Interval between scans in milliseconds.
     */
    uint32_t scanInterval = 100;

    /**
     * @brief Performs one scan using the current scanner mode.
     *
     * @return true if the scan completed successfully.
     * @return false otherwise.
     */
    bool scan();
};