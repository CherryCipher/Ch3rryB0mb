/**
 * @file wifilab.h
 * @brief Wi-Fi Lab feature interface.
 */

#pragma once

#include "../../../services/wifi/wifimanager.h"

class Services;

/**
 * @class WiFiLab
 * @brief Controls the Ch3rryB0mb Wi-Fi Lab feature.
 *
 * WiFiLab provides the application logic for Wi-Fi discovery,
 * network selection and Station connections.
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
    explicit WiFiLab(Services& services);

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
     * @brief Selects a discovered Wi-Fi network.
     *
     * Copies the selected network information so it remains available
     * independently of the WiFiManager scan result cache.
     *
     * @param index Index of the network to select.
     *
     * @return true if the network was selected.
     * @return false if the index is invalid.
     */
    bool selectNetwork(int index);

    /**
     * @brief Returns whether a Wi-Fi network has been selected.
     *
     * @return true when a network is selected.
     */
    bool hasSelectedNetwork() const;

    /**
     * @brief Returns the selected Wi-Fi network.
     *
     * @return Constant reference to the selected WiFiNetwork.
     */
    const WiFiNetwork& getSelectedNetwork() const;

    /**
     * @brief Starts a connection to the selected Wi-Fi network.
     *
     * Starts the connection through WiFiManager without blocking while
     * association is in progress.
     *
     * @param password Password for the selected network.
     *
     * @return true if the connection attempt was started.
     * @return false if no network is selected or the attempt could not start.
     */
    bool connect(const String& password);

    /**
     * @brief Disconnects from the currently connected Wi-Fi network.
     */
    void disconnect();

    /**
     * @brief Returns whether Wi-Fi is currently connected.
     *
     * @return true when the Station interface is connected.
     */
    bool isConnected() const;

    /**
     * @brief Returns the SSID of the active Wi-Fi connection.
     *
     * Retrieves the currently connected network name through WiFiManager.
     *
     * @return SSID of the active Station connection.
     */
    String getConnectedSSID() const;

    /**
     * @brief Returns the signal strength of the active Wi-Fi connection.
     *
     * @return Current Wi-Fi signal strength in dBm.
     */
    int32_t getRSSI() const;

    /**
     * @brief Returns the Station interface IP address.
     *
     * @return Local Station IP address.
     */
    IPAddress getLocalIP() const;

    /**
     * @brief Shuts down Wi-Fi resources used by WiFi Lab.
     *
     * Stops Wi-Fi activity and disables the Wi-Fi subsystem so its runtime
     * resources are released when leaving the WiFi Lab feature.
     *
     * Wi-Fi can be initialized again when a new scan or connection is started.
     */
    void shutdown();

private:
    /**
     * @brief Reference to the shared application services.
     */
    Services& services;

    /**
     * @brief Copy of the currently selected Wi-Fi network.
     *
     * Stored independently from the WiFiManager scan cache so the selection
     * remains valid if scan results are later released or replaced.
     */
    WiFiNetwork selectedNetwork;

    /**
     * @brief Indicates whether a Wi-Fi network has been selected.
     */
    bool networkSelected = false;
};