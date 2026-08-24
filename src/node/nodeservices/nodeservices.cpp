/**
 * @file nodeservices.cpp
 * @brief Implementation of the Ch3rryN0de service container.
 */

#include "nodeservices.h"

/**
 * @brief Constructs the node service container.
 *
 * Initializes all service dependencies in the correct order.
 */
NodeServices::NodeServices()
    : spi(logger),
      nrf(logger, spi),
      cc1101(logger, spi),
      ble(logger)
{
}

/**
 * @brief Starts all services required by the node.
 *
 * @return true if all required services initialized successfully.
 * @return false if one or more required services failed to initialize.
 */
bool NodeServices::start()
{
    logger.start();

    displayReady = display.begin();

    // Safe shared SPI state.
    pinMode(22, OUTPUT);
    digitalWrite(22, HIGH);

    pinMode(16, OUTPUT);
    digitalWrite(16, HIGH);

    pinMode(27, OUTPUT);
    digitalWrite(27, LOW);

    bool spiReady = spi.start();
    bleReady = ble.start();

    if (displayReady) display.showConfigMode();

    return spiReady && bleReady;
}

/**
 * @brief Stops all active node services.
 */
void NodeServices::stop()
{
    logger.info("Stopping C3N0 services...");

    if (ble.isRunning()) ble.stop();
    if (cc1101.isRunning()) cc1101.stop();
    if (nrf.isRunning()) nrf.stop();
    if (spi.isRunning()) spi.stop();

    if (displayReady) display.clear();

    logger.info("C3N0 services stopped.");

    logger.stop();
}