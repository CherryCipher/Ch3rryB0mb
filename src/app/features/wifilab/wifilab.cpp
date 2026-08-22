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