#include "nrfmanager.h"

/**
 * @brief Constructs a new NRFManager.
 *
 * Stores references to the application's Logger and SPIManager
 * and configures the NRF24 CE and CSN pins.
 *
 * @param logger Reference to the application's Logger.
 * @param spiManager Reference to the application's SPIManager.
 */
NRFManager::NRFManager(Logger& logger, SPIManager& spiManager)
    : logger(logger), spiManager(spiManager), radio(NRF_CE, NRF_CSN)
{
}

/**
 * @brief Initializes the NRF24 radio.
 *
 * Verifies that the shared SPI infrastructure is available and attempts
 * to initialize the NRF24 radio using the hardware SPI bus managed by
 * SPIManager.
 *
 * The radio is powered down immediately after successful detection because
 * no scanning or packet operations are performed during initialization.
 *
 * @return true if the NRF24 radio was detected and initialized successfully.
 * @return false if the SPIManager is unavailable or the NRF24 radio could
 *         not be detected.
 */
bool NRFManager::start()
{
    if (running)
        return true;

    logger.info("Starting NRFManager.");

    if (!spiManager.isRunning())
    {
        logger.error("SPIManager is not running.");
        return false;
    }

    if (!radio.begin(&spiManager.getHardwareBus()))
    {
        logger.error("NRF24 radio not detected.");
        return false;
    }

    radio.powerDown();
    running = true;

    logger.info("NRF24 radio detected.");
    logger.info("NRFManager started.");

    return true;
}

/**
 * @brief Stops the NRFManager.
 *
 * Powers down the NRF24 radio and marks the manager as inactive.
 *
 * If the manager is already stopped, the function returns successfully
 * without performing additional radio operations.
 *
 * @return true when the NRFManager is stopped.
 */
bool NRFManager::stop()
{
    if (!running)
        return true;

    radio.powerDown();
    running = false;

    logger.info("NRFManager stopped.");

    return true;
}

/**
 * @brief Returns whether the NRFManager is currently running.
 *
 * @return true if the NRF24 radio was initialized successfully and the
 *         manager is active.
 * @return false otherwise.
 */
bool NRFManager::isRunning() const
{
    return running;
}