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

    WiFiAPConfig config;

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