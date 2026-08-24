/**
 * @file main.cpp
 * @brief Entry point for the Ch3rryN0de firmware.
 */

#include <Arduino.h>

#include "nodeservices/nodeservices.h"
#include "application/nodeapplication.h"

NodeServices services;
NodeApplication application(services);

/**
 * @brief Initializes the Ch3rryN0de firmware.
 */
void setup()
{
    Serial.begin(115200);
    delay(500);

    if (!services.start()) {
        Serial.println("[ERROR] Failed to start C3N0 services.");
        return;
    }
    
    if (services.startNRF())
    {
        services.logger.info("NRF24 hardware test: OK.");
        services.stopNRF();
    } 
    else 
    {
        services.logger.error("NRF24 hardware test: FAIL.");
    }

    if (!application.start()) {
        services.logger.error("Failed to start NodeApplication.");
        return;
    }
}

/**
 * @brief Updates the Ch3rryN0de application.
 */
void loop()
{
    application.update();
    delay(5);
}