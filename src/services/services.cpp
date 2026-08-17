#include "Services.h"

/**
 * @brief Constructs the application's shared services.
 *
 * Every shared service is constructed exactly once and receives the
 * dependencies it requires.
 *
 * Construction order
 * ------------------
 * Logger
 *      │
 *      ▼
 * StorageManager
 *      │
 *      ▼
 * WiFiManager
 *      │
 *      ▼
 * WebServerManager
 */
Services::Services()
    : logger()
    , storage(logger)
    , wifi(logger)
    , web(storage, logger)
    , display(logger)
{
}

/**
 * @brief Starts all application services.
 *
 * Services are started in dependency order. If one service fails to
 * initialize, startup is aborted and false is returned.
 *
 * Startup order
 * -------------
 * @code
 * Logger
 *      │
 *      ▼
 * Storage
 *      │
 *      ▼
 * WiFi
 *      │
 *      ▼
 * Web Server
 * @endcode
 *
 * @return true if all services started successfully.
 * @return false if one or more services failed.
 */
bool Services::start()
{
    Serial.println("Services::Start - We are staring the app");

    if (!logger.start())
    {
        Serial.println("FATAL ERROR: Can't start Logger");
        return false;
    }

    logger.info("Starting application services...");

    if (!display.start())
    {
        Serial.println("FATAL ERROR: Can't start DisplayManager");
        return false;
    }

    if (!storage.start())
    {
        logger.error("Failed to start StorageManager.");
        return false;
    }

    if (!wifi.start())
    {
        logger.error("Failed to start WiFiManager.");
        return false;
    }

    logger.printBanner();
    logger.info("All services started successfully.");

    return true;
}

/**
 * @brief Updates all running services.
 *
 * This function should be called continuously from the application's
 * main loop.
 */
void Services::update()
{
    if (web.isRunning())
        web.handleClients();

    if(display.isRunning())
        display.update();
}

/**
 * @brief Stops all running services.
 *
 * Services are stopped in reverse startup order to ensure that
 * dependent services are shut down before the services they rely on.
 */
void Services::stop()
{
    logger.info("Stopping application services...");

    web.stop();
    logger.info("WebServerManager stopped.");

    wifi.stop();
    logger.info("WiFiManager stopped.");

    storage.stop();
    logger.info("StorageManager stopped.");

    logger.info("Logger stopped.");

    logger.stop();
}

/**
 * @brief Starts AP mode on the Ch3rryB0mb
 *
 * Automatically starts AP mode, and starts the webserver
 * 
 * @return true if all services started successfully.
 * @return false if one or more services failed.
 */
bool Services::startAPMode()
{
    logger.info("AP Mode Activated: Starting Access Point");

    WiFiAPConfig config;

    if (!wifi.startAP(config))
    {
        logger.error("Failed to start Access Point.");
        return false;
    }

    if (!web.start())
    {
        logger.error("Failed to start WebServerManager.");
        wifi.stopAP();
        return false;
    }

    logger.info("Success: Webserver started.");
    logger.info("Connect and navigate to: http://192.168.4.1");

    return true;
}