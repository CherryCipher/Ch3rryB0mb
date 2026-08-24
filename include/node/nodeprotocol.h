/**
 * @file nodeprotocol.h
 * @brief Shared Ch3rryN0de communication protocol.
 *
 * Defines the BLE service identifiers, supported radios, node modes and
 * configuration structures shared between Ch3rryB0mb and Ch3rryN0de.
 */

#pragma once

#include <stdint.h>

/**
 * @namespace NodeProtocol
 * @brief Shared protocol definitions used by C3B0 and C3N0.
 */
namespace NodeProtocol
{
    /**
     * @brief Current node protocol version.
     */
    static constexpr uint8_t VERSION = 1;

    /**
     * @brief BLE service UUID used to identify Ch3rryN0de devices.
     */
    static constexpr const char* SERVICE_UUID = "6f17c001-9d8b-4b4d-a2e3-43d9a3c30001";

    /**
     * @brief BLE characteristic used to transfer node configuration.
     */
    static constexpr const char* CONFIG_UUID = "6f17c002-9d8b-4b4d-a2e3-43d9a3c30001";

    /**
     * @brief BLE characteristic used for node commands.
     */
    static constexpr const char* COMMAND_UUID = "6f17c003-9d8b-4b4d-a2e3-43d9a3c30001";

    /**
     * @brief BLE characteristic used for node status updates.
     */
    static constexpr const char* STATUS_UUID = "6f17c004-9d8b-4b4d-a2e3-43d9a3c30001";

    /**
     * @brief Shared NRF24 address used by Ch3rryN0de lab sessions.
     */
    static constexpr uint8_t NRF_ADDRESS[6] = "C3N0";

    /**
     * @brief Default payload transmitted by NRF beacon mode.
     */
    static constexpr const char* NRF_BEACON_PAYLOAD = "C3N0";
}

/**
 * @enum NodeRadio
 * @brief Radios that can be controlled on a Ch3rryN0de.
 */
enum class NodeRadio : uint8_t
{
    BLE = 0,
    NRF24 = 1,
    CC1101 = 2
};

/**
 * @enum NodeMode
 * @brief Operating modes supported by a Ch3rryN0de radio.
 */
enum class NodeMode : uint8_t
{
    Beacon = 0,
    Listen = 1
};

/**
 * @enum NodeCommand
 * @brief Commands that can be sent to a configured node.
 */
enum class NodeCommand : uint8_t
{
    Start = 1,
    Stop = 2
};

/**
 * @enum NodeStatus
 * @brief Status values reported by a Ch3rryN0de.
 */
enum class NodeStatus : uint8_t
{
    Idle = 0,
    ConfigReceived = 1,
    Ready = 2,
    Running = 3,
    Error = 255
};

/**
 * @struct NodeConfig
 * @brief Binary configuration transferred from C3B0 to C3N0.
 *
 * Fields that do not apply to the selected radio are ignored.
 */
struct __attribute__((packed)) NodeConfig
{
    uint8_t version = NodeProtocol::VERSION;
    NodeRadio radio = NodeRadio::NRF24;
    NodeMode mode = NodeMode::Listen;
    uint8_t channel = 30;
    float frequency = 868.300f;
    uint16_t interval = 500;
};