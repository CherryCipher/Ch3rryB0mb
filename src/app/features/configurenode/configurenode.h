/**
 * @file configurenode.h
 * @brief Declaration of the Ch3rryN0de configuration feature.
 *
 * This feature discovers Ch3rryN0de devices through BLE, manages node
 * selection and stores the configuration for the selected node.
 */

#pragma once

#include <Arduino.h>

#include "services/BLE/blemanager.h"
#include "node/nodeprotocol.h"

class Services;

/**
 * @class ConfigureNode
 * @brief Discovers, selects and configures Ch3rryN0de devices.
 *
 * ConfigureNode acts as the application layer between the node UI and
 * the shared BLEManager.
 *
 * BLEManager remains generic and exposes discovered BLE devices while
 * ConfigureNode filters these results using the Ch3rryN0de service UUID.
 */
class ConfigureNode
{
public:
    /**
     * @brief BLE service UUID advertised by Ch3rryN0de devices.
     */
    static constexpr const char* NODE_SERVICE_UUID = "6f17c001-9d8b-4b4d-a2e3-43d9a3c30001";

    /**
     * @brief Maximum number of nodes stored during discovery.
     */
    static constexpr uint8_t MAX_NODES = 10;

    /**
     * @brief Constructs the ConfigureNode feature.
     *
     * @param services Reference to the shared application services.
     */
    explicit ConfigureNode(Services& services);

    /**
     * @brief Starts scanning for Ch3rryN0de devices.
     *
     * Existing BLE scan results and node results are cleared before a
     * new scan begins.
     *
     * @return true when scanning started successfully.
     */
    bool startScan();

    /**
     * @brief Stops the active node scan.
     */
    void stopScan();

    /**
     * @brief Returns whether BLE scanning is active.
     *
     * @return true when a node scan is active.
     */
    bool isScanning() const;

    /**
     * @brief Updates the filtered node list from BLE scan results.
     */
    void update();

    /**
     * @brief Clears all discovered nodes.
     */
    void clearNodes();

    /**
     * @brief Returns the number of discovered nodes.
     *
     * @return Number of discovered Ch3rryN0de devices.
     */
    uint8_t getNodeCount() const;

    /**
     * @brief Returns a discovered node.
     *
     * @param index Index in the filtered node list.
     *
     * @return Constant reference to the discovered BLE device.
     */
    const BLEDeviceInfo& getNode(uint8_t index) const;

    /**
     * @brief Selects a discovered node.
     *
     * @param index Index in the filtered node list.
     *
     * @return true when the node was selected successfully.
     */
    bool selectNode(uint8_t index);

    /**
     * @brief Clears the currently selected node.
     */
    void clearSelectedNode();

    /**
     * @brief Returns whether a node is currently selected.
     *
     * @return true when a valid node is selected.
     */
    bool hasSelectedNode() const;

    /**
     * @brief Returns the currently selected node.
     *
     * @return Constant reference to the selected BLE device.
     */
    const BLEDeviceInfo& getSelectedNode() const;

    /**
     * @brief Returns the current node configuration.
     *
     * @return Reference to the current node configuration.
     */
    NodeConfig& getConfig();

    /**
     * @brief Returns the current node configuration.
     *
     * @return Constant reference to the current node configuration.
     */
    const NodeConfig& getConfig() const;

    /**
     * @brief Returns a display name for a node radio.
     *
     * @param radio Radio value to convert.
     *
     * @return Human-readable radio name.
     */
    static const char* getRadioName(NodeRadio radio);

    /**
     * @brief Returns a display name for a node mode.
     *
     * @param mode Mode value to convert.
     *
     * @return Human-readable mode name.
     */
    static const char* getModeName(NodeMode mode);

    /**
     * @brief Connects to the selected node and sends its current configuration.
     *
     * @return true when the configuration was sent successfully.
     */
    bool sendConfig();

private:
    /**
     * @brief Shared application services.
     */
    Services& services;

    /**
     * @brief BLEManager indexes corresponding to discovered nodes.
     */
    uint8_t nodeIndexes[MAX_NODES] = {};

    /**
     * @brief Number of discovered nodes.
     */
    uint8_t nodeCount = 0;

    /**
     * @brief Selected node index in the filtered node list.
     *
     * A value of -1 means that no node is currently selected.
     */
    int8_t selectedNodeIndex = -1;

    /**
     * @brief Current configuration for the selected node.
     */
    NodeConfig config;

    /**
     * @brief Checks whether a BLE device is a Ch3rryN0de.
     *
     * @param device BLE device to inspect.
     *
     * @return true when the Ch3rryN0de service UUID is advertised.
     */
    bool isNode(const BLEDeviceInfo& device) const;

    /**
     * @brief Checks whether a node is already stored.
     *
     * @param address BLE address to search for.
     *
     * @return true when the node is already present.
     */
    bool hasNode(const String& address) const;
};