/**
 * @file wifilab.cpp
 * @brief Implementation of the Wi-Fi Lab feature.
 */

#include "wifilab.h"
#include "../../../services/services.h"

/**
 * @brief Constructs the Wi-Fi Lab feature.
 *
 * @param services Reference to the shared application services.
 */
WiFiLab::WiFiLab(Services& services) : services(services)
{
}

/**
 * @brief Scans for nearby Wi-Fi networks.
 *
 * Requests the WiFiManager to perform a Wi-Fi scan and store
 * the discovered Access Points.
 *
 * @return true if the scan completed successfully.
 * @return false if the scan failed.
 */
bool WiFiLab::scan()
{
    services.logger.info("WiFi Lab: Starting network scan.");

    if (!services.wifi.scanNetworks())
    {
        services.logger.error("WiFi Lab: Network scan failed.");
        return false;
    }

    services.logger.info("WiFi Lab: Network scan completed. Networks found: " + String(services.wifi.getNetworkCount()));
    return true;
}

/**
 * @brief Returns the number of discovered Wi-Fi networks.
 *
 * @return Number of networks found during the most recent scan.
 */
int WiFiLab::getNetworkCount() const
{
    return services.wifi.getNetworkCount();
}

/**
 * @brief Returns a discovered Wi-Fi network.
 *
 * @param index Index of the network in the scan results.
 * @return Constant reference to the requested WiFiNetwork.
 */
const WiFiNetwork& WiFiLab::getNetwork(int index) const
{
    return services.wifi.getNetwork(index);
}

/**
 * @brief Selects a discovered Wi-Fi network.
 *
 * Copies the selected network from the WiFiManager scan cache so it remains
 * valid if the scan results are later replaced or released.
 *
 * @param index Index of the network to select.
 *
 * @return true if the network was selected.
 * @return false if the supplied index is invalid.
 */
bool WiFiLab::selectNetwork(int index)
{
    if (index < 0 || index >= getNetworkCount())
        return false;

    selectedNetwork = getNetwork(index);
    networkSelected = true;

    services.logger.info("WiFi Lab: Selected " + selectedNetwork.ssid);
    return true;
}

/**
 * @brief Returns whether a Wi-Fi network has been selected.
 *
 * @return true when a network is selected.
 */
bool WiFiLab::hasSelectedNetwork() const
{
    return networkSelected;
}

/**
 * @brief Returns the selected Wi-Fi network.
 *
 * @return Constant reference to the selected WiFiNetwork.
 */
const WiFiNetwork& WiFiLab::getSelectedNetwork() const
{
    return selectedNetwork;
}

/**
 * @brief Starts a connection to the selected Wi-Fi network.
 *
 * @param password Password for the selected network.
 *
 * @return true if the connection attempt was started.
 * @return false if no network is selected or the attempt could not start.
 */
bool WiFiLab::connect(const String& password)
{
    if (!networkSelected)
    {
        services.logger.error("WiFi Lab: No network selected.");
        return false;
    }

    services.logger.info("WiFi Lab: Connecting to " + selectedNetwork.ssid);

    if (!services.wifi.connect(selectedNetwork.ssid, password))
    {
        services.logger.error("WiFi Lab: Failed to start Wi-Fi connection.");
        return false;
    }

    return true;
}

/**
 * @brief Disconnects from the currently connected Wi-Fi network.
 */
void WiFiLab::disconnect()
{
    services.wifi.disconnect();
    services.logger.info("WiFi Lab: Wi-Fi disconnected.");
}

/**
 * @brief Returns whether Wi-Fi is currently connected.
 *
 * @return true when the Station interface is connected.
 */
bool WiFiLab::isConnected() const
{
    return services.wifi.isConnected();
}

/**
 * @brief Returns the SSID of the active Wi-Fi connection.
 *
 * Retrieves the currently connected network name through WiFiManager.
 *
 * @return SSID of the active Station connection.
 */
String WiFiLab::getConnectedSSID() const
{
    return services.wifi.getConnectedSSID();
}

/**
 * @brief Returns the signal strength of the active Wi-Fi connection.
 *
 * @return Current Wi-Fi signal strength in dBm.
 */
int32_t WiFiLab::getRSSI() const
{
    return services.wifi.getRSSI();
}

/**
 * @brief Returns the Station interface IP address.
 *
 * @return Local Station IP address.
 */
IPAddress WiFiLab::getLocalIP() const
{
    return services.wifi.getLocalIP();
}

/**
 * @brief Shuts down Wi-Fi resources used by WiFi Lab.
 *
 * Stops Wi-Fi activity and releases the Wi-Fi subsystem when leaving
 * the feature.
 */
void WiFiLab::shutdown()
{
    services.wifi.stop();
    services.logger.info("WiFi Lab: Wi-Fi subsystem released.");
}