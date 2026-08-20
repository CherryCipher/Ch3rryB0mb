#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "../logger/logger.h"

/**
 * @class SPIManager
 * @brief Manages the shared hardware SPI bus used by Ch3rryB0mb.
 *
 * The SPIManager owns and initializes the ESP32 hardware SPI bus used
 * by peripherals such as the microSD card, NRF24 and CC1101.
 *
 * All SPI peripherals share the same SCK, MISO and MOSI lines and use
 * their own chip-select and control pins.
 *
 * Hardware SPI
 * ------------
 * SCK  : GPIO18
 * MISO : GPIO19
 * MOSI : GPIO23
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
     * @brief Initializes the shared hardware SPI bus.
     *
     * Configures the hardware SPI bus using the configured SCK,
     * MISO and MOSI pins.
     *
     * @return true if initialization succeeded.
     */
    bool start();

    /**
     * @brief Stops the SPI infrastructure.
     *
     * Stops the shared hardware SPI bus and marks the manager as inactive.
     *
     * @return true if the SPIManager stopped successfully.
     */
    bool stop();

    /**
     * @brief Returns whether the SPIManager is currently running.
     *
     * @return true if the SPIManager is running.
     * @return false otherwise.
     */
    bool isRunning() const;

    /**
     * @brief Returns the shared hardware SPI bus.
     *
     * The returned SPI bus is shared between peripherals such as
     * the microSD card, NRF24 and CC1101. Each peripheral must use
     * its own chip-select pin.
     *
     * @return Reference to the hardware SPIClass instance.
     */
    SPIClass& getHardwareBus();

private:
    /**
     * @brief Reference to the application's Logger instance.
     *
     * Used for logging SPI initialization and state changes.
     */
    Logger& logger;

    /**
     * @brief Shared hardware SPI bus instance.
     */
    SPIClass hardwareBus;

    /**
     * @brief Indicates whether the SPIManager is currently running.
     */
    bool running = false;

    /**
     * @brief GPIO pin used for hardware SPI clock.
     */
    static constexpr uint8_t HARDWARE_SPI_SCK = 18;

    /**
     * @brief GPIO pin used for hardware SPI MISO.
     */
    static constexpr uint8_t HARDWARE_SPI_MISO = 19;

    /**
     * @brief GPIO pin used for hardware SPI MOSI.
     */
    static constexpr uint8_t HARDWARE_SPI_MOSI = 23;
};