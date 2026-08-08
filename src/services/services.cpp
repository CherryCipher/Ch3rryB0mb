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
    if (!logger.start())
    {
        return false;
    }

    logger.info("Starting application services...");

    if (!storage.start())
    {
        logger.error("Failed to start StorageManager.");
        return false;
    }

    logger.info("StorageManager started.");

    if (!wifi.start())
    {
        logger.error("Failed to start WiFiManager.");
        return false;
    }

    logger.info("WiFiManager started.");

    if (!web.start())
    {
        logger.error("Failed to start WebServerManager.");
        return false;
    }

    logger.info("WebServerManager started.");

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
    web.handleClients();

    // Future services
    //
    // wifi.update();
    // display.update();
    // gps.update();
    // packetLab.update();
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