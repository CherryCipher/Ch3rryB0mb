#pragma once

#include "../logger/logger.h"
#include "../spi/spimanager.h"

/**
 * @class NRFManager
 * @brief Manages NRF24 radio functionality.
 *
 * The NRFManager is responsible for all communication with the NRF24 radio.
 * It uses the shared SPI bus provided by SPIManager.
 *
 * Applications and features should never communicate directly with the
 * NRF24 library. All NRF24 operations should be performed through this manager.
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
     * @param spiManager Reference to the shared SPIManager.
     */
    NRFManager(Logger& logger, SPIManager& spiManager);

    /**
     * @brief Initializes the NRFManager.
     *
     * Verifies that the shared SPI bus is available and prepares the
     * NRFManager for use.
     *
     * The physical NRF24 radio is not initialized yet.
     *
     * @return true if initialization succeeded.
     * @return false otherwise.
     */
    bool start();

    /**
     * @brief Stops the NRFManager.
     *
     * Stops NRF24 functionality and returns the manager to an inactive state.
     *
     * @return true if the manager stopped successfully.
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
     * @brief Reference to the application's Logger instance.
     *
     * Used for logging NRF24 related operations.
     */
    Logger& logger;

    /**
     * @brief Reference to the shared SPIManager.
     *
     * Provides access to the SPI bus used by the NRF24 radio.
     */
    SPIManager& spiManager;

    /**
     * @brief Indicates whether the NRFManager is currently running.
     */
    bool running = false;
};