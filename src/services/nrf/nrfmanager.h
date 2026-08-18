#pragma once

#include <Arduino.h>
#include <RF24.h>

#include "../logger/logger.h"
#include "../spi/spimanager.h"

/**
 * @class NRFManager
 * @brief Manages NRF24 radio functionality.
 *
 * The NRFManager is responsible for initializing and controlling
 * the NRF24 radio used by Ch3rryB0mb.
 *
 * The radio uses the software SPI configuration provided by
 * SPIManager.
 *
 * Responsibilities
 * ----------------
 * - Initialize the NRF24 radio.
 * - Manage NRF24 radio state.
 * - Perform 2.4 GHz channel scanning.
 * - Detect RF activity.
 * - Support future packet transmission and reception.
 */
class NRFManager
{
public:
    /**
     * @brief Constructs a new NRFManager.
     *
     * @param logger Reference to the application's Logger.
     * @param spiManager Reference to the application's SPIManager.
     */
    NRFManager(Logger& logger, SPIManager& spiManager);

    /**
     * @brief Initializes the NRF24 radio.
     *
     * Verifies that the SPI infrastructure is available and attempts
     * to initialize the connected NRF24 radio using software SPI.
     *
     * @return true if the NRF24 radio was initialized successfully.
     * @return false otherwise.
     */
    bool start();

    /**
     * @brief Stops the NRFManager.
     *
     * Powers down the NRF24 radio and marks the manager as inactive.
     *
     * @return true if the NRFManager stopped successfully.
     */
    bool stop();

    /**
     * @brief Returns whether the NRFManager is currently running.
     *
     * @return true if the NRFManager is running.
     * @return false otherwise.
     */
    bool isRunning() const;

private:
    /**
     * @brief GPIO pin connected to the NRF24 Chip Enable pin.
     */
    static constexpr uint8_t NRF_CE = 27;

    /**
     * @brief GPIO pin connected to the NRF24 Chip Select pin.
     */
    static constexpr uint8_t NRF_CSN = 22;

    /**
     * @brief Reference to the application's Logger instance.
     */
    Logger& logger;

    /**
     * @brief Reference to the application's SPIManager.
     *
     * Provides the centralized SPI configuration used by the radio.
     */
    SPIManager& spiManager;

    /**
     * @brief NRF24 radio instance.
     *
     * The RF24 library uses software SPI when SOFTSPI is enabled
     * through the project build configuration.
     */
    RF24 radio;

    /**
     * @brief Indicates whether the NRFManager is currently running.
     */
    bool running = false;
};