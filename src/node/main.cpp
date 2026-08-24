/**
 * @file main.cpp
 * @brief Entry point for the Ch3rryN0de firmware.
 *
 * Starts the node services and places the device in BLE configuration mode.
 * Higher-level node behavior can later be moved into a NodeApplication or
 * NodeController without changing the service layer.
 */

#include <Arduino.h>

#include "nodeservices/nodeservices.h"

NodeServices services;

static constexpr const char* NODE_NAME = "C3N0-01";
static constexpr const char* NODE_SERVICE_UUID = "6f17c001-9d8b-4b4d-a2e3-43d9a3c30001";

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

    services.logger.info("Starting node configuration mode.");

    if (!services.ble.startAdvertising(NODE_NAME, NODE_SERVICE_UUID)) {
        services.logger.error("Failed to start BLE advertising.");
        return;
    }

    services.logger.info(String("Advertising as ") + NODE_NAME + ".");
    services.display.showConfigMode();
}

/**
 * @brief Main Ch3rryN0de firmware loop.
 */
void loop()
{
    delay(10);
}