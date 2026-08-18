#include "nrfmanager.h"

/**
 * @brief Constructs a new NRFManager.
 *
 * Stores a reference to the application's Logger.
 *
 * @param logger Reference to the application's Logger.
 */
NRFManager::NRFManager(Logger& logger)
    : logger(logger)
{
}

/**
 * @brief Initializes the NRFManager.
 *
 * Prepares the NRFManager for use. The physical NRF24 radio
 * is not initialized yet.
 *
 * @return true if initialization succeeded.
 */
bool NRFManager::start()
{
    running = true;

    logger.info("NRFManager started.");

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
    running = false;

    logger.info("NRFManager stopped.");

    return true;
}

/**
 * @brief Returns whether the NRFManager is currently running.
 *
 * @return true if the manager is running.
 * @return false otherwise.
 */
bool NRFManager::isRunning() const
{
    return running;
}