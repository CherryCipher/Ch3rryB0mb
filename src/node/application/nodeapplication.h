/**
 * @file nodeapplication.h
 * @brief Declaration of the Ch3rryN0de application controller.
 *
 * NodeApplication manages the Ch3rryN0de application state, handles
 * configuration received through BLE and controls active radio sessions.
 */

#pragma once

#include <Arduino.h>

#include "node/nodeprotocol.h"

class NodeServices;

/**
 * @class NodeApplication
 * @brief Controls Ch3rryN0de configuration and radio sessions.
 *
 * NodeApplication forms the application layer of Ch3rryN0de.
 *
 * It uses NodeServices for hardware access while keeping application
 * state, protocol handling and radio session behavior outside the
 * service layer.
 *
 * The node initially enters BLE configuration mode. Configuration and
 * commands received through BLE are stored and processed by update().
 *
 * NRF24 beacon mode is currently supported as the first radio session.
 */
class NodeApplication
{
public:
    /**
     * @brief Constructs the NodeApplication.
     *
     * @param services Reference to the Ch3rryN0de services.
     */
    explicit NodeApplication(NodeServices& services);

    /**
     * @brief Starts the node application.
     *
     * Creates the Ch3rryN0de BLE GATT service, registers protocol
     * characteristics and starts advertising in configuration mode.
     *
     * @return true when the application started successfully.
     */
    bool start();

    /**
     * @brief Updates the node application.
     *
     * Processes pending commands and updates the currently active
     * radio session.
     */
    void update();

private:
    /**
     * @brief BLE advertisement name used by this node.
     */
    static constexpr const char* NODE_NAME = "C3N0-01";

    /**
     * @brief Active NodeApplication instance used by BLE callbacks.
     *
     * BLEManager uses a plain function pointer for characteristic writes,
     * so the static callback forwards received data to this instance.
     */
    static NodeApplication* instance;

    /**
     * @brief Reference to the shared node services.
     */
    NodeServices& services;

    /**
     * @brief Configuration currently stored by the node.
     */
    NodeConfig activeConfig;

    /**
     * @brief Indicates whether a valid configuration has been received.
     */
    bool configReceived = false;

    /**
     * @brief Indicates whether a START command is waiting to be processed.
     */
    bool startRequested = false;

    /**
     * @brief Indicates whether a radio session is currently running.
     */
    bool sessionRunning = false;

    /**
     * @brief Timestamp of the last transmitted NRF24 beacon.
     */
    uint32_t lastBeacon = 0;

    /**
     * @brief Static BLE write callback used by BLEManager.
     *
     * Forwards incoming characteristic writes to the active
     * NodeApplication instance.
     *
     * @param characteristicUUID UUID of the written characteristic.
     * @param data Pointer to the received data.
     * @param length Number of received bytes.
     */
    static void bleWriteCallback(const String& characteristicUUID, const uint8_t* data, size_t length);

    /**
     * @brief Handles a BLE characteristic write.
     *
     * Routes protocol messages to the appropriate application handler.
     *
     * @param characteristicUUID UUID of the written characteristic.
     * @param data Pointer to the received data.
     * @param length Number of received bytes.
     */
    void handleBLEWrite(const String& characteristicUUID, const uint8_t* data, size_t length);

    /**
     * @brief Handles a received node configuration.
     *
     * Validates the binary NodeConfig and stores it as the active
     * configuration.
     *
     * @param data Pointer to the received configuration.
     * @param length Number of received bytes.
     */
    void handleConfig(const uint8_t* data, size_t length);

    /**
     * @brief Handles a received node command.
     *
     * @param data Pointer to the received command.
     * @param length Number of received bytes.
     */
    void handleCommand(const uint8_t* data, size_t length);

    /**
     * @brief Processes a pending START request.
     *
     * Starts the requested radio session outside the BLE callback
     * context.
     */
    void processStartRequest();

    /**
     * @brief Starts the currently configured radio session.
     *
     * @return true when the session started successfully.
     */
    bool startSession();

    /**
     * @brief Stops the currently active radio session.
     */
    void stopSession();

    /**
     * @brief Starts an NRF24 beacon session.
     *
     * Initializes and configures the NRF24 transmitter using the
     * configured channel and beacon interval.
     *
     * @return true when the beacon session started successfully.
     */
    bool startNRFBeacon();

    /**
     * @brief Updates an active NRF24 beacon session.
     *
     * Sends a beacon packet whenever the configured transmission
     * interval has elapsed.
     */
    void updateNRFBeacon();

    /**
     * @brief Updates the node status exposed through BLE.
     *
     * @param status New node status.
     */
    void setNodeStatus(NodeStatus status);
};