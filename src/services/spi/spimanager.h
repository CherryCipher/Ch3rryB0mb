/**
 * @file spimanager.h
 * @brief Declaration of the shared SPI manager.
 *
 * The SPIManager initializes and manages the shared hardware SPI bus used
 * by SPI peripherals such as the NRF24L01+ and CC1101.
 */

#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "services/logger/logger.h"

/**
 * @class SPIManager
 * @brief Manages the shared hardware SPI bus.
 *
 * The SPIManager initializes the hardware SPI bus and ensures that all
 * connected SPI peripherals are deselected before the bus is started.
 *
 * SPI peripherals share the SCK, MISO and MOSI lines while each peripheral
 * uses its own chip-select pin.
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
     * Configures all known chip-select pins as outputs and drives them HIGH
     * before initializing the shared SPI bus. This prevents multiple SPI
     * peripherals from accessing the bus simultaneously during startup.
     *
     * @return true if initialization succeeded.
     */
    bool start();

    /**
     * @brief Stops the shared SPI infrastructure.
     *
     * Deselects all known SPI peripherals before stopping the hardware SPI bus.
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

private:
    /** @brief Logger used by the SPIManager. */
    Logger& logger;

    /** @brief Shared VSPI hardware bus instance. */
    SPIClass hardwareBus;

    /** @brief Indicates whether the SPIManager is currently running. 
     * 
    */
    bool running = false;

    /** @brief Hardware SPI clock pin. */
    static constexpr uint8_t HARDWARE_SPI_SCK = 18;

    /** @brief Hardware SPI MISO pin shared by all SPI peripherals. 
     * 
    */
    static constexpr uint8_t HARDWARE_SPI_MISO = 19;

    /** @brief Hardware SPI MOSI pin shared by all SPI peripherals. 
     * 
    */
    static constexpr uint8_t HARDWARE_SPI_MOSI = 23;

    /** @brief NRF24L01+ SPI chip-select pin. 
     * 
    */
    static constexpr uint8_t NRF_CSN_PIN = 22;

    /** @brief CC1101 SPI chip-select pin. 
     * 
    */
    static constexpr uint8_t CC1101_CSN_PIN = 16;
};