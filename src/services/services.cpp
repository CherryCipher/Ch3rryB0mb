#include "Services.h"

/**
 * @brief Constructs the application's shared services.
 *
 * Every shared service is constructed exactly once and receives the
 * dependencies it requires.
 */
Services::Services()
    : logger()
    , spi(logger)
    , storage(logger, spi)
    , wifi(logger)
    , web(storage, logger)
    , ui(logger)
    , nrf(logger, spi)
{
}

/**
 * @brief Starts all application services.
 *
 * Services are started in dependency order. If one service fails to
 * initialize, startup is aborted and false is returned.
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

    if (!spi.start())
    {
        logger.error("Failed to start SPI Manager.");
        return false;
    }

    if (!storage.start())
    {
        logger.error("Failed to start StorageManager.");
        return false;
    }

    if (!ui.start())
    {
        Serial.println("FATAL ERROR: Can't start UIManager");
        return false;
    }

    if (!wifi.start())
    {
        logger.error("Failed to start WiFiManager.");
        return false;
    }

    if (!nrf.start())
    {
        logger.error("Failed to start NRFManager.");
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

    if(ui.isRunning())
        ui.update();
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

    ui.stop();
    logger.info("UIManager stopped.");

    nrf.stop();
    logger.info("NRFManager stopped.");

    logger.info("Logger stopped.");

    logger.stop();
}