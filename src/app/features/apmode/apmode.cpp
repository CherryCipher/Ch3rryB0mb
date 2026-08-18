#include "apmode.h"
#include "../../../services/services.h"

/**
 * @brief Constructs the AP Mode feature.
 *
 * @param services Reference to the shared application services.
 */
APMode::APMode(Services& services)
    : services(services)
{
}

/**
 * @brief Starts Access Point mode.
 *
 * Starts the WiFi access point and embedded web server.
 *
 * @return true if AP mode started successfully.
 * @return false if one of the required services failed to start.
 */
bool APMode::start()
{
    services.logger.info("AP Mode Activated: Starting Access Point");

    if (!services.wifi.startAP(config))
    {
        services.logger.error("Failed to start Access Point.");
        return false;
    }

    if (!services.web.start())
    {
        services.logger.error("Failed to start WebServerManager.");

        services.wifi.stopAP();

        return false;
    }

    services.logger.info("Success: Webserver started.");
    services.logger.info("Connect and navigate to: http://192.168.4.1");

    return true;
}

/**
 * @brief Stops Access Point mode.
 */
void APMode::stop()
{
    services.logger.info("Stopping AP Mode...");

    services.web.stop();
    services.wifi.stopAP();

    services.logger.info("AP Mode stopped.");
}

/**
 * @brief Returns whether AP Mode is currently running.
 *
 * Queries the WiFiManager through the shared Services container.
 *
 * @return true if the Access Point is currently active.
 * @return false otherwise.
 */
bool APMode::isRunning() const
{
    return services.wifi.isAPRunning();
}

/**
 * @brief Returns the current Access Point configuration.
 *
 * Provides read-only access to the WiFiAPConfig owned by APMode.
 *
 * @return Constant reference to the current WiFiAPConfig.
 */
const WiFiAPConfig& APMode::getConfig() const
{
    return config;
}

/**
 * @brief Updates the Access Point configuration.
 *
 * Stores the supplied configuration for use the next time AP Mode
 * starts the WiFi access point.
 *
 * @param newConfig New Access Point configuration.
 */
void APMode::setConfig(const WiFiAPConfig& newConfig)
{
    config = newConfig;
}

/**
 * @brief Returns the current Access Point IP address.
 *
 * Retrieves the IP address from the WiFiManager and converts it to
 * an Arduino String for display purposes.
 *
 * @return Access Point IP address as a String.
 */
String APMode::getIP() const
{
    return services.wifi.getAPIP().toString();
}