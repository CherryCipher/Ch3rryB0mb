#pragma once

#include "../../../services/wifi/wificonfig.h"

class Services;

/**
 * @class APMode
 * @brief Controls the Ch3rryB0mb Access Point mode feature.
 *
 * APMode combines the services required to provide Access Point mode.
 *
 * Starting AP mode will:
 * - Start the WiFi access point.
 * - Start the embedded web server.
 *
 * Stopping AP mode will:
 * - Stop the embedded web server.
 * - Stop the WiFi access point.
 *
 * APMode does not own these services. It uses the shared application
 * services provided by the Services container.
 */
class APMode
{
public:
    /**
     * @brief Constructs the AP Mode feature.
     *
     * @param services Reference to the shared application services.
     */
    APMode(Services& services);

    /**
     * @brief Starts Access Point mode.
     *
     * Starts the WiFi access point followed by the embedded web server.
     * If the web server fails to start, the access point is stopped again.
     *
     * @return true if AP mode started successfully.
     * @return false if one of the required services failed to start.
     */
    bool start();

    /**
     * @brief Stops Access Point mode.
     *
     * Stops the web server first and then disables the WiFi access point.
     */
    void stop();

        /**
     * @brief Returns whether AP Mode is currently running.
     *
     * @return true if the WiFi access point is active.
     * @return false otherwise.
     */
    bool isRunning() const;

    /**
     * @brief Returns the current Access Point configuration.
     *
     * Provides read-only access to the configuration currently used
     * by AP Mode.
     *
     * @return Constant reference to the current WiFiAPConfig.
     */
    const WiFiAPConfig& getConfig() const;

    /**
     * @brief Updates the Access Point configuration.
     *
     * Replaces the configuration used by AP Mode. The new configuration
     * will be used the next time the Access Point is started.
     *
     * @param newConfig New Access Point configuration.
     */
    void setConfig(const WiFiAPConfig& newConfig);

    /**
     * @brief Returns the current Access Point IP address.
     *
     * @return Access Point IP address as a String.
     */
    String getIP() const;

private:
    /**
     * @brief Reference to the shared application services.
     */
    Services& services;

    /**
     * @brief Current Access Point configuration.
     *
     * Contains the SSID, password, channel, visibility and maximum
     * number of connected clients used when starting AP Mode.
     */
    WiFiAPConfig config;
};