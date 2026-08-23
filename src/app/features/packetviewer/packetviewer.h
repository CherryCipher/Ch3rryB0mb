/**
 * @file packetviewer.h
 * @brief Wi-Fi Packet Viewer feature interface.
 */

#pragma once

#include "../../../services/wifi/wifimanager.h"

class Services;

/**
 * @class PacketViewer
 * @brief Provides passive Wi-Fi packet inspection functionality.
 *
 * PacketViewer exposes captured Wi-Fi frame metadata to the application
 * without interacting directly with the ESP32 Wi-Fi driver.
 *
 * Packet capture is performed through WiFiManager and requires an active
 * Station connection.
 *
 * Protected Wi-Fi payloads are never decrypted or inspected. Plaintext
 * payloads may be classified into recognizable protocols such as ARP,
 * IPv4, ICMP, UDP, TCP, DNS and DHCP.
 */
class PacketViewer
{
public:
    /**
     * @brief Constructs the Packet Viewer feature.
     *
     * @param services Reference to the shared application services.
     */
    explicit PacketViewer(Services& services);

    /**
     * @brief Starts passive packet capture.
     *
     * Uses the packet filter currently configured in WiFiManager.
     *
     * @return true when packet capture started successfully.
     * @return false when Wi-Fi is not connected or capture could not start.
     */
    bool start();

    /**
     * @brief Stops passive packet capture.
     *
     * The active Wi-Fi Station connection remains unchanged.
     */
    void stop();

    /**
     * @brief Returns whether packet capture is active.
     *
     * @return true while packet capture is running.
     */
    bool isRunning() const;

    /**
     * @brief Returns whether Wi-Fi is currently connected.
     *
     * @return true when an active Station connection exists.
     */
    bool isConnected() const;

    /**
     * @brief Clears all stored packet records.
     */
    void clear();

    /**
     * @brief Returns the number of retained packet records.
     *
     * @return Number of packets currently stored in the capture ring buffer.
     */
    uint8_t getPacketCount() const;

    /**
     * @brief Returns a captured packet.
     *
     * Packets are indexed newest first.
     *
     * @param index Packet index.
     * @return Copy of the requested packet.
     */
    WiFiPacketInfo getPacket(uint8_t index) const;

    /**
     * @brief Selects a captured packet for detailed inspection.
     *
     * The packet is copied so its details remain valid even when the
     * capture ring buffer later overwrites the original record.
     *
     * @param index Packet index, newest first.
     * @return true when the packet was selected.
     * @return false when the index is invalid.
     */
    bool selectPacket(uint8_t index);

    /**
     * @brief Returns whether a packet has been selected.
     *
     * @return true when a selected packet is available.
     */
    bool hasSelectedPacket() const;

    /**
     * @brief Returns the selected packet.
     *
     * @return Constant reference to the selected packet copy.
     */
    const WiFiPacketInfo& getSelectedPacket() const;

    /**
     * @brief Sets the active packet capture filter.
     *
     * Updates the filter directly in WiFiManager and clears previously
     * retained packet records so the visible list immediately represents
     * the selected packet category.
     *
     * @param filter Packet capture filter to activate.
     */
    void setFilter(WiFiPacketCaptureFilter filter);

    /**
     * @brief Returns the active packet capture filter.
     *
     * @return Currently active packet capture filter.
     */
    WiFiPacketCaptureFilter getFilter() const;

    /**
     * @brief Returns a readable name for a packet capture filter.
     *
     * @param filter Packet capture filter.
     * @return Human-readable filter name.
     */
    static const char* getFilterName(WiFiPacketCaptureFilter filter);

    /**
     * @brief Returns a readable protocol name.
     *
     * @param protocol Packet protocol.
     * @return Short human-readable protocol name.
     */
    static const char* getProtocolName(WiFiPacketProtocol protocol);

    /**
     * @brief Returns a readable 802.11 frame type name.
     *
     * @param frameType 802.11 frame type.
     * @return Human-readable frame type name.
     */
    static const char* getFrameTypeName(uint8_t frameType);

    /**
     * @brief Returns a readable 802.11 frame subtype name.
     *
     * @param frameType 802.11 frame type.
     * @param subtype 802.11 frame subtype.
     * @return Human-readable frame subtype name.
     */
    static const char* getFrameSubtypeName(uint8_t frameType, uint8_t subtype);

    /**
     * @brief Formats a MAC address for display.
     *
     * @param address Six-byte MAC address.
     * @return Formatted MAC address.
     */
    static String formatMAC(const uint8_t address[6]);

    /**
     * @brief Formats an IPv4 address for display.
     *
     * @param address IPv4 address in network byte order.
     * @return Formatted IPv4 address.
     */
    static String formatIP(uint32_t address);

    /**
     * @brief Releases Packet Viewer capture resources.
     *
     * Stops packet capture and releases the dynamically allocated packet
     * buffer while preserving the active Wi-Fi connection.
     */
    void shutdown();

private:
    /**
     * @brief Reference to the shared application services.
     */
    Services& services;

    /**
     * @brief Copy of the packet selected for detailed inspection.
     */
    WiFiPacketInfo selectedPacket;

    /**
     * @brief Indicates whether a selected packet is available.
     */
    bool packetSelected = false;
};