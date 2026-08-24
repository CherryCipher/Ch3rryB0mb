/**
 * @file nodeservices.h
 * @brief Declaration of the Ch3rryN0de service container.
 *
 * NodeServices owns and manages the shared hardware services used by
 * Ch3rryN0de.
 *
 * Infrastructure services are initialized during startup while radio
 * services such as NRF24 and CC1101 are started lazily when requested
 * by the application layer.
 */

#pragma once

#include <Arduino.h>

#include "services/logger/logger.h"
#include "services/spi/spimanager.h"
#include "services/nrf/nrfmanager.h"
#include "services/cc1101/cc1101manager.h"
#include "services/BLE/blemanager.h"
#include "node/display/nodedisplaymanager.h"

/**
 * @class NodeServices
 * @brief Owns and manages Ch3rryN0de hardware services.
 *
 * NodeServices provides centralized ownership and lifecycle management
 * for the node hardware layer.
 *
 * The shared SPI bus and BLEManager are started during node startup.
 * NRF24 and CC1101 radios remain inactive until explicitly requested
 * by the application layer.
 *
 * Only one SPI radio is activated at a time.
 */
class NodeServices
{
public:
    /**
     * @brief Constructs the Ch3rryN0de service container.
     */
    NodeServices();

    /**
     * @brief Starts the core Ch3rryN0de services.
     *
     * Initializes logging, the OLED display, shared SPI bus and BLEManager.
     *
     * NRF24 and CC1101 are deliberately not started here and remain in
     * safe inactive states until requested by the application layer.
     *
     * @return true when all required core services started successfully.
     */
    bool start();

    /**
     * @brief Stops all active Ch3rryN0de services.
     *
     * Stops active radios, BLE and the shared SPI manager.
     */
    void stop();

    /**
     * @brief Starts the NRF24 radio service.
     *
     * Stops the CC1101 when active, applies safe SPI radio pin states and
     * starts NRFManager.
     *
     * @return true when NRFManager started successfully.
     */
    bool startNRF();

    /**
     * @brief Stops the NRF24 radio service.
     *
     * Stops NRFManager and returns the NRF24 control pins to their safe
     * inactive states.
     */
    void stopNRF();

    /**
     * @brief Starts the CC1101 radio service.
     *
     * Stops the NRF24 when active, applies safe SPI radio pin states and
     * starts CC1101Manager.
     *
     * @return true when CC1101Manager started successfully.
     */
    bool startCC1101();

    /**
     * @brief Stops the CC1101 radio service.
     *
     * Stops CC1101Manager and returns its chip select pin to the inactive
     * state.
     */
    void stopCC1101();

    /**
     * @brief Application logger.
     */
    Logger logger;

    /**
     * @brief Shared SPI manager.
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
     * @brief NRF24 Chip Enable GPIO.
     */
    static constexpr uint8_t NRF_CE_PIN = 27;

    /**
     * @brief NRF24 Chip Select GPIO.
     */
    static constexpr uint8_t NRF_CSN_PIN = 22;

    /**
     * @brief CC1101 Chip Select GPIO.
     */
    static constexpr uint8_t CC1101_CSN_PIN = 16;

    /**
     * @brief Configures shared radio pins to safe inactive states.
     */
    void configureRadioPins();
};