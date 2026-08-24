/**
 * @file nodeservices.cpp
 * @brief Implementation of the Ch3rryN0de service container.
 */

#include "nodeservices.h"

/**
 * @brief Constructs the Ch3rryN0de service container.
 */
NodeServices::NodeServices()
    : spi(logger),
      nrf(logger, spi),
      cc1101(logger, spi),
      ble(logger)
{
}

/**
 * @brief Starts the core Ch3rryN0de services.
 *
 * @return true when all required core services started successfully.
 */
bool NodeServices::start()
{
    logger.start();

    logger.info("Starting C3N0 services.");

    configureRadioPins();

    if (!display.begin()) {
        logger.error("Failed to start NodeDisplayManager.");
        return false;
    }

    display.showBoot();

    if (!spi.start()) {
        logger.error("Failed to start SPIManager.");
        return false;
    }

    if (!ble.start()) {
        logger.error("Failed to start BLEManager.");
        return false;
    }

    logger.info("C3N0 services started.");

    return true;
}

/**
 * @brief Stops all active Ch3rryN0de services.
 */
void NodeServices::stop()
{
    stopNRF();
    stopCC1101();

    if (ble.isRunning()) ble.stop();
    if (spi.isRunning()) spi.stop();

    configureRadioPins();

    logger.info("C3N0 services stopped.");
}

/**
 * @brief Starts the NRF24 radio service.
 *
 * Stops the CC1101 when active, restores safe NRF24 and CC1101 pin
 * states and starts NRFManager.
 *
 * @return true when NRFManager started successfully.
 */
bool NodeServices::startNRF()
{
    if (nrf.isRunning()) return true;

    if (cc1101.isRunning()) stopCC1101();

    pinMode(NRF_CE_PIN, OUTPUT);
    pinMode(NRF_CSN_PIN, OUTPUT);
    pinMode(CC1101_CSN_PIN, OUTPUT);

    digitalWrite(NRF_CE_PIN, LOW);
    digitalWrite(NRF_CSN_PIN, HIGH);
    digitalWrite(CC1101_CSN_PIN, HIGH);

    delay(10);

    logger.info("Starting NRF24 radio.");

    if (!nrf.start()) {
        logger.error("Failed to start NRFManager.");
        return false;
    }

    return true;
}

/**
 * @brief Stops the NRF24 radio service.
 */
void NodeServices::stopNRF()
{
    if (!nrf.isRunning()) return;

    nrf.stop();

    digitalWrite(NRF_CE_PIN, LOW);
    digitalWrite(NRF_CSN_PIN, HIGH);
}

/**
 * @brief Starts the CC1101 radio service.
 *
 * @return true when CC1101Manager started successfully.
 */
bool NodeServices::startCC1101()
{
    if (cc1101.isRunning()) return true;

    if (nrf.isRunning()) stopNRF();

    digitalWrite(NRF_CE_PIN, LOW);
    digitalWrite(NRF_CSN_PIN, HIGH);
    digitalWrite(CC1101_CSN_PIN, HIGH);

    if (!cc1101.start()) {
        logger.error("Failed to start CC1101Manager.");
        return false;
    }

    return true;
}

/**
 * @brief Stops the CC1101 radio service.
 */
void NodeServices::stopCC1101()
{
    if (!cc1101.isRunning()) return;

    cc1101.stop();

    digitalWrite(CC1101_CSN_PIN, HIGH);
}

/**
 * @brief Configures shared radio pins to safe inactive states.
 */
void NodeServices::configureRadioPins()
{
    pinMode(NRF_CE_PIN, OUTPUT);
    pinMode(NRF_CSN_PIN, OUTPUT);
    pinMode(CC1101_CSN_PIN, OUTPUT);

    digitalWrite(NRF_CE_PIN, LOW);
    digitalWrite(NRF_CSN_PIN, HIGH);
    digitalWrite(CC1101_CSN_PIN, HIGH);
}