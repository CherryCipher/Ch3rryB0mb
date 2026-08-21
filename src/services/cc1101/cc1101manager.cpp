/**
 * @file cc1101manager.cpp
 * @brief Implementation of the CC1101 radio manager.
 */

#include "cc1101manager.h"

/**
 * @brief Constructs a new CC1101Manager.
 *
 * Stores references to the application's Logger and SPIManager and
 * configures the RadioLib CC1101 instance.
 *
 * @param logger Reference to the application's Logger.
 * @param spiManager Reference to the application's SPIManager.
 */
CC1101Manager::CC1101Manager(Logger& logger, SPIManager& spiManager)
    : logger(logger),
      spiManager(spiManager),
      module(CC1101_CSN, CC1101_GDO0, RADIOLIB_NC, RADIOLIB_NC, spiManager.getHardwareBus()),
      radio(&module)
{
}

/**
 * @brief Initializes the CC1101 radio.
 *
 * Verifies that the shared SPI infrastructure is running and attempts
 * to initialize communication with the CC1101.
 *
 * The radio is placed into idle mode after successful initialization.
 *
 * @return true if the CC1101 was initialized successfully.
 * @return false otherwise.
 */
bool CC1101Manager::start()
{
    if (running) return true;

    logger.info("Starting CC1101Manager.");

    if (!spiManager.isRunning())
    {
        logger.error("SPIManager is not running.");
        return false;
    }

    int16_t state = radio.begin();

    if (state != RADIOLIB_ERR_NONE)
    {
        logger.error("CC1101 radio not detected.");
        return false;
    }

    radio.standby();

    running = true;

    logger.info("CC1101 radio detected.");
    logger.info("CC1101Manager started.");

    return true;
}

/**
 * @brief Stops the CC1101Manager.
 *
 * Places the CC1101 into sleep mode and marks the manager as inactive.
 *
 * @return true if the CC1101Manager stopped successfully.
 */
bool CC1101Manager::stop()
{
    if (!running) return true;

    radio.sleep();
    running = false;

    logger.info("CC1101Manager stopped.");

    return true;
}

/**
 * @brief Returns whether the CC1101Manager is currently running.
 *
 * @return true if the CC1101Manager is running.
 * @return false otherwise.
 */
bool CC1101Manager::isRunning() const
{
    return running;
}