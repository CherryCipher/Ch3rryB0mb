/**
 * @file main.cpp
 * @brief Entry point for the Ch3rryN0de firmware.
 *
 * Starts the node services and exposes the Ch3rryN0de configuration
 * interface through BLE.
 */

#include <Arduino.h>

#include "nodeservices/nodeservices.h"
#include "node/nodeprotocol.h"

NodeServices services;

static constexpr const char* NODE_NAME = "C3N0-01";

/**
 * @brief Handles BLE writes received by the node.
 *
 * @param characteristicUUID UUID of the written characteristic.
 * @param data Pointer to the received data.
 * @param length Number of received bytes.
 */
void nodeBLEWrite(const String& characteristicUUID, const uint8_t* data, size_t length)
{
    if (!characteristicUUID.equalsIgnoreCase(NodeProtocol::CONFIG_UUID)) return;

    if (length != sizeof(NodeConfig)) {
        services.logger.error("Invalid NodeConfig size.");
        return;
    }

    NodeConfig config;
    memcpy(&config, data, sizeof(NodeConfig));

    if (config.version != NodeProtocol::VERSION) {
        services.logger.error("Unsupported NodeConfig version.");
        return;
    }

    services.logger.info("Node configuration received.");

    Serial.print("RADIO: ");
    Serial.println(static_cast<uint8_t>(config.radio));

    Serial.print("MODE: ");
    Serial.println(static_cast<uint8_t>(config.mode));

    Serial.print("CHANNEL: ");
    Serial.println(config.channel);

    Serial.print("FREQUENCY: ");
    Serial.println(config.frequency, 3);

    Serial.print("INTERVAL: ");
    Serial.println(config.interval);
}

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

    if (!services.ble.createServer(NodeProtocol::SERVICE_UUID)) {
        services.logger.error("Failed to create node BLE service.");
        return;
    }

    if (!services.ble.addServerCharacteristic(NodeProtocol::CONFIG_UUID, NIMBLE_PROPERTY::WRITE, nodeBLEWrite)) {
        services.logger.error("Failed to create node config characteristic.");
        return;
    }

    if (!services.ble.startServer()) {
        services.logger.error("Failed to start node BLE server.");
        return;
    }

    if (!services.ble.startAdvertising(NODE_NAME, NodeProtocol::SERVICE_UUID)) {
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