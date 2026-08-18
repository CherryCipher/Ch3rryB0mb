#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "../logger/logger.h"

/**
 * @class SPIManager
 * @brief Manages the SPI infrastructure used by Ch3rryB0mb.
 *
 * The SPIManager owns and initializes the shared hardware SPI bus used
 * by peripherals such as the SD card.
 *
 * It also provides the pin configuration for the software SPI bus used
 * by external radio modules such as the NRF24 and CC1101.
 *
 * Hardware SPI
 * ------------
 * SCK  : GPIO18
 * MISO : GPIO19
 * MOSI : GPIO23
 *
 * Radio software SPI
 * ------------------
 * SCK  : GPIO17
 * MISO : GPIO35
 * MOSI : GPIO16
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
     * @brief Initializes the SPI infrastructure.
     *
     * Initializes the hardware SPI bus. Software SPI peripherals use
     * the radio SPI pin configuration exposed by this manager.
     *
     * @return true if initialization succeeded.
     */
    bool start();

    /**
     * @brief Stops the SPI infrastructure.
     *
     * Stops the hardware SPI bus and marks the manager as inactive.
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
     * @return Reference to the hardware SPIClass instance.
     */
    SPIClass& getHardwareBus();

    /**
     * @brief Returns the radio software SPI clock pin.
     *
     * @return GPIO number used for SCK.
     */
    uint8_t getRadioSckPin() const;

    /**
     * @brief Returns the radio software SPI MISO pin.
     *
     * @return GPIO number used for MISO.
     */
    uint8_t getRadioMisoPin() const;

    /**
     * @brief Returns the radio software SPI MOSI pin.
     *
     * @return GPIO number used for MOSI.
     */
    uint8_t getRadioMosiPin() const;

private:
    /**
     * @brief Reference to the application's Logger instance.
     *
     * Used for logging SPI initialization and state changes.
     */
    Logger& logger;

    /**
     * @brief Shared hardware SPI bus instance.
     *
     * Used by hardware SPI peripherals such as the SD card.
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

    /**
     * @brief GPIO pin used for radio software SPI clock.
     *
     * This pin reuses the GPIO previously connected to the onboard RGB LED.
     */
    static constexpr uint8_t RADIO_SPI_SCK = 17;

    /**
     * @brief GPIO pin used for radio software SPI MISO.
     *
     * GPIO35 is input-only and is therefore suitable for MISO.
     */
    static constexpr uint8_t RADIO_SPI_MISO = 35;

    /**
     * @brief GPIO pin used for radio software SPI MOSI.
     *
     * This pin reuses the GPIO previously connected to the onboard RGB LED.
     */
    static constexpr uint8_t RADIO_SPI_MOSI = 16;
};