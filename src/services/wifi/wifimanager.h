/**
 * @file wifimanager.h
 * @brief Wi-Fi service interface for Ch3rryB0mb.
 */

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
     */
    void stopAP();

    /**
     * @brief Returns whether the Access Point is currently running.
     *
     * @return true when AP mode is active.
     */
    bool isAPRunning() const;

    /**
     * @brief Returns the IP address of the Access Point.
     *
     * Usually this will be 192.168.4.1.
     *
     * @return Access Point IP address.
     */
    IPAddress getAPIP() const;

    /**
     * @brief Scans for nearby Wi-Fi networks.
     *
     * Performs a Wi-Fi network scan and stores the discovered Access Points
     * internally for later retrieval.
     *
     * @return true if the scan completed successfully.
     * @return false if the scan failed.
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
     * @brief Starts a Station mode connection to a Wi-Fi network.
     *
     * Starts the connection process without waiting for it to complete.
     * Connection state can be checked using isConnected().
     *
     * @param ssid SSID of the network.
     * @param password Password of the network. May be empty for open networks.
     *
     * @return true if the connection attempt was started.
     * @return false if the supplied SSID is empty.
     */
    bool connect(const String& ssid, const String& password);

    /**
     * @brief Disconnects the Station interface from the current Wi-Fi network.
     *
     * The Wi-Fi subsystem remains enabled so another connection or scan can
     * be started without fully reinitializing the radio.
     */
    void disconnect();

    /**
     * @brief Returns whether the Station interface is connected.
     *
     * @return true when connected to a Wi-Fi network.
     */
    bool isConnected() const;

    /**
     * @brief Returns the SSID of the currently connected Wi-Fi network.
     *
     * @return Connected network SSID.
     */
    String getConnectedSSID() const;

    /**
     * @brief Returns the Station interface IP address.
     *
     * @return Local IP address assigned to the Station interface.
     */
    IPAddress getLocalIP() const;

    /**
     * @brief Stops the WiFiManager and disables all Wi-Fi functionality.
     *
     * @return true when Wi-Fi has been shut down.
     */
    bool stop();

private:
    /**
     * @brief Maximum number of Wi-Fi scan results stored by the manager.
     */
    static constexpr int MAX_SCAN_RESULTS = 30;

    /**
     * @brief Networks discovered during the most recent Wi-Fi scan.
     *
     * Allocated dynamically when a scan is performed to prevent the scan
     * cache from permanently consuming static DRAM.
     */
    WiFiNetwork* networks = nullptr;

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