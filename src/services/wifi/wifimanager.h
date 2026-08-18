#pragma once

#include <WiFi.h>

#include "WiFiConfig.h"
#include "../logger/logger.h"

/**
 * @struct WiFiNetwork
 * @brief Contains information about a discovered Wi-Fi network.
 *
 * Represents a single Access Point discovered during a Wi-Fi scan.
 * The structure contains the network information required by
 * Ch3rryB0mb features without exposing the ESP32 WiFi scanning API.
 */
struct WiFiNetwork
{
    String ssid;
    String bssid;
    int32_t rssi;
    uint8_t channel;
    wifi_auth_mode_t encryption;
};


/******************************************************************************
 * WiFiManager
 *
 * Responsible for all Wi-Fi functionality within Ch3rryB0mb.
 *
 * Features
 * --------
 * - Access Point (AP) mode
 * - Station (Client) mode
 * - Wi-Fi Scanning
 * - Captive Portal
 * - Web Dashboard
 * - Future wireless extensions
 *
 * Design
 * ------
 * Applications should NEVER communicate directly with the ESP32 Wi-Fi
 * library (WiFi.h). All Wi-Fi operations should be performed through the
 * WiFiManager.
 *
 *
 ******************************************************************************/
class WiFiManager
{
public:
    /**
     * @brief Constructs a new WiFiManager.
     *
     * @param logger Reference to the application's Logger.
     */
    explicit WiFiManager(Logger& logger);

    /**
     * @brief Initializes the WiFiManager.
     *
     * This prepares the manager for use but does not enable any Wi-Fi mode.
     * Applications should explicitly start the desired mode (AP, STA, etc.).
     *
     *
     * @return true if initialization succeeded.
     */
    bool start();

    /**
     * @brief Starts the ESP32 in Access Point mode.
     *
     * Creates a configurable Wi-Fi Access Point using the supplied settings.
     *
     * @param config Access Point configuration.
     * @return true if the AP started successfully.
     * @return false otherwise.
     */
    bool startAP(const WiFiAPConfig& config);

    /**
     * @brief Stops the Access Point.
     *
     * Disconnects all connected clients and disables AP mode.
     *
     */
    void stopAP();

    /**
     * @brief Returns whether the Access Point is currently running.
     *
     */
    bool isAPRunning() const;

    /**
     * @brief Returns the IP address of the Access Point.
     *
     * Usually this will be 192.168.4.1.
     */
    IPAddress getAPIP() const;

    /**
     * @brief Scans for nearby Wi-Fi networks.
     *
     * Performs a Wi-Fi network scan and stores the discovered access points
     * internally for later retrieval.
     *
     * @return true if the scan completed successfully, otherwise false.
     */
    bool scanNetworks();

    /**
     * @brief Returns the number of stored Wi-Fi scan results.
     *
     * @return Number of networks discovered during the most recent scan.
     */
    int getNetworkCount() const;

    /**
     * @brief Returns a Wi-Fi network from the stored scan results.
     *
     * @param index Index of the requested network.
     * @return Constant reference to the requested WiFiNetwork.
     */
    const WiFiNetwork& getNetwork(int index) const;

    /**
     * @brief Stops the WiFiManager and disables all Wi-Fi functionality.
     *
     */
    bool stop();

private:
    /**
     * @brief Maximum number of Wi-Fi scan results stored by the manager.
     */
    static constexpr int MAX_SCAN_RESULTS = 30;

    /**
     * @brief Networks discovered during the most recent Wi-Fi scan.
     */
    WiFiNetwork networks[MAX_SCAN_RESULTS];

    /**
     * @brief Number of valid networks currently stored.
     */
    int networkCount = 0;

    /**
     * @brief Reference to the application's Logger instance.
     *
     * Used for logging messages related to Wi-Fi operations.
     */
    Logger& logger;
};