/**
 * @file nodeservices.h
 * @brief Declaration of the Ch3rryN0de service container.
 *
 * NodeServices owns and provides access to the hardware and system
 * services required by the Ch3rryN0de firmware.
 *
 * The node reuses shared Ch3rryB0mb managers where possible while keeping
 * node-specific functionality such as the OLED display separate.
 */

#pragma once

#include "services/logger/logger.h"
#include "services/spi/spimanager.h"
#include "services/nrf/nrfmanager.h"
#include "services/cc1101/cc1101manager.h"
#include "services/BLE/blemanager.h"

#include "../display/nodedisplaymanager.h"

/**
 * @class NodeServices
 * @brief Owns and initializes the services used by Ch3rryN0de.
 *
 * NodeServices acts as the central service container for the node firmware.
 * It owns the logger, shared radio managers and node-specific display manager.
 *
 * The class is responsible for starting and stopping these services in a
 * predictable order and exposing them to higher-level node components.
 */
class NodeServices
{
public:
    /**
     * @brief Constructs the node service container.
     *
     * Initializes service dependencies such as BLEManager, which requires
     * access to the shared Logger instance.
     */
    NodeServices();

    /**
     * @brief Starts all services required by the node.
     *
     * Initializes the display, SPI bus and available radio managers.
     * Hardware initialization results are reported through Serial and,
     * when available, on the node OLED display.
     *
     * @return true if all required node services started successfully.
     * @return false if one or more required services failed to start.
     */
    bool start();

    /**
     * @brief Stops all active node services.
     *
     * Stops active radio services and clears the node display.
     */
    void stop();

    /**
     * @brief Shared application logger.
     */
    Logger logger;

    /**
     * @brief Shared SPI bus manager.
     */
    SPIManager spi;

    /**
     * @brief NRF24 radio manager.
     */
    NRFManager nrf;

    /**
     * @brief CC1101 radio manager.
     */
    CC1101Manager cc1101;

    /**
     * @brief Bluetooth Low Energy manager.
     */
    BLEManager ble;

    /**
     * @brief Node OLED display manager.
     */
    NodeDisplayManager display;

private:
    /**
     * @brief Whether the OLED display initialized successfully.
     */
    bool displayReady = false;

    /**
     * @brief Whether the NRF24 radio initialized successfully.
     */
    bool nrfReady = false;

    /**
     * @brief Whether the CC1101 radio initialized successfully.
     */
    bool cc1101Ready = false;

    /**
     * @brief Whether BLE initialized successfully.
     */
    bool bleReady = false;
};