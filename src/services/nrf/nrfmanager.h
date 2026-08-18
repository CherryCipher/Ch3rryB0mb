#pragma once

#include "../logger/logger.h"

/******************************************************************************
 * NRFManager
 *
 * Responsible for all NRF24 radio functionality within Ch3rryB0mb.
 *
 * Features
 * --------
 * - NRF24 radio initialization
 * - 2.4 GHz channel scanning
 * - RF activity detection
 * - Packet transmission and reception
 * - Future NRF24 extensions
 *
 * Design
 * ------
 * Applications should NEVER communicate directly with the NRF24 radio
 * library. All NRF24 operations should be performed through the NRFManager.
 *
 ******************************************************************************/
class NRFManager
{
public:
    /**
     * @brief Constructs a new NRFManager.
     *
     * @param logger Reference to the application's Logger.
     */
    explicit NRFManager(Logger& logger);

    /**
     * @brief Initializes the NRFManager.
     *
     * Prepares the manager for use but does not yet initialize
     * or enable the physical NRF24 radio.
     *
     * @return true if initialization succeeded.
     */
    bool start();

    /**
     * @brief Stops the NRFManager.
     *
     * Stops all NRF24 functionality and returns the manager
     * to an inactive state.
     *
     * @return true if the manager stopped successfully.
     */
    bool stop();

    /**
     * @brief Returns whether the NRFManager is currently running.
     *
     * @return true if the manager is running.
     * @return false otherwise.
     */
    bool isRunning() const;

private:
    /**
     * @brief Reference to the application's Logger instance.
     *
     * Used for logging messages related to NRF24 operations.
     */
    Logger& logger;

    /**
     * @brief Indicates whether the NRFManager has been started.
     */
    bool running = false;
};