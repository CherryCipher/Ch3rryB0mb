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
    , ble(logger)
    , web(storage, logger)
    , ui(logger)
    , nrf(logger, spi)
    , cc1101(logger, spi)
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

    printHeap("logger");

    logger.info("Starting application services...");

    if (!spi.start())
    {
        logger.error("Failed to start SPI Manager.");
        return false;
    }

    printHeap("Spi");

    if (!storage.start())
    {
        logger.error("Failed to start StorageManager.");
        return false;
    }
    printHeap("storage");

    if (!ui.start())
    {
        Serial.println("FATAL ERROR: Can't start UIManager");
        return false;
    }
    printHeap("ui");

    if (!wifi.start())
    {
        logger.error("Failed to start WiFiManager.");
        return false;
    }
    printHeap("wifi");

    if (!ble.start())
    {
        logger.error("Failed to start BLEManager.");
        return false;
    }
    printHeap("ble");

    if (!nrf.start())
    {
        logger.error("Failed to start NRFManager. It will be unavailable in the main menu");
        serviceDown = true;
    }
    printHeap("nrf");

    if (!cc1101.start())
    {
        logger.error("Failed to start CC1101Manager.");
        serviceDown = true;
    }
    printHeap("cc11");

    logger.printBanner();

    if(!serviceDown)
        logger.info("All services started successfully.");
    else
    {
        logger.warning("NOT all services started successfully.");
        logger.info("These are non crucial services but some features may not be available.");
    }

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

    ble.stop();
    logger.info("BLEManager stopped.");

    storage.stop();
    logger.info("StorageManager stopped.");

    ui.stop();
    logger.info("UIManager stopped.");

    nrf.stop();
    logger.info("NRFManager stopped.");

    cc1101.stop();
    logger.info("CC1101Manager stopped.");

    logger.info("Logger stopped.");

    logger.stop();
}

#include <esp_heap_caps.h>

void Services::printHeap(const char* label)
{
    Serial.printf(
        "[HEAP] %-24s free=%u largest=%u internal=%u\n",
        label,
        ESP.getFreeHeap(),
        heap_caps_get_largest_free_block(MALLOC_CAP_8BIT),
        heap_caps_get_free_size(MALLOC_CAP_INTERNAL)
    );
}