#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "../logger/logger.h"

/**
 * @class SPIManager
 * @brief Manages the shared external SPI bus.
 *
 * The SPIManager owns and initializes the SPI bus used by
 * external peripherals such as the SD card, NRF24 and CC1101.
 *
 * Individual devices share the MOSI, MISO and SCK lines and
 * use their own chip-select pins.
 */
class SPIManager
{
public:
    /**
     * @brief Constructs a new SPIManager.
     *
     * @param logger Reference to the application's Logger.
     */
    explicit SPIManager(Logger& logger);

    /**
     * @brief Initializes the shared SPI bus.
     *
     * @return true if the SPI bus was initialized successfully.
     */
    bool start();

    /**
     * @brief Stops the shared SPI bus.
     *
     * @return true if the SPI bus was stopped successfully.
     */
    bool stop();

    /**
     * @brief Returns whether the SPIManager is running.
     *
     * @return true if the SPI bus is initialized.
     */
    bool isRunning() const;

    /**
     * @brief Returns the shared SPI bus instance.
     *
     * @return Reference to the SPIClass instance.
     */
    SPIClass& getBus();

private:
    /**
     * @brief Reference to the application's Logger instance.
     *
     * Used for logging SPI bus initialization and state changes.
     */
    Logger& logger;

    /**
     * @brief Shared SPI bus instance.
     *
     * Represents the hardware SPI bus used by peripherals such as
     * the SD card, NRF24 and CC1101 modules.
     */
    SPIClass bus;

    /**
     * @brief Indicates whether the SPIManager is currently running.
     */
    bool running = false;

    /**
     * @brief GPIO pin used for the SPI clock signal.
     */
    static constexpr uint8_t SPI_SCK = 18;

    /**
     * @brief GPIO pin used for SPI MISO communication.
     */
    static constexpr uint8_t SPI_MISO = 19;

    /**
     * @brief GPIO pin used for SPI MOSI communication.
     */
    static constexpr uint8_t SPI_MOSI = 23;
};