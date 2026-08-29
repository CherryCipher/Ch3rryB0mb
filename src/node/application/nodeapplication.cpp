/**
 * @file nodeapplication.cpp
 * @brief Implementation of the Ch3rryN0de application controller.
 */

#include "nodeapplication.h"

#include <cstring>

#include "../nodeservices/nodeservices.h"

/**
 * @brief Shared NRF24 destination address used by C3N0 beacon sessions.
 */
static const uint8_t NRF_ADDRESS[6] = "C3N0";

/**
 * @brief Payload transmitted by NRF24 beacon mode.
 */
static constexpr const char* NRF_BEACON_PAYLOAD = "C3N0";

NodeApplication* NodeApplication::instance = nullptr;

/**
 * @brief Constructs the NodeApplication.
 *
 * @param services Reference to the Ch3rryN0de services.
 */
NodeApplication::NodeApplication(NodeServices& services)
    : services(services)
{
    instance = this;
}

/**
 * @brief Starts the node application.
 *
 * Creates the BLE configuration GATT server and places Ch3rryN0de
 * in discoverable configuration mode.
 *
 * @return true when the application started successfully.
 */
bool NodeApplication::start()
{
    services.logger.info("Starting NodeApplication.");

    if (!services.ble.isRunning() && !services.ble.start()) {
        services.logger.error("Failed to start BLEManager.");
        return false;
    }

    if (!services.ble.createServer(NodeProtocol::SERVICE_UUID)) {
        services.logger.error("Failed to create node BLE service.");
        return false;
    }

    if (!services.ble.addServerCharacteristic(NodeProtocol::CONFIG_UUID, NIMBLE_PROPERTY::WRITE, bleWriteCallback)) {
        services.logger.error("Failed to create CONFIG characteristic.");
        return false;
    }

    if (!services.ble.addServerCharacteristic(NodeProtocol::COMMAND_UUID, NIMBLE_PROPERTY::WRITE, bleWriteCallback)) {
        services.logger.error("Failed to create COMMAND characteristic.");
        return false;
    }

    if (!services.ble.addServerCharacteristic(NodeProtocol::STATUS_UUID, NIMBLE_PROPERTY::READ)) {
        services.logger.error("Failed to create STATUS characteristic.");
        return false;
    }

    setNodeStatus(NodeStatus::Idle);

    if (!services.ble.startServer()) {
        services.logger.error("Failed to start node BLE server.");
        return false;
    }

    services.logger.info("Node BLE server ready.");

    if (!services.ble.startAdvertising(NODE_NAME, NodeProtocol::SERVICE_UUID)) {
        services.logger.error("Failed to start BLE advertising.");
        return false;
    }

    services.logger.info(String("Node configuration mode active as ") + NODE_NAME + ".");
    services.display.showConfigMode();

    return true;
}

/**
 * @brief Updates the node application.
 */
void NodeApplication::update()
{
    processStartRequest();

    if (!sessionRunning) return;

    switch (activeConfig.radio) {
        case NodeRadio::NRF24:
            if (activeConfig.mode == NodeMode::Beacon) updateNRFBeacon();
            break;

        case NodeRadio::BLE:
        case NodeRadio::CC1101:
        default:
            break;
    }
}

/**
 * @brief Static BLE write callback used by BLEManager.
 *
 * @param characteristicUUID UUID of the written characteristic.
 * @param data Pointer to the received data.
 * @param length Number of received bytes.
 */
void NodeApplication::bleWriteCallback(const String& characteristicUUID, const uint8_t* data, size_t length)
{
    if (instance == nullptr) return;

    instance->handleBLEWrite(characteristicUUID, data, length);
}

/**
 * @brief Handles a BLE characteristic write.
 *
 * @param characteristicUUID UUID of the written characteristic.
 * @param data Pointer to the received data.
 * @param length Number of received bytes.
 */
void NodeApplication::handleBLEWrite(const String& characteristicUUID, const uint8_t* data, size_t length)
{
    if (characteristicUUID.equalsIgnoreCase(NodeProtocol::CONFIG_UUID)) {
        handleConfig(data, length);
        return;
    }

    if (characteristicUUID.equalsIgnoreCase(NodeProtocol::COMMAND_UUID)) {
        handleCommand(data, length);
        return;
    }

    services.logger.warning("Write received for unknown node characteristic.");
}

/**
 * @brief Handles a received node configuration.
 *
 * @param data Pointer to the received configuration.
 * @param length Number of received bytes.
 */
void NodeApplication::handleConfig(const uint8_t* data, size_t length)
{
    if (data == nullptr || length != sizeof(NodeConfig)) {
        services.logger.error("Invalid NodeConfig size.");
        setNodeStatus(NodeStatus::Error);
        return;
    }

    NodeConfig config;
    memcpy(&config, data, sizeof(NodeConfig));

    if (config.version != NodeProtocol::VERSION) {
        services.logger.error("Unsupported NodeConfig version.");
        setNodeStatus(NodeStatus::Error);
        return;
    }

    if (sessionRunning) stopSession();

    activeConfig = config;
    configReceived = true;
    startRequested = false;

    services.logger.info("Node configuration received.");

    Serial.print("RADIO: ");
    Serial.println(static_cast<uint8_t>(activeConfig.radio));

    Serial.print("MODE: ");
    Serial.println(static_cast<uint8_t>(activeConfig.mode));

    Serial.print("CHANNEL: ");
    Serial.println(activeConfig.channel);

    Serial.print("FREQUENCY: ");
    Serial.println(activeConfig.frequency, 3);

    Serial.print("INTERVAL: ");
    Serial.println(activeConfig.interval);

    setNodeStatus(NodeStatus::ConfigReceived);
}

/**
 * @brief Handles a received node command.
 *
 * @param data Pointer to the received command.
 * @param length Number of received bytes.
 */
void NodeApplication::handleCommand(const uint8_t* data, size_t length)
{
    if (data == nullptr || length != sizeof(NodeCommand)) {
        services.logger.error("Invalid NodeCommand size.");
        setNodeStatus(NodeStatus::Error);
        return;
    }

    NodeCommand command;
    memcpy(&command, data, sizeof(NodeCommand));

    switch (command) {
        case NodeCommand::Start:
            if (!configReceived) {
                services.logger.error("START received without configuration.");
                setNodeStatus(NodeStatus::Error);
                return;
            }

            startRequested = true;
            services.logger.info("START command received.");
            break;

        case NodeCommand::Stop:
            startRequested = false;
            stopSession();
            services.logger.info("STOP command received.");
            break;

        default:
            services.logger.error("Unknown node command.");
            setNodeStatus(NodeStatus::Error);
            break;
    }
}

/**
 * @brief Processes a pending START request.
 */
void NodeApplication::processStartRequest()
{
    if (!startRequested) return;

    startRequested = false;

    if (sessionRunning) stopSession();

    if (!startSession()) {
        sessionRunning = false;
        setNodeStatus(NodeStatus::Error);
        return;
    }

    sessionRunning = true;
    setNodeStatus(NodeStatus::Ready);
}

/**
 * @brief Starts the currently configured radio session.
 *
 * @return true when the session started successfully.
 */
bool NodeApplication::startSession()
{
    switch (activeConfig.radio) {
        case NodeRadio::NRF24:
            if (activeConfig.mode != NodeMode::Beacon) {
                services.logger.error("NRF listen mode is not implemented yet.");
                return false;
            }

            return startNRFBeacon();

        case NodeRadio::BLE:
            services.logger.error("BLE node sessions are not implemented yet.");
            return false;

        case NodeRadio::CC1101:
            services.logger.error("CC1101 node sessions are not implemented yet.");
            return false;

        default:
            services.logger.error("Unknown node radio.");
            return false;
    }
}

/**
 * @brief Stops the currently active radio session.
 */
void NodeApplication::stopSession()
{
    if (!sessionRunning) return;

    switch (activeConfig.radio) {
        case NodeRadio::NRF24:
            if (services.nrf.isRunning()) services.nrf.stop();
            break;

        case NodeRadio::BLE:
        case NodeRadio::CC1101:
        default:
            break;
    }

    sessionRunning = false;
    lastBeacon = 0;

    setNodeStatus(configReceived ? NodeStatus::ConfigReceived : NodeStatus::Idle);

    services.logger.info("Node session stopped.");
}

/**
 * @brief Starts an NRF24 beacon session.
 *
 * @return true when the beacon session started successfully.
 */
bool NodeApplication::startNRFBeacon()
{
    if (activeConfig.channel >= NRFManager::NRF_CHANNEL_COUNT) {
        services.logger.error("Invalid NRF beacon channel.");
        return false;
    }

    if (activeConfig.interval == 0) {
        services.logger.error("Invalid NRF beacon interval.");
        return false;
    }

    if (!services.startNRF()) return false;

    if (!services.nrf.configureTransmitter(activeConfig.channel, NRF_ADDRESS)) {
        services.logger.error("Failed to configure NRF transmitter.");
        services.nrf.stop();
        return false;
    }

    lastBeacon = millis();

    services.logger.info(
        String("NRF beacon started on channel ") +
        activeConfig.channel +
        " every " +
        activeConfig.interval +
        " ms."
    );

    return true;
}

/**
 * @brief Updates an active NRF24 beacon session.
 */
void NodeApplication::updateNRFBeacon()
{
    if (millis() - lastBeacon < activeConfig.interval) return;

    lastBeacon = millis();

    if (!services.nrf.send(NRF_BEACON_PAYLOAD, strlen(NRF_BEACON_PAYLOAD) + 1))
        services.logger.warning("NRF beacon transmission failed.");
}

/**
 * @brief Updates the node status exposed through BLE.
 *
 * @param status New node status.
 */
void NodeApplication::setNodeStatus(NodeStatus status)
{
    uint8_t value = static_cast<uint8_t>(status);

    if (!services.ble.setServerCharacteristicValue(NodeProtocol::STATUS_UUID, &value, sizeof(value)))
        services.logger.error("Failed to update node BLE status.");
}