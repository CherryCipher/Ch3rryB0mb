#include "spimanager.h"

/**
 * @brief Constructs a new SPIManager.
 *
 * Creates the hardware SPI bus used by shared SPI peripherals.
 *
 * @param logger Reference to the application's Logger.
 */
SPIManager::SPIManager(Logger& logger)
    : logger(logger), hardwareBus(VSPI)
{
}

/**
 * @brief Initializes the SPI infrastructure.
 *
 * Initializes the shared hardware SPI bus using the configured
 * SCK, MISO and MOSI pins.
 *
 * @return true if initialization succeeded.
 */
bool SPIManager::start()
{
    if (running)
        return true;

    logger.info("Starting SPIManager.");

    hardwareBus.begin(HARDWARE_SPI_SCK, HARDWARE_SPI_MISO, HARDWARE_SPI_MOSI);

    running = true;

    logger.info("SPIManager started.");

    return true;
}

/**
 * @brief Stops the SPI infrastructure.
 *
 * Stops the shared hardware SPI bus and marks the manager as inactive.
 *
 * @return true if the SPIManager stopped successfully.
 */
bool SPIManager::stop()
{
    if (!running)
        return true;

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
 * @return Reference to the hardware SPIClass instance.
 */
SPIClass& SPIManager::getHardwareBus()
{
    return hardwareBus;
}

/**
 * @brief Returns the radio software SPI clock pin.
 *
 * @return GPIO number used for SCK.
 */
uint8_t SPIManager::getRadioSckPin() const
{
    return RADIO_SPI_SCK;
}

/**
 * @brief Returns the radio software SPI MISO pin.
 *
 * @return GPIO number used for MISO.
 */
uint8_t SPIManager::getRadioMisoPin() const
{
    return RADIO_SPI_MISO;
}

/**
 * @brief Returns the radio software SPI MOSI pin.
 *
 * @return GPIO number used for MOSI.
 */
uint8_t SPIManager::getRadioMosiPin() const
{
    return RADIO_SPI_MOSI;
}