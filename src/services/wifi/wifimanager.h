#pragma once

#include <WiFi.h>

#include "WiFiConfig.h"
#include "../logger/logger.h"

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
     * @brief Stops the WiFiManager and disables all Wi-Fi functionality.
     *
     */
    bool stop();

private:
    /**
     * @brief Reference to the application's Logger instance.
     *
     * Used for logging messages related to Wi-Fi operations.
     */
    Logger& logger;
};