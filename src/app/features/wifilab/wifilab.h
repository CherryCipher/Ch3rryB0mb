#pragma once

#include "../../../services/wifi/wifimanager.h"

class Services;

/**
 * @class WiFiLab
 * @brief Controls the Ch3rryB0mb Wi-Fi Lab feature.
 *
 * WiFiLab provides the application logic for Wi-Fi discovery
 * and interaction with discovered wireless networks.
 *
 * WiFiLab does not communicate directly with the ESP32 Wi-Fi library.
 * All Wi-Fi operations are performed through the WiFiManager provided
 * by the shared Services container.
 */
class WiFiLab
{
public:
    /**
     * @brief Constructs the Wi-Fi Lab feature.
     *
     * @param services Reference to the shared application services.
     */
    WiFiLab(Services& services);

    /**
     * @brief Scans for nearby Wi-Fi networks.
     *
     * Requests the WiFiManager to perform a network scan and store
     * the discovered Access Points.
     *
     * @return true if the scan completed successfully.
     * @return false if the scan failed.
     */
    bool scan();

    /**
     * @brief Returns the number of discovered Wi-Fi networks.
     *
     * @return Number of networks found during the most recent scan.
     */
    int getNetworkCount() const;

    /**
     * @brief Returns a discovered Wi-Fi network.
     *
     * @param index Index of the network in the scan results.
     * @return Constant reference to the requested WiFiNetwork.
     */
    const WiFiNetwork& getNetwork(int index) const;

    /**
     * @brief Shuts down Wi-Fi resources used by WiFi Lab.
     *
     * Stops Wi-Fi activity and disables the Wi-Fi subsystem so its runtime
     * resources are released when leaving the WiFi Lab feature.
     *
     * Wi-Fi can be initialized again when a new scan is started.
     */
    void shutdown();

private:
    /**
     * @brief Reference to the shared application services.
     */
    Services& services;
};