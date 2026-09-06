/**
 * @file wifimanager.h
 * @brief Wi-Fi service interface for Ch3rryB0mb.
 */

#pragma once

#include <WiFi.h>
#include <esp_wifi.h>

#include "WiFiConfig.h"
#include "../logger/logger.h"

/**
 * @struct WiFiNetwork
 * @brief Contains information about a discovered Wi-Fi network.
 *
 * Represents a single Access Point discovered during a Wi-Fi scan.
 * The structure contains the network information required by
 * Ch3rryB0mb features without exposing the ESP32 WiFi scanning API.
 */
struct WiFiNetwork
{
    String ssid;
    String bssid;
    int32_t rssi;
    uint8_t channel;
    wifi_auth_mode_t encryption;
};

/**
 * @enum WiFiPacketProtocol
 * @brief Identifies a protocol discovered inside a captured Wi-Fi frame.
 */
enum class WiFiPacketProtocol : uint8_t
{
    None,
    Protected,
    ARP,
    IPv4,
    IPv6,
    ICMP,
    TCP,
    UDP,
    DNS,
    MDNS,
    DHCP,
    HTTP,
    HTTPS
};

/**
 * @enum WiFiPacketCaptureFilter
 * @brief Defines which captured Wi-Fi packets are retained.
 *
 * The filter is applied after packet metadata has been extracted but before
 * the packet is written to the capture ring buffer.
 */
enum class WiFiPacketCaptureFilter : uint8_t
{
    Interesting,
    Data,
    Management,
    All
};

/**
 * @struct WiFiPacketInfo
 * @brief Contains metadata extracted from a captured Wi-Fi frame.
 *
 * Only compact packet metadata is stored. Raw frame payloads are not retained,
 * keeping the capture buffer at a fixed and predictable memory size.
 */
struct WiFiPacketInfo
{
    uint32_t id = 0;
    int8_t rssi = 0;
    uint16_t length = 0;
    uint8_t channel = 0;
    uint8_t frameType = 0;
    uint8_t frameSubtype = 0;
    bool protectedFrame = false;

    uint8_t receiver[6] = {};
    uint8_t transmitter[6] = {};
    uint8_t address3[6] = {};

    WiFiPacketProtocol protocol = WiFiPacketProtocol::None;

    uint32_t sourceIP = 0;
    uint32_t destinationIP = 0;

    uint16_t sourcePort = 0;
    uint16_t destinationPort = 0;
};

/******************************************************************************
 * WiFiManager
 *
 * Responsible for all Wi-Fi functionality within Ch3rryB0mb.
 *
 * Features
 * --------
 * - Access Point (AP) mode
 * - Station (Client) mode
 * - Wi-Fi Scanning
 * - Captive Portal
 * - Web Dashboard
 * - Future wireless extensions
 *
 * Design
 * ------
 * Applications should NEVER communicate directly with the ESP32 Wi-Fi
 * library (WiFi.h). All Wi-Fi operations should be performed through the
 * WiFiManager.
 *
 ******************************************************************************/
class WiFiManager
{
public:
    /**
     * @brief Constructs a new WiFiManager.
     *
     * @param logger Reference to the application's Logger.
     */
    explicit WiFiManager(Logger& logger);

    /**
     * @brief Initializes the WiFiManager.
     *
     * This prepares the manager for use but does not enable any Wi-Fi mode.
     * Applications should explicitly start the desired mode (AP, STA, etc.).
     *
     * @return true if initialization succeeded.
     */
    bool start();

    /**
     * @brief Starts the ESP32 in Access Point mode.
     *
     * Creates a configurable Wi-Fi Access Point using the supplied settings.
     *
     * @param config Access Point configuration.
     * @return true if the AP started successfully.
     * @return false otherwise.
     */
    bool startAP(const WiFiAPConfig& config);

    /**
     * @brief Stops the Access Point.
     *
     * Disconnects all connected clients and disables AP mode.
     */
    void stopAP();

    /**
     * @brief Returns whether the Access Point is currently running.
     *
     * @return true when AP mode is active.
     */
    bool isAPRunning() const;

    /**
     * @brief Returns the IP address of the Access Point.
     *
     * Usually this will be 192.168.4.1.
     *
     * @return Access Point IP address.
     */
    IPAddress getAPIP() const;

    /**
     * @brief Scans for nearby Wi-Fi networks.
     *
     * Performs a Wi-Fi network scan and stores the discovered Access Points
     * internally for later retrieval.
     *
     * @return true if the scan completed successfully.
     * @return false if the scan failed.
     */
    bool scanNetworks();

    /**
     * @brief Returns the number of stored Wi-Fi scan results.
     *
     * @return Number of networks discovered during the most recent scan.
     */
    int getNetworkCount() const;

    /**
     * @brief Returns a Wi-Fi network from the stored scan results.
     *
     * @param index Index of the requested network.
     * @return Constant reference to the requested WiFiNetwork.
     */
    const WiFiNetwork& getNetwork(int index) const;

    /**
     * @brief Starts a Station mode connection to a Wi-Fi network.
     *
     * Starts the connection process without waiting for it to complete.
     * Connection state can be checked using isConnected().
     *
     * @param ssid SSID of the network.
     * @param password Password of the network. May be empty for open networks.
     *
     * @return true if the connection attempt was started.
     * @return false if the supplied SSID is empty.
     */
    bool connect(const String& ssid, const String& password);

    /**
     * @brief Disconnects the Station interface from the current Wi-Fi network.
     *
     * The Wi-Fi subsystem remains enabled so another connection or scan can
     * be started without fully reinitializing the radio.
     */
    void disconnect();

    /**
     * @brief Returns whether the Station interface is connected.
     *
     * @return true when connected to a Wi-Fi network.
     */
    bool isConnected() const;

    /**
     * @brief Returns the SSID of the currently connected Wi-Fi network.
     *
     * @return Connected network SSID.
     */
    String getConnectedSSID() const;

    /**
     * @brief Returns the Station interface IP address.
     *
     * @return Local IP address assigned to the Station interface.
     */
    IPAddress getLocalIP() const;

    /**
     * @brief Returns the signal strength of the active Station connection.
     *
     * Retrieves the RSSI reported by the ESP32 Wi-Fi subsystem for the
     * currently connected Access Point.
     *
     * @return Current Wi-Fi signal strength in dBm.
     */
    int32_t getRSSI() const;

    /**
     * @brief Stops the WiFiManager and disables all Wi-Fi functionality.
     *
     * @return true when Wi-Fi has been shut down.
     */
    bool stop();

    /**
     * @brief Starts passive Wi-Fi packet capture.
     *
     * Enables ESP32 promiscuous receive mode while preserving the active
     * Station connection.
     *
     * Captured frames are reduced to compact metadata and stored in a fixed
     * ring buffer. No packet payload is retained after processing.
     *
     * @return true when packet capture started successfully.
     * @return false when Wi-Fi is not connected or capture could not start.
     */
    bool startPacketCapture();

    /**
     * @brief Stops passive Wi-Fi packet capture.
     *
     * Disables promiscuous receive mode without disconnecting the active
     * Station connection.
     */
    void stopPacketCapture();

    /**
     * @brief Returns whether passive packet capture is active.
     *
     * @return true while packet capture is enabled.
     */
    bool isPacketCaptureRunning() const;

    /**
     * @brief Clears all stored packet metadata.
     *
     * Does not stop an active packet capture.
     */
    void clearCapturedPackets();

    /**
     * @brief Returns the number of packet records currently stored.
     *
     * @return Number of valid records in the fixed packet ring buffer.
     */
    uint8_t getCapturedPacketCount() const;

    /**
     * @brief Returns a captured packet.
     *
     * Packets are indexed newest first. Index zero represents the most recently
     * captured packet.
     *
     * A copy is returned so the caller is not exposed to the capture callback
     * modifying the ring buffer concurrently.
     *
     * @param index Packet index, newest first.
     * @return Copy of the requested packet metadata.
     */
    WiFiPacketInfo getCapturedPacket(uint8_t index) const;

    /**
     * @brief Sets the active packet capture filter.
     *
     * The selected filter determines which captured Wi-Fi packets are retained
     * in the packet ring buffer.
     *
     * @param filter Packet capture filter to activate.
     */
    void setPacketCaptureFilter(WiFiPacketCaptureFilter filter);

    /**
     * @brief Returns the active packet capture filter.
     *
     * @return Currently active packet capture filter.
     */
    WiFiPacketCaptureFilter getPacketCaptureFilter() const;

    /**
     * @brief Releases all packet capture memory.
     *
     * Stops active capture, clears stored packet records and releases the
     * dynamically allocated ring buffer.
     *
     * The active Wi-Fi Station connection is preserved.
     */
    void releasePacketCapture();

private:
    /**
     * @brief Maximum number of Wi-Fi scan results stored by the manager.
     */
    static constexpr int MAX_SCAN_RESULTS = 30;

    /**
     * @brief Networks discovered during the most recent Wi-Fi scan.
     *
     * Allocated dynamically when a scan is performed to prevent the scan
     * cache from permanently consuming static DRAM.
     */
    WiFiNetwork* networks = nullptr;

    /**
     * @brief Maximum number of packet records retained by Packet Viewer.
     *
     * The capture buffer is allocated dynamically only while Packet Viewer
     * is in use, preventing packet storage from permanently consuming static
     * DRAM.
     */
    static constexpr uint8_t MAX_CAPTURED_PACKETS = 16;

    /**
     * @brief Dynamically allocated packet capture ring buffer.
     *
     * The buffer is allocated when packet capture is first started and can
     * be released when Packet Viewer is closed.
     */
    WiFiPacketInfo* capturedPackets = nullptr;

    /**
     * @brief Index where the next captured packet will be written.
     */
    uint8_t packetWriteIndex = 0;

    /**
     * @brief Number of valid packet records currently stored.
     */
    uint8_t capturedPacketCount = 0;

    /**
     * @brief Sequence number assigned to captured packets.
     */
    uint32_t capturedPacketSequence = 0;

    /**
     * @brief Indicates whether promiscuous packet capture is active.
     */
    bool packetCaptureRunning = false;

    /**
     * @brief Protects packet ring buffer access between Wi-Fi and application tasks.
     */
    mutable portMUX_TYPE packetMux = portMUX_INITIALIZER_UNLOCKED;

    /**
     * @brief WiFiManager instance receiving promiscuous Wi-Fi callbacks.
     */
    static WiFiManager* packetCaptureInstance;

    /**
     * @brief ESP32 promiscuous Wi-Fi receive callback.
     *
     * @param buffer Packet data supplied by the ESP32 Wi-Fi driver.
     * @param type Promiscuous packet type.
     */
    static void promiscuousCallback(void* buffer, wifi_promiscuous_pkt_type_t type);

    /**
     * @brief Extracts compact metadata from a received Wi-Fi frame.
     *
     * Protected data frames are never inspected beyond the visible 802.11
     * metadata. Plaintext frames may be inspected for publicly visible
     * protocols such as ARP, IPv4, ICMP, UDP and DNS.
     *
     * @param packet Received promiscuous Wi-Fi packet.
     * @param type Promiscuous packet type.
     */
    void capturePacket(const wifi_promiscuous_pkt_t* packet, wifi_promiscuous_pkt_type_t type);

    /**
     * @brief Stores a packet record in the fixed capture ring buffer.
     *
     * @param packet Packet metadata to store.
     */
    void storeCapturedPacket(const WiFiPacketInfo& packet);

    /**
     * @brief Active filter applied before packets enter the capture ring buffer.
     *
     * Interesting is used by default to suppress common management and control
     * frame noise while retaining useful data traffic.
     */
    WiFiPacketCaptureFilter packetCaptureFilter = WiFiPacketCaptureFilter::Interesting;

    /**
     * @brief Returns whether a captured packet matches the active filter.
     *
     * @param packet Parsed packet metadata.
     * @return true when the packet should be retained.
     * @return false when the packet should be discarded.
     */
    bool matchesPacketCaptureFilter(const WiFiPacketInfo& packet) const;

    /**
     * @brief Number of valid networks currently stored.
     */
    int networkCount = 0;

    /**
     * @brief Reference to the application's Logger instance.
     *
     * Used for logging messages related to Wi-Fi operations.
     */
    Logger& logger;
};