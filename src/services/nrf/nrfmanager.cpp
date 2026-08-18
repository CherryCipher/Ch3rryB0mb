#include "nrfmanager.h"

/**
 * @brief Constructs a new NRFManager.
 *
 * Stores references to the application's Logger and shared SPIManager.
 *
 * @param logger Reference to the application's Logger.
 * @param spiManager Reference to the shared SPIManager.
 */
NRFManager::NRFManager(Logger& logger, SPIManager& spiManager)
    : logger(logger), spiManager(spiManager)
{
}

/**
 * @brief Initializes the NRFManager.
 *
 * Verifies that the shared SPI bus is available and prepares the
 * NRFManager for use.
 *
 * The physical NRF24 radio is not initialized yet.
 *
 * @return true if initialization succeeded.
 * @return false otherwise.
 */
bool NRFManager::start()
{
    if (running)
        return true;

    logger.info("NRFManager started.");

    if (!spiManager.isRunning())
    {
        logger.error("SPIManager is not running.");
        return false;
    }

    running = true;

    return true;
}

/**
 * @brief Stops the NRFManager.
 *
 * Stops NRF24 functionality and marks the manager as inactive.
 *
 * @return true if the manager stopped successfully.
 */
bool NRFManager::stop()
{
    if (!running)
        return true;

    running = false;

    logger.info("NRFManager stopped.");

    return true;
}

/**
 * @brief Returns whether the NRFManager is currently running.
 *
 * @return true if the NRFManager is running.
 * @return false otherwise.
 */
bool NRFManager::isRunning() const
{
    return running;
}