/**
 * @file cc1101manager.h
 * @brief Declaration of the CC1101 radio manager.
 */

#pragma once

#include <Arduino.h>
#include <RadioLib.h>

#include "../logger/Logger.h"
#include "../spi/spimanager.h"

/**
 * @class CC1101Manager
 * @brief Manages the CC1101 Sub-GHz radio.
 *
 * The CC1101Manager initializes and controls the CC1101 radio using the
 * shared hardware SPI bus provided by SPIManager.
 *
 * The CC1101 shares SCK, MISO and MOSI with other SPI peripherals while
 * using its own CSN pin.
 */
class CC1101Manager
{
public:
    /**
     * @brief Constructs a new CC1101Manager.
     *
     * @param logger Reference to the application's Logger.
     * @param spiManager Reference to the application's SPIManager.
     */
    CC1101Manager(Logger& logger, SPIManager& spiManager);

    /**
     * @brief Initializes the CC1101 radio.
     *
     * Verifies that the shared SPI infrastructure is running and attempts
     * to initialize communication with the CC1101.
     *
     * @return true if the CC1101 was initialized successfully.
     * @return false if initialization failed.
     */
    bool start();

    /**
     * @brief Stops the CC1101Manager.
     *
     * Places the CC1101 into sleep mode and marks the manager as inactive.
     *
     * @return true if the manager stopped successfully.
     */
    bool stop();

    /**
     * @brief Returns whether the CC1101Manager is currently running.
     *
     * @return true if the CC1101Manager is running.
     * @return false otherwise.
     */
    bool isRunning() const;

private:
    /** @brief Logger used by the CC1101Manager. */
    Logger& logger;

    /** @brief Shared SPI infrastructure used by the CC1101. */
    SPIManager& spiManager;

    /** @brief RadioLib module configuration for the CC1101. */
    Module module;

    /** @brief RadioLib CC1101 radio instance. */
    CC1101 radio;

    /** @brief Indicates whether the CC1101Manager is currently running. */
    bool running = false;

    /** @brief CC1101 SPI chip-select pin. */
    static constexpr uint8_t CC1101_CSN = 16;

    /** @brief CC1101 GDO0 interrupt/data pin. */
    static constexpr uint8_t CC1101_GDO0 = 35;
};