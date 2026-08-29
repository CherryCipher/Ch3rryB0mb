/**
 * @file spimanager.cpp
 * @brief Implementation of the shared SPI manager.
 */

#include "spimanager.h"
#include "hardware/modulepins.h"

/**
 * @brief Constructs a new SPIManager.
 *
 * Creates the VSPI hardware bus used by shared SPI peripherals.
 *
 * @param logger Reference to the application's Logger.
 */
SPIManager::SPIManager(Logger& logger)
    : logger(logger),
      hardwareBus(ModulePins::SPI_BUS)
{
}

/**
 * @brief Starts the shared hardware SPI bus.
 *
 * Configures all SPI chip select pins in their inactive state before
 * initializing the shared SPI bus.
 *
 * @return true when the SPI manager started successfully.
 */
bool SPIManager::start()
{
    if (running) return true;

    logger.info("Starting SPIManager.");

    pinMode(ModulePins::NRF_CSN, OUTPUT);
    pinMode(ModulePins::CC1101_CSN, OUTPUT);

    digitalWrite(ModulePins::NRF_CSN, HIGH);
    digitalWrite(ModulePins::CC1101_CSN, HIGH);

    hardwareBus.begin(ModulePins::SPI_SCK, ModulePins::SPI_MISO, ModulePins::SPI_MOSI, ModulePins::NRF_CSN);

    running = true;

    logger.info("SPIManager started.");
    return true;
}

/**
 * @brief Stops the shared SPI infrastructure.
 *
 * Deselects all known SPI peripherals before stopping the shared hardware
 * SPI bus. This leaves the connected peripherals in a safe inactive state.
 *
 * @return true if the SPIManager stopped successfully.
 */
bool SPIManager::stop()
{
    if (!running) return true;

    // Ensure all SPI peripherals remain deselected.
    digitalWrite(ModulePins::NRF_CSN, HIGH);
    digitalWrite(ModulePins::CC1101_CSN, HIGH);

    hardwareBus.end();

    running = false;

    logger.info("SPIManager stopped.");

    return true;
}

/**
 * @brief Returns whether the SPIManager is currently running.
 *
 * @return true if the SPIManager is running.
 * @return false otherwise.
 */
bool SPIManager::isRunning() const
{
    return running;
}

/**
 * @brief Returns the shared hardware SPI bus.
 *
 * The returned SPIClass instance can be used by peripheral managers that
 * communicate over the shared hardware SPI bus.
 *
 * @return Reference to the hardware SPIClass instance.
 */
SPIClass& SPIManager::getHardwareBus()
{
    return hardwareBus;
}