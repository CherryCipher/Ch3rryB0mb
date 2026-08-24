/**
 * @file configurenode.cpp
 * @brief Implementation of the Ch3rryN0de configuration feature.
 */

#include "configurenode.h"

#include "services/services.h"

/**
 * @brief Constructs the ConfigureNode feature.
 *
 * @param services Reference to the shared application services.
 */
ConfigureNode::ConfigureNode(Services& services)
    : services(services)
{
}

/**
 * @brief Starts scanning for Ch3rryN0de devices.
 *
 * @return true when scanning started successfully.
 */
bool ConfigureNode::startScan()
{
    clearNodes();

    if (!services.ble.isRunning() && !services.ble.start()) return false;

    return services.ble.startScan();
}

/**
 * @brief Stops the active node scan.
 */
void ConfigureNode::stopScan()
{
    services.ble.stopScan();
}

/**
 * @brief Returns whether BLE scanning is active.
 *
 * @return true when scanning is active.
 */
bool ConfigureNode::isScanning() const
{
    return services.ble.isScanning();
}

/**
 * @brief Updates the filtered node list from BLE scan results.
 */
void ConfigureNode::update()
{
    uint8_t deviceCount = services.ble.getDeviceCount();

    for (uint8_t i = 0; i < deviceCount; i++) {
        const BLEDeviceInfo& device = services.ble.getDevice(i);

        if (!isNode(device)) continue;
        if (hasNode(device.address)) continue;
        if (nodeCount >= MAX_NODES) break;

        nodeIndexes[nodeCount++] = i;
    }
}

/**
 * @brief Clears all discovered nodes.
 */
void ConfigureNode::clearNodes()
{
    nodeCount = 0;
    selectedNodeIndex = -1;

    for (uint8_t i = 0; i < MAX_NODES; i++) nodeIndexes[i] = 0;
}

/**
 * @brief Returns the number of discovered nodes.
 *
 * @return Number of discovered nodes.
 */
uint8_t ConfigureNode::getNodeCount() const
{
    return nodeCount;
}

/**
 * @brief Returns a discovered node.
 *
 * @param index Index in the filtered node list.
 *
 * @return Constant reference to the discovered BLE device.
 */
const BLEDeviceInfo& ConfigureNode::getNode(uint8_t index) const
{
    return services.ble.getDevice(nodeIndexes[index]);
}

/**
 * @brief Selects a discovered node.
 *
 * @param index Index in the filtered node list.
 *
 * @return true when the node was selected successfully.
 */
bool ConfigureNode::selectNode(uint8_t index)
{
    if (index >= nodeCount) return false;

    selectedNodeIndex = static_cast<int8_t>(index);

    return true;
}

/**
 * @brief Clears the currently selected node.
 */
void ConfigureNode::clearSelectedNode()
{
    selectedNodeIndex = -1;
}

/**
 * @brief Returns whether a node is currently selected.
 *
 * @return true when a valid node is selected.
 */
bool ConfigureNode::hasSelectedNode() const
{
    return selectedNodeIndex >= 0 && selectedNodeIndex < nodeCount;
}

/**
 * @brief Returns the currently selected node.
 *
 * @return Constant reference to the selected BLE device.
 */
const BLEDeviceInfo& ConfigureNode::getSelectedNode() const
{
    return getNode(static_cast<uint8_t>(selectedNodeIndex));
}

/**
 * @brief Returns the current node configuration.
 *
 * @return Reference to the current node configuration.
 */
NodeConfig& ConfigureNode::getConfig()
{
    return config;
}

/**
 * @brief Returns the current node configuration.
 *
 * @return Constant reference to the current node configuration.
 */
const NodeConfig& ConfigureNode::getConfig() const
{
    return config;
}

/**
 * @brief Returns a display name for a node radio.
 *
 * @param radio Radio value to convert.
 *
 * @return Human-readable radio name.
 */
const char* ConfigureNode::getRadioName(NodeRadio radio)
{
    switch (radio) {
        case NodeRadio::BLE: return "BLE";
        case NodeRadio::NRF24: return "NRF24";
        case NodeRadio::CC1101: return "CC1101";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Returns a display name for a node mode.
 *
 * @param mode Mode value to convert.
 *
 * @return Human-readable mode name.
 */
const char* ConfigureNode::getModeName(NodeMode mode)
{
    switch (mode) {
        case NodeMode::Beacon: return "BEACON";
        case NodeMode::Listen: return "LISTEN";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Checks whether a BLE device is a Ch3rryN0de.
 *
 * @param device BLE device to inspect.
 *
 * @return true when the device advertises the node service UUID.
 */
bool ConfigureNode::isNode(const BLEDeviceInfo& device) const
{
    return device.serviceUUID.equalsIgnoreCase(NODE_SERVICE_UUID);
}

/**
 * @brief Checks whether a node is already stored.
 *
 * @param address BLE address to search for.
 *
 * @return true when the node is already present.
 */
bool ConfigureNode::hasNode(const String& address) const
{
    for (uint8_t i = 0; i < nodeCount; i++)
        if (getNode(i).address == address) return true;

    return false;
}