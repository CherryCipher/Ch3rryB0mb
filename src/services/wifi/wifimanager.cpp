/**
 * @file wifimanager.cpp
 * @brief Implementation of the Ch3rryB0mb Wi-Fi service.
 */

#include "WiFiManager.h"

/**
 * @brief Constructs a new WiFiManager.
 *
 * Stores a reference to the application's Logger.
 *
 * @param logger Reference to the application's Logger.
 */
WiFiManager::WiFiManager(Logger& logger) : logger(logger)
{
}

/**
 * @brief Initializes the WiFiManager.
 *
 * Prepares the WiFiManager for use without initializing or enabling the
 * ESP32 Wi-Fi subsystem. Individual Wi-Fi features explicitly activate
 * the required Wi-Fi mode when needed.
 *
 * @return true when the manager is ready for use.
 */
bool WiFiManager::start()
{
    logger.info("WiFiManager started.");
    return true;
}

/**
 * @brief Returns if the C3B0 is in Access Point mode.
 *
 * Uses the ESP32 WiFi library to check if the device is currently in Access Point mode.
 * WIFI_AP is the mode for Access Point only, while WIFI_AP_STA is the mode for both Access Point and Station.
 *
 * @return true when AP mode is active.
 */
bool WiFiManager::isAPRunning() const
{
    return (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA);
}

/**
 * @brief Starts the ESP32 in Access Point (SoftAP) mode.
 *
 * Configures the Wi-Fi radio as an Access Point using the supplied
 * configuration and begins broadcasting the configured SSID.
 *
 * SoftAP() is a native function from the ESP32 Wi-Fi library that handles the underlying implementation of starting an AP.
 *
 * If you want the AP to serve a web page, you will need to set up a web server separately.
 * This function only starts the AP and does not handle any web server functionality.
 * Webserver is started though webservermanager, which is a separate service that can be started after the AP is running.
 *
 * @param config The Access Point configuration.
 * @return true if the Access Point started successfully.
 * @return false if the SoftAP failed to start.
 */
bool WiFiManager::startAP(const WiFiAPConfig& config)
{
    WiFi.mode(WIFI_AP);
    logger.info("WiFi mode set to WIFI_AP. Starting SoftAP");

    bool success = WiFi.softAP(
        config.ssid.c_str(),
        config.password.c_str(),
        config.channel,
        config.hidden,
        config.maxClients
    );

    logger.info("SoftAP started with SSID: " + config.ssid + ", Channel: " + String(config.channel) + ", Hidden: " + String(config.hidden) + ", Max Clients: " + String(config.maxClients));

    return success;
}

/**
 * @brief Stops the active Access Point.
 *
 * Disconnects all connected clients and stops the SoftAP. The complete
 * Wi-Fi subsystem remains managed by WiFiManager and can be shut down
 * separately using stop().
 */
void WiFiManager::stopAP()
{
    WiFi.softAPdisconnect(true);
    logger.info("SoftAP stopped. All clients disconnected.");
}

/**
 * @brief Returns the IP address of the Access Point.
 *
 * Calls the ESP32 Wi-Fi library to retrieve the IP address assigned to the Access Point interface.
 *
 * @return Access Point IP address.
 */
IPAddress WiFiManager::getAPIP() const
{
    return WiFi.softAPIP();
}

/**
 * @brief Scans for nearby Wi-Fi networks.
 *
 * Enables Station mode when required and performs a synchronous Wi-Fi scan.
 * Discovered Access Points are stored internally as WiFiNetwork objects.
 *
 * Existing scan results are cleared before starting a new scan.
 *
 * @return true if the scan completed successfully.
 * @return false if the scan failed.
 */
bool WiFiManager::scanNetworks()
{
    networkCount = 0;
    delete[] networks;
    networks = nullptr;

    wifi_mode_t currentMode = WiFi.getMode();

    if (currentMode == WIFI_OFF)
    {
        WiFi.mode(WIFI_STA);
        logger.info("WiFi mode set to WIFI_STA for network scan.");
    }
    else if (currentMode == WIFI_AP)
    {
        WiFi.mode(WIFI_AP_STA);
        logger.info("WiFi mode set to WIFI_AP_STA for network scan.");
    }

    int foundNetworks = WiFi.scanNetworks();

    if (foundNetworks < 0)
    {
        logger.error("Wi-Fi network scan failed.");
        return false;
    }

    networkCount = min(foundNetworks, MAX_SCAN_RESULTS);

    if (networkCount > 0)
    {
        networks = new WiFiNetwork[networkCount];

        if (networks == nullptr)
        {
            WiFi.scanDelete();
            networkCount = 0;
            logger.error("Failed to allocate Wi-Fi scan result cache.");
            return false;
        }
    }

    for (int i = 0; i < networkCount; i++)
    {
        networks[i].ssid = WiFi.SSID(i);
        networks[i].bssid = WiFi.BSSIDstr(i);
        networks[i].rssi = WiFi.RSSI(i);
        networks[i].channel = WiFi.channel(i);
        networks[i].encryption = WiFi.encryptionType(i);
    }

    WiFi.scanDelete();
    logger.info("Wi-Fi scan completed. Networks found: " + String(networkCount));

    return true;
}

/**
 * @brief Returns the number of stored Wi-Fi scan results.
 *
 * @return Number of networks discovered during the most recent scan.
 */
int WiFiManager::getNetworkCount() const
{
    return networkCount;
}

/**
 * @brief Returns a Wi-Fi network from the stored scan results.
 *
 * @param index Index of the requested network.
 * @return Constant reference to the requested WiFiNetwork.
 */
const WiFiNetwork& WiFiManager::getNetwork(int index) const
{
    return networks[index];
}

/**
 * @brief Starts a Station mode connection to a Wi-Fi network.
 *
 * Enables Station mode when required and starts the connection without
 * blocking while the ESP32 associates with the Access Point.
 *
 * @param ssid SSID of the network.
 * @param password Password of the network. May be empty for open networks.
 *
 * @return true if the connection attempt was started.
 * @return false if the supplied SSID is empty.
 */
bool WiFiManager::connect(const String& ssid, const String& password)
{
    if (ssid.length() == 0)
    {
        logger.error("WiFi connection failed: SSID is empty.");
        return false;
    }

    wifi_mode_t currentMode = WiFi.getMode();

    if (currentMode == WIFI_OFF)
    {
        WiFi.mode(WIFI_STA);
        logger.info("WiFi mode set to WIFI_STA for connection.");
    }
    else if (currentMode == WIFI_AP)
    {
        WiFi.mode(WIFI_AP_STA);
        logger.info("WiFi mode set to WIFI_AP_STA for connection.");
    }

    if (WiFi.status() == WL_CONNECTED)
        WiFi.disconnect(false);

    logger.info("Connecting to Wi-Fi network: " + ssid);
    WiFi.begin(ssid.c_str(), password.c_str());

    return true;
}

/**
 * @brief Disconnects the Station interface from Wi-Fi.
 *
 * Cancels any active Station connection or connection attempt while
 * keeping the Wi-Fi subsystem enabled for future scans or connections.
 */
void WiFiManager::disconnect()
{
    String ssid = WiFi.SSID();

    WiFi.disconnect(false);

    if (ssid.length() > 0)
        logger.info("Disconnected from Wi-Fi network: " + ssid);
    else
        logger.info("Wi-Fi Station disconnected.");
}

/**
 * @brief Returns whether the Station interface is connected.
 *
 * @return true when connected to a Wi-Fi network.
 */
bool WiFiManager::isConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}

/**
 * @brief Returns the SSID of the currently connected Wi-Fi network.
 *
 * @return Connected network SSID.
 */
String WiFiManager::getConnectedSSID() const
{
    if (!isConnected())
        return "";

    return WiFi.SSID();
}

/**
 * @brief Returns the Station interface IP address.
 *
 * @return Local IP address assigned to the Station interface.
 */
IPAddress WiFiManager::getLocalIP() const
{
    return WiFi.localIP();
}

/**
 * @brief Returns the signal strength of the active Station connection.
 *
 * Retrieves the RSSI reported by the ESP32 Wi-Fi subsystem for the
 * currently connected Access Point.
 *
 * @return Current Wi-Fi signal strength in dBm.
 */
int32_t WiFiManager::getRSSI() const
{
    return WiFi.RSSI();
}

/**
 * @brief Stops the WiFiManager and releases Wi-Fi resources.
 *
 * Stops any active Access Point, clears stored scan results and disables
 * the ESP32 Wi-Fi subsystem so its runtime resources become available to
 * other features.
 *
 * Wi-Fi can be activated again later by starting an Access Point or
 * performing a network scan.
 *
 * @return true when Wi-Fi has been shut down.
 */
bool WiFiManager::stop()
{
    if (isAPRunning())
        WiFi.softAPdisconnect(true);

    WiFi.scanDelete();

    delete[] networks;
    networks = nullptr;
    networkCount = 0;

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    logger.info("WiFiManager stopped. Wi-Fi subsystem released.");
    return true;
}