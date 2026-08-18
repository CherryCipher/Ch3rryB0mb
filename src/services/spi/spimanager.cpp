#include "spimanager.h"

/**
 * @brief Constructs a new SPIManager.
 *
 * @param logger Reference to the application's Logger.
 */
SPIManager::SPIManager(Logger& logger)
    : logger(logger),
      bus(VSPI)
{
}

/**
 * @brief Initializes the shared external SPI bus.
 *
 * Configures the SPI bus used by external peripherals.
 *
 * @return true if initialization succeeded.
 */
bool SPIManager::start()
{
    if (running)
        return true;

    logger.info("Starting SPIManager.");

    bus.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    running = true;

    logger.info("SPIManager started.");

    return true;
}

/**
 * @brief Stops the shared external SPI bus.
 *
 * @return true if the SPI bus was stopped successfully.
 */
bool SPIManager::stop()
{
    if (!running)
        return true;

    bus.end();

    running = false;

    logger.info("SPIManager stopped.");

    return true;
}

/**
 * @brief Returns whether the SPIManager is running.
 *
 * @return true if the SPI bus is initialized.
 */
bool SPIManager::isRunning() const
{
    return running;
}

/**
 * @brief Returns the shared SPI bus instance.
 *
 * @return Reference to the shared SPIClass instance.
 */
SPIClass& SPIManager::getBus()
{
    return bus;
}