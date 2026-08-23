/**
 * @file packetviewer.cpp
 * @brief Implementation of the Wi-Fi Packet Viewer feature.
 */

#include "packetviewer.h"
#include "../../../services/services.h"

/**
 * @brief Constructs the Packet Viewer feature.
 *
 * @param services Reference to the shared application services.
 */
PacketViewer::PacketViewer(Services& services) : services(services)
{
}

/**
 * @brief Starts passive packet capture.
 *
 * Starts packet capture through WiFiManager using the currently configured
 * packet capture filter.
 *
 * @return true when capture started successfully.
 * @return false when Wi-Fi is not connected or capture could not start.
 */
bool PacketViewer::start()
{
    if (!services.wifi.isConnected())
    {
        services.logger.error("Packet Viewer: Wi-Fi is not connected.");
        return false;
    }

    if (!services.wifi.startPacketCapture())
    {
        services.logger.error("Packet Viewer: Failed to start capture.");
        return false;
    }

    services.logger.info("Packet Viewer: Capture started.");
    return true;
}

/**
 * @brief Stops passive packet capture.
 */
void PacketViewer::stop()
{
    services.wifi.stopPacketCapture();
    services.logger.info("Packet Viewer: Capture stopped.");
}

/**
 * @brief Returns whether packet capture is active.
 *
 * @return true while capture is running.
 */
bool PacketViewer::isRunning() const
{
    return services.wifi.isPacketCaptureRunning();
}

/**
 * @brief Returns whether Wi-Fi is currently connected.
 *
 * @return true when an active Station connection exists.
 */
bool PacketViewer::isConnected() const
{
    return services.wifi.isConnected();
}

/**
 * @brief Clears all stored packet records.
 */
void PacketViewer::clear()
{
    services.wifi.clearCapturedPackets();
    packetSelected = false;
}

/**
 * @brief Returns the number of retained packet records.
 *
 * @return Number of stored packets.
 */
uint8_t PacketViewer::getPacketCount() const
{
    return services.wifi.getCapturedPacketCount();
}

/**
 * @brief Returns a captured packet.
 *
 * @param index Packet index, newest first.
 * @return Copy of the requested packet.
 */
WiFiPacketInfo PacketViewer::getPacket(uint8_t index) const
{
    return services.wifi.getCapturedPacket(index);
}

/**
 * @brief Selects a captured packet.
 *
 * @param index Packet index, newest first.
 * @return true when the packet was selected.
 * @return false when the index is invalid.
 */
bool PacketViewer::selectPacket(uint8_t index)
{
    if (index >= getPacketCount())
        return false;

    selectedPacket = getPacket(index);
    packetSelected = true;

    return true;
}

/**
 * @brief Returns whether a packet has been selected.
 *
 * @return true when a selected packet is available.
 */
bool PacketViewer::hasSelectedPacket() const
{
    return packetSelected;
}

/**
 * @brief Returns the selected packet.
 *
 * @return Constant reference to the selected packet.
 */
const WiFiPacketInfo& PacketViewer::getSelectedPacket() const
{
    return selectedPacket;
}

/**
 * @brief Sets the active packet capture filter.
 *
 * Updates the filter directly in WiFiManager and clears previously retained
 * packet records so all subsequently displayed packets match the selected
 * category.
 *
 * @param filter Packet capture filter to activate.
 */
void PacketViewer::setFilter(WiFiPacketCaptureFilter filter)
{
    if (services.wifi.getPacketCaptureFilter() == filter)
        return;

    services.wifi.setPacketCaptureFilter(filter);
    clear();

    services.logger.info("Packet Viewer: Filter changed to " + String(getFilterName(filter)));
}

/**
 * @brief Returns the active packet capture filter.
 *
 * @return Currently active packet capture filter.
 */
WiFiPacketCaptureFilter PacketViewer::getFilter() const
{
    return services.wifi.getPacketCaptureFilter();
}

/**
 * @brief Returns a readable name for a packet capture filter.
 *
 * @param filter Packet capture filter.
 * @return Human-readable filter name.
 */
const char* PacketViewer::getFilterName(WiFiPacketCaptureFilter filter)
{
    switch (filter)
    {
        case WiFiPacketCaptureFilter::Interesting: return "INTERESTING";
        case WiFiPacketCaptureFilter::Data: return "DATA";
        case WiFiPacketCaptureFilter::Management: return "MGMT";
        case WiFiPacketCaptureFilter::All: return "ALL";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Returns a readable protocol name.
 *
 * @param protocol Packet protocol.
 * @return Human-readable protocol name.
 */
const char* PacketViewer::getProtocolName(WiFiPacketProtocol protocol)
{
    switch (protocol)
    {
        case WiFiPacketProtocol::Protected: return "ENCRYPTED";
        case WiFiPacketProtocol::ARP: return "ARP";
        case WiFiPacketProtocol::IPv4: return "IPv4";
        case WiFiPacketProtocol::IPv6: return "IPv6";
        case WiFiPacketProtocol::ICMP: return "ICMP";
        case WiFiPacketProtocol::TCP: return "TCP";
        case WiFiPacketProtocol::UDP: return "UDP";
        case WiFiPacketProtocol::DNS: return "DNS";
        case WiFiPacketProtocol::MDNS: return "mDNS";
        case WiFiPacketProtocol::DHCP: return "DHCP";
        case WiFiPacketProtocol::HTTP: return "HTTP";
        case WiFiPacketProtocol::HTTPS: return "HTTPS";
        default: return "-";
    }
}

/**
 * @brief Returns a readable 802.11 frame type name.
 *
 * @param frameType 802.11 frame type.
 * @return Human-readable frame type name.
 */
const char* PacketViewer::getFrameTypeName(uint8_t frameType)
{
    switch (frameType)
    {
        case 0: return "MGMT";
        case 1: return "CTRL";
        case 2: return "DATA";
        default: return "OTHER";
    }
}

/**
 * @brief Returns a readable 802.11 frame subtype name.
 *
 * @param frameType 802.11 frame type.
 * @param subtype 802.11 frame subtype.
 * @return Human-readable frame subtype name.
 */
const char* PacketViewer::getFrameSubtypeName(uint8_t frameType, uint8_t subtype)
{
    if (frameType == 0)
    {
        switch (subtype)
        {
            case 0: return "ASSOC REQ";
            case 1: return "ASSOC RESP";
            case 4: return "PROBE REQ";
            case 5: return "PROBE RESP";
            case 8: return "BEACON";
            case 10: return "DISASSOC";
            case 11: return "AUTH";
            case 12: return "DEAUTH";
            default: return "MANAGEMENT";
        }
    }

    if (frameType == 1)
    {
        switch (subtype)
        {
            case 8: return "BLOCK ACK REQ";
            case 9: return "BLOCK ACK";
            case 10: return "PS-POLL";
            case 11: return "RTS";
            case 12: return "CTS";
            case 13: return "ACK";
            default: return "CONTROL";
        }
    }

    if (frameType == 2)
    {
        if (subtype & 0x08)
            return "QOS DATA";

        return "DATA";
    }

    return "UNKNOWN";
}

/**
 * @brief Formats a MAC address for display.
 *
 * @param address Six-byte MAC address.
 * @return Formatted MAC address.
 */
String PacketViewer::formatMAC(const uint8_t address[6])
{
    char buffer[18];

    snprintf(
        buffer,
        sizeof(buffer),
        "%02X:%02X:%02X:%02X:%02X:%02X",
        address[0], address[1], address[2],
        address[3], address[4], address[5]
    );

    return String(buffer);
}

/**
 * @brief Formats an IPv4 address for display.
 *
 * @param address IPv4 address in network byte order.
 * @return Formatted IPv4 address.
 */
String PacketViewer::formatIP(uint32_t address)
{
    return String((address >> 24) & 0xFF) + "." +
           String((address >> 16) & 0xFF) + "." +
           String((address >> 8) & 0xFF) + "." +
           String(address & 0xFF);
}

/**
 * @brief Releases Packet Viewer capture resources.
 *
 * Stops packet capture and releases its dynamically allocated packet
 * buffer without disconnecting Wi-Fi.
 */
void PacketViewer::shutdown()
{
    services.wifi.releasePacketCapture();
    packetSelected = false;

    services.logger.info("Packet Viewer: Capture resources released.");
}