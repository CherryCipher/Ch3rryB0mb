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
 * Scanning is performed incrementally so long-running spectrum scans
 * do not block the application UI.
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
     * Resets the scan position and enables continuous scanning
     * using the currently selected scan mode.
     */
    void start();

    /**
     * @brief Stops continuous NRF scanning.
     *
     * Stops new RF measurements while preserving the latest results.
     */
    void stop();

    /**
     * @brief Updates the NRF Scanner feature.
     *
     * Performs at most one small RF scan step per update. This keeps
     * scanning cooperative and prevents the UI from being blocked.
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
     * Resets the scan position when the mode changes.
     *
     * @param mode Scan mode to use.
     */
    void setMode(ScanMode mode);

    /**
     * @brief Returns the current scanner operating mode.
     *
     * @return Current scanner operating mode.
     */
    ScanMode getMode() const;

    /**
     * @brief Sets the selected NRF24 channel.
     *
     * @param channel NRF24 channel between 0 and 125.
     *
     * @return true if the channel was accepted.
     * @return false otherwise.
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
     * @param channel Wi-Fi channel between 1 and 13.
     *
     * @return true if the channel was accepted.
     * @return false otherwise.
     */
    bool setWifiChannel(uint8_t channel);

    /**
     * @brief Returns the selected Wi-Fi channel.
     *
     * @return Selected Wi-Fi channel.
     */
    uint8_t getWifiChannel() const;

    /**
     * @brief Returns the latest full spectrum scan results.
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
     * @return Pointer to the Wi-Fi band result array.
     */
    const uint8_t* getWifiResults() const;

    /**
     * @brief Returns the first NRF24 channel represented by the Wi-Fi results.
     *
     * @return First NRF24 channel in the Wi-Fi result array.
     */
    uint8_t getWifiStartChannel() const;

    /**
     * @brief Returns the peak values for the full spectrum scan.
     *
     * The returned array contains the highest recently observed activity
     * value for each NRF24 channel.
     *
     * @return Pointer to the full spectrum peak array.
     */
    const uint8_t* getSpectrumPeaks() const;

    /**
     * @brief Clears all stored peak values.
     */
    void clearPeaks();

private:
    /**
     * @brief Number of RPD samples taken during one incremental scan step.
     *
     * Keeping the sample count relatively small prevents scanning from
     * blocking UI and touch processing for noticeable periods.
     */
    static constexpr uint16_t SAMPLES_PER_STEP = 10;

    /**
     * @brief Minimum interval between scan steps in milliseconds.
     */
    static constexpr uint32_t SCAN_INTERVAL_MS = 10;

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
     * @brief NRF24 channel currently being processed by an incremental scan.
     */
    uint8_t currentScanChannel = 0;

    /**
     * @brief Latest full spectrum scan results.
     */
    uint8_t spectrumResults[NRFManager::NRF_CHANNEL_COUNT] = {0};

    /**
     * @brief Latest activity value for the selected NRF24 channel.
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
     * @brief Timestamp of the most recent scan step.
     */
    uint32_t lastScan = 0;

    /**
     * @brief Performs one incremental RF scan step.
     *
     * @return true if the scan step completed successfully.
     * @return false otherwise.
     */
    bool scanStep();

    /**
     * @brief Resets the current scan position.
     *
     * Calculates the correct starting NRF24 channel for the
     * currently selected scan mode.
     */
    void resetScanPosition();

    /**
     * @brief Peak values for each NRF24 spectrum channel.
     *
     * Peaks slowly decay over time so short RF activity remains visible
     * without permanently filling the spectrum display.
     */
    uint8_t spectrumPeaks[NRFManager::NRF_CHANNEL_COUNT] = {0};

    /**
     * @brief Amount removed from peak values after each full spectrum sweep.
     */
    static constexpr uint8_t PEAK_DECAY = 2;
};