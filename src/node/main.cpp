/**
 * @file main.cpp
 * @brief Entry point for the Ch3rryN0de firmware.
 *
 * Initializes the shared node services and starts the node application.
 */

#include <Arduino.h>

#include "nodeservices/nodeservices.h"
#include "application/nodeapplication.h"

NodeServices services;
NodeApplication application(services);

/**
 * @brief Initializes the Ch3rryN0de firmware.
 *
 * Starts the shared node services and enters BLE configuration mode.
 */
void setup()
{
    Serial.begin(115200);
    delay(500);

    if (!services.start()) {
        Serial.println("[ERROR] Failed to start C3N0 services.");
        return;
    }

    if (!application.start()) {
        services.logger.error("Failed to start NodeApplication.");
        return;
    }

    services.logger.info("NodeApplication started succesfull.");
    services.logger.printBanner();
}

/**
 * @brief Runs the Ch3rryN0de application loop.
 */
void loop()
{
    application.update();
    delay(1);
}