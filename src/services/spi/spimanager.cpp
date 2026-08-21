/**
 * @file spimanager.cpp
 * @brief Implementation of the shared SPI manager.
 */

#include "spimanager.h"

/**
 * @brief Constructs a new SPIManager.
 *
 * Creates the VSPI hardware bus used by shared SPI peripherals.
 *
 * @param logger Reference to the application's Logger.
 */
SPIManager::SPIManager(Logger& logger)
    : logger(logger),
      hardwareBus(VSPI)
{
}

/**
 * @brief Initializes the shared hardware SPI bus.
 *
 * Configures the chip-select pins of all known SPI peripherals as outputs
 * and drives them HIGH before starting the hardware SPI bus. This ensures
 * that all peripherals are deselected during initialization.
 *
 * @return true if initialization succeeded.
 */
bool SPIManager::start()
{
    if (running) return true;

    logger.info("Starting SPIManager.");

    // Deselect NRF24L01+.
    pinMode(NRF_CSN_PIN, OUTPUT);
    digitalWrite(NRF_CSN_PIN, HIGH);

    // Deselect CC1101.
    pinMode(CC1101_CSN_PIN, OUTPUT);
    digitalWrite(CC1101_CSN_PIN, HIGH);

    // Start the shared hardware SPI bus.
    hardwareBus.begin(HARDWARE_SPI_SCK, HARDWARE_SPI_MISO, HARDWARE_SPI_MOSI);

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
    digitalWrite(NRF_CSN_PIN, HIGH);
    digitalWrite(CC1101_CSN_PIN, HIGH);

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