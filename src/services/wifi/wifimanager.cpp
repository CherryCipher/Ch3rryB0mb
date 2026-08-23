/**
 * @file wifimanager.cpp
 * @brief Implementation of the Ch3rryB0mb Wi-Fi service.
 */

#include "WiFiManager.h"
#include <cstring>
#include <new>

WiFiManager* WiFiManager::packetCaptureInstance = nullptr;

/**
 * @brief Constructs a new WiFiManager.
 *
 * Stores a reference to the application's Logger.
 *
 * @param logger Reference to the application's Logger.
 */
WiFiManager::WiFiManager(Logger& logger) : logger(logger)
{
}

/**
 * @brief Initializes the WiFiManager.
 *
 * Prepares the WiFiManager for use without initializing or enabling the
 * ESP32 Wi-Fi subsystem. Individual Wi-Fi features explicitly activate
 * the required Wi-Fi mode when needed.
 *
 * @return true when the manager is ready for use.
 */
bool WiFiManager::start()
{
    logger.info("WiFiManager started.");
    return true;
}

/**
 * @brief Returns if the C3B0 is in Access Point mode.
 *
 * Uses the ESP32 WiFi library to check if the device is currently in Access Point mode.
 * WIFI_AP is the mode for Access Point only, while WIFI_AP_STA is the mode for both Access Point and Station.
 *
 * @return true when AP mode is active.
 */
bool WiFiManager::isAPRunning() const
{
    return (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA);
}

/**
 * @brief Starts the ESP32 in Access Point (SoftAP) mode.
 *
 * Configures the Wi-Fi radio as an Access Point using the supplied
 * configuration and begins broadcasting the configured SSID.
 *
 * SoftAP() is a native function from the ESP32 Wi-Fi library that handles the underlying implementation of starting an AP.
 *
 * If you want the AP to serve a web page, you will need to set up a web server separately.
 * This function only starts the AP and does not handle any web server functionality.
 * Webserver is started though webservermanager, which is a separate service that can be started after the AP is running.
 *
 * @param config The Access Point configuration.
 * @return true if the Access Point started successfully.
 * @return false if the SoftAP failed to start.
 */
bool WiFiManager::startAP(const WiFiAPConfig& config)
{
    WiFi.mode(WIFI_AP);
    logger.info("WiFi mode set to WIFI_AP. Starting SoftAP");

    bool success = WiFi.softAP(
        config.ssid.c_str(),
        config.password.c_str(),
        config.channel,
        config.hidden,
        config.maxClients
    );

    logger.info("SoftAP started with SSID: " + config.ssid + ", Channel: " + String(config.channel) + ", Hidden: " + String(config.hidden) + ", Max Clients: " + String(config.maxClients));

    return success;
}

/**
 * @brief Stops the active Access Point.
 *
 * Disconnects all connected clients and stops the SoftAP. The complete
 * Wi-Fi subsystem remains managed by WiFiManager and can be shut down
 * separately using stop().
 */
void WiFiManager::stopAP()
{
    WiFi.softAPdisconnect(true);
    logger.info("SoftAP stopped. All clients disconnected.");
}

/**
 * @brief Returns the IP address of the Access Point.
 *
 * Calls the ESP32 Wi-Fi library to retrieve the IP address assigned to the Access Point interface.
 *
 * @return Access Point IP address.
 */
IPAddress WiFiManager::getAPIP() const
{
    return WiFi.softAPIP();
}

/**
 * @brief Scans for nearby Wi-Fi networks.
 *
 * Enables Station mode when required and performs a synchronous Wi-Fi scan.
 * Discovered Access Points are stored internally as WiFiNetwork objects.
 *
 * Existing scan results are cleared before starting a new scan.
 *
 * @return true if the scan completed successfully.
 * @return false if the scan failed.
 */
bool WiFiManager::scanNetworks()
{
    networkCount = 0;
    delete[] networks;
    networks = nullptr;

    wifi_mode_t currentMode = WiFi.getMode();

    if (currentMode == WIFI_OFF)
    {
        WiFi.mode(WIFI_STA);
        logger.info("WiFi mode set to WIFI_STA for network scan.");
    }
    else if (currentMode == WIFI_AP)
    {
        WiFi.mode(WIFI_AP_STA);
        logger.info("WiFi mode set to WIFI_AP_STA for network scan.");
    }

    int foundNetworks = WiFi.scanNetworks();

    if (foundNetworks < 0)
    {
        logger.error("Wi-Fi network scan failed.");
        return false;
    }

    networkCount = min(foundNetworks, MAX_SCAN_RESULTS);

    if (networkCount > 0)
    {
        networks = new WiFiNetwork[networkCount];

        if (networks == nullptr)
        {
            WiFi.scanDelete();
            networkCount = 0;
            logger.error("Failed to allocate Wi-Fi scan result cache.");
            return false;
        }
    }

    for (int i = 0; i < networkCount; i++)
    {
        networks[i].ssid = WiFi.SSID(i);
        networks[i].bssid = WiFi.BSSIDstr(i);
        networks[i].rssi = WiFi.RSSI(i);
        networks[i].channel = WiFi.channel(i);
        networks[i].encryption = WiFi.encryptionType(i);
    }

    WiFi.scanDelete();
    logger.info("Wi-Fi scan completed. Networks found: " + String(networkCount));

    return true;
}

/**
 * @brief Returns the number of stored Wi-Fi scan results.
 *
 * @return Number of networks discovered during the most recent scan.
 */
int WiFiManager::getNetworkCount() const
{
    return networkCount;
}

/**
 * @brief Returns a Wi-Fi network from the stored scan results.
 *
 * @param index Index of the requested network.
 * @return Constant reference to the requested WiFiNetwork.
 */
const WiFiNetwork& WiFiManager::getNetwork(int index) const
{
    return networks[index];
}

/**
 * @brief Starts a Station mode connection to a Wi-Fi network.
 *
 * Enables Station mode when required and starts the connection without
 * blocking while the ESP32 associates with the Access Point.
 *
 * @param ssid SSID of the network.
 * @param password Password of the network. May be empty for open networks.
 *
 * @return true if the connection attempt was started.
 * @return false if the supplied SSID is empty.
 */
bool WiFiManager::connect(const String& ssid, const String& password)
{
    if (ssid.length() == 0)
    {
        logger.error("WiFi connection failed: SSID is empty.");
        return false;
    }

    wifi_mode_t currentMode = WiFi.getMode();

    if (currentMode == WIFI_OFF)
    {
        WiFi.mode(WIFI_STA);
        logger.info("WiFi mode set to WIFI_STA for connection.");
    }
    else if (currentMode == WIFI_AP)
    {
        WiFi.mode(WIFI_AP_STA);
        logger.info("WiFi mode set to WIFI_AP_STA for connection.");
    }

    if (WiFi.status() == WL_CONNECTED)
        WiFi.disconnect(false);

    logger.info("Connecting to Wi-Fi network: " + ssid);
    WiFi.begin(ssid.c_str(), password.c_str());

    return true;
}

/**
 * @brief Disconnects the Station interface from Wi-Fi.
 *
 * Cancels any active Station connection or connection attempt while
 * keeping the Wi-Fi subsystem enabled for future scans or connections.
 */
void WiFiManager::disconnect()
{
    String ssid = WiFi.SSID();

    WiFi.disconnect(false);

    if (ssid.length() > 0)
        logger.info("Disconnected from Wi-Fi network: " + ssid);
    else
        logger.info("Wi-Fi Station disconnected.");
}

/**
 * @brief Returns whether the Station interface is connected.
 *
 * @return true when connected to a Wi-Fi network.
 */
bool WiFiManager::isConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}

/**
 * @brief Returns the SSID of the currently connected Wi-Fi network.
 *
 * @return Connected network SSID.
 */
String WiFiManager::getConnectedSSID() const
{
    if (!isConnected())
        return "";

    return WiFi.SSID();
}

/**
 * @brief Returns the Station interface IP address.
 *
 * @return Local IP address assigned to the Station interface.
 */
IPAddress WiFiManager::getLocalIP() const
{
    return WiFi.localIP();
}

/**
 * @brief Returns the signal strength of the active Station connection.
 *
 * Retrieves the RSSI reported by the ESP32 Wi-Fi subsystem for the
 * currently connected Access Point.
 *
 * @return Current Wi-Fi signal strength in dBm.
 */
int32_t WiFiManager::getRSSI() const
{
    return WiFi.RSSI();
}

/**
 * @brief Stops the WiFiManager and releases Wi-Fi resources.
 *
 * Stops any active Access Point, clears stored scan results and disables
 * the ESP32 Wi-Fi subsystem so its runtime resources become available to
 * other features.
 *
 * Wi-Fi can be activated again later by starting an Access Point or
 * performing a network scan.
 *
 * @return true when Wi-Fi has been shut down.
 */
bool WiFiManager::stop()
{
    releasePacketCapture();

    if (isAPRunning())
        WiFi.softAPdisconnect(true);

    WiFi.scanDelete();

    delete[] networks;
    networks = nullptr;
    networkCount = 0;
    
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    logger.info("WiFiManager stopped. Wi-Fi subsystem released.");
    return true;
}

/**
 * @brief Starts passive Wi-Fi packet capture.
 *
 * Allocates the packet ring buffer when required and enables promiscuous
 * receive mode while preserving the active Station connection.
 *
 * @return true when packet capture started successfully.
 * @return false when Wi-Fi is not connected, memory allocation fails or
 * capture could not be enabled.
 */
bool WiFiManager::startPacketCapture()
{
    if (!isConnected())
    {
        logger.error("Packet capture requires an active Wi-Fi connection.");
        return false;
    }

    if (packetCaptureRunning)
        return true;

    if (capturedPackets == nullptr)
    {
        capturedPackets = new (std::nothrow) WiFiPacketInfo[MAX_CAPTURED_PACKETS];

        if (capturedPackets == nullptr)
        {
            logger.error("Failed to allocate packet capture buffer.");
            return false;
        }

        packetWriteIndex = 0;
        capturedPacketCount = 0;

        logger.info("Packet capture buffer allocated.");
    }

    wifi_promiscuous_filter_t filter;
    filter.filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT | WIFI_PROMIS_FILTER_MASK_DATA | WIFI_PROMIS_FILTER_MASK_CTRL;

    if (esp_wifi_set_promiscuous_filter(&filter) != ESP_OK)
    {
        logger.error("Failed to configure Wi-Fi packet capture filter.");
        return false;
    }

    packetCaptureInstance = this;

    if (esp_wifi_set_promiscuous_rx_cb(promiscuousCallback) != ESP_OK)
    {
        packetCaptureInstance = nullptr;
        logger.error("Failed to register Wi-Fi packet capture callback.");
        return false;
    }

    if (esp_wifi_set_promiscuous(true) != ESP_OK)
    {
        packetCaptureInstance = nullptr;
        logger.error("Failed to enable Wi-Fi packet capture.");
        return false;
    }

    packetCaptureRunning = true;

    logger.info("Wi-Fi packet capture started.");
    return true;
}

/**
 * @brief Stops passive Wi-Fi packet capture.
 *
 * Disables promiscuous receive mode without changing the active Station
 * connection.
 */
void WiFiManager::stopPacketCapture()
{
    if (!packetCaptureRunning)
        return;

    esp_wifi_set_promiscuous(false);

    packetCaptureRunning = false;
    packetCaptureInstance = nullptr;

    logger.info("Wi-Fi packet capture stopped.");
}

/**
 * @brief Returns whether passive packet capture is active.
 *
 * @return true while packet capture is enabled.
 */
bool WiFiManager::isPacketCaptureRunning() const
{
    return packetCaptureRunning;
}

/**
 * @brief Clears all stored packet metadata.
 */
void WiFiManager::clearCapturedPackets()
{
    portENTER_CRITICAL(&packetMux);

    packetWriteIndex = 0;
    capturedPacketCount = 0;

    portEXIT_CRITICAL(&packetMux);
}

/**
 * @brief Returns the number of packet records currently stored.
 *
 * @return Number of valid packet records.
 */
uint8_t WiFiManager::getCapturedPacketCount() const
{
    portENTER_CRITICAL(&packetMux);

    uint8_t count = capturedPacketCount;

    portEXIT_CRITICAL(&packetMux);

    return count;
}

/**
 * @brief Returns a captured packet.
 *
 * Packets are indexed newest first.
 *
 * @param index Packet index, newest first.
 * @return Copy of the requested packet metadata.
 */
WiFiPacketInfo WiFiManager::getCapturedPacket(uint8_t index) const
{
    WiFiPacketInfo result;

    portENTER_CRITICAL(&packetMux);

    if (capturedPackets != nullptr && index < capturedPacketCount)
    {
        int packetIndex = static_cast<int>(packetWriteIndex) - 1 - index;

        while (packetIndex < 0)
            packetIndex += MAX_CAPTURED_PACKETS;

        result = capturedPackets[packetIndex];
    }

    portEXIT_CRITICAL(&packetMux);

    return result;
}

/**
 * @brief ESP32 promiscuous Wi-Fi receive callback.
 *
 * Delegates received frames to the active WiFiManager instance.
 *
 * @param buffer Packet data supplied by the ESP32 Wi-Fi driver.
 * @param type Promiscuous packet type.
 */
void WiFiManager::promiscuousCallback(void* buffer, wifi_promiscuous_pkt_type_t type)
{
    if (packetCaptureInstance == nullptr || buffer == nullptr || type == WIFI_PKT_MISC)
        return;

    packetCaptureInstance->capturePacket(static_cast<wifi_promiscuous_pkt_t*>(buffer), type);
}

/**
 * @brief Extracts compact metadata from a captured Wi-Fi frame.
 *
 * Only visible 802.11 information is inspected for protected frames.
 * Plaintext data frames are additionally inspected for LLC/SNAP and
 * recognizable network protocols.
 *
 * @param packet Received promiscuous packet.
 * @param type Promiscuous packet type.
 */
void WiFiManager::capturePacket(const wifi_promiscuous_pkt_t* packet, wifi_promiscuous_pkt_type_t type)
{
    if (packet == nullptr || packet->rx_ctrl.sig_len < 2)
        return;

    const uint8_t* frame = packet->payload;
    uint16_t length = packet->rx_ctrl.sig_len;

    WiFiPacketInfo info;

    info.id = ++capturedPacketSequence;
    info.rssi = packet->rx_ctrl.rssi;
    info.channel = packet->rx_ctrl.channel;
    info.length = length;

    uint8_t frameControl1 = frame[0];
    uint8_t frameControl2 = frame[1];

    info.frameType = (frameControl1 >> 2) & 0x03;
    info.frameSubtype = (frameControl1 >> 4) & 0x0F;
    info.protectedFrame = (frameControl2 & 0x40) != 0;

    if (length >= 10)
        memcpy(info.receiver, frame + 4, 6);

    if (length >= 16)
        memcpy(info.transmitter, frame + 10, 6);

    if (length >= 22)
        memcpy(info.address3, frame + 16, 6);

    if (info.protectedFrame)
    {
        info.protocol = WiFiPacketProtocol::Protected;
        storeCapturedPacket(info);
        return;
    }

    /*
     * Protocol parsing only applies to plaintext 802.11 data frames.
     */
    if (type != WIFI_PKT_DATA || length < 32)
    {
        storeCapturedPacket(info);
        return;
    }

    bool toDS = (frameControl2 & 0x01) != 0;
    bool fromDS = (frameControl2 & 0x02) != 0;
    bool qosData = (info.frameSubtype & 0x08) != 0;

    uint16_t payloadOffset = 24;

    if (toDS && fromDS)
        payloadOffset += 6;

    if (qosData)
        payloadOffset += 2;

    if (length < payloadOffset + 8)
    {
        storeCapturedPacket(info);
        return;
    }

    const uint8_t* payload = frame + payloadOffset;

    /*
     * IEEE 802.2 LLC/SNAP header.
     */
    if (payload[0] != 0xAA || payload[1] != 0xAA || payload[2] != 0x03)
    {
        storeCapturedPacket(info);
        return;
    }

    uint16_t etherType = (static_cast<uint16_t>(payload[6]) << 8) | payload[7];
    uint16_t networkOffset = payloadOffset + 8;

    if (etherType == 0x0806)
    {
        info.protocol = WiFiPacketProtocol::ARP;

        if (length >= networkOffset + 28)
        {
            const uint8_t* arp = frame + networkOffset;

            info.sourceIP =
                (static_cast<uint32_t>(arp[14]) << 24) |
                (static_cast<uint32_t>(arp[15]) << 16) |
                (static_cast<uint32_t>(arp[16]) << 8) |
                arp[17];

            info.destinationIP =
                (static_cast<uint32_t>(arp[24]) << 24) |
                (static_cast<uint32_t>(arp[25]) << 16) |
                (static_cast<uint32_t>(arp[26]) << 8) |
                arp[27];
        }

        storeCapturedPacket(info);
        return;
    }

    if (etherType == 0x86DD)
    {
        info.protocol = WiFiPacketProtocol::IPv6;
        storeCapturedPacket(info);
        return;
    }

    if (etherType != 0x0800 || length < networkOffset + 20)
    {
        storeCapturedPacket(info);
        return;
    }

    info.protocol = WiFiPacketProtocol::IPv4;

    const uint8_t* ipv4 = frame + networkOffset;
    uint8_t headerLength = (ipv4[0] & 0x0F) * 4;

    if (headerLength < 20 || length < networkOffset + headerLength)
    {
        storeCapturedPacket(info);
        return;
    }

    info.sourceIP =
        (static_cast<uint32_t>(ipv4[12]) << 24) |
        (static_cast<uint32_t>(ipv4[13]) << 16) |
        (static_cast<uint32_t>(ipv4[14]) << 8) |
        ipv4[15];

    info.destinationIP =
        (static_cast<uint32_t>(ipv4[16]) << 24) |
        (static_cast<uint32_t>(ipv4[17]) << 16) |
        (static_cast<uint32_t>(ipv4[18]) << 8) |
        ipv4[19];

    uint8_t ipProtocol = ipv4[9];

    if (ipProtocol == 1)
    {
        info.protocol = WiFiPacketProtocol::ICMP;
        storeCapturedPacket(info);
        return;
    }

    if (ipProtocol != 6 && ipProtocol != 17)
    {
        storeCapturedPacket(info);
        return;
    }

    uint16_t transportOffset = networkOffset + headerLength;

    if (length < transportOffset + 4)
    {
        storeCapturedPacket(info);
        return;
    }

    const uint8_t* transport = frame + transportOffset;

    info.sourcePort = (static_cast<uint16_t>(transport[0]) << 8) | transport[1];
    info.destinationPort = (static_cast<uint16_t>(transport[2]) << 8) | transport[3];

    if (ipProtocol == 17)
    {
        info.protocol = WiFiPacketProtocol::UDP;

        if (info.sourcePort == 67 || info.sourcePort == 68 || info.destinationPort == 67 || info.destinationPort == 68)
            info.protocol = WiFiPacketProtocol::DHCP;
        else if (info.sourcePort == 5353 || info.destinationPort == 5353)
            info.protocol = WiFiPacketProtocol::MDNS;
        else if (info.sourcePort == 53 || info.destinationPort == 53)
            info.protocol = WiFiPacketProtocol::DNS;
    }
    else
    {
        info.protocol = WiFiPacketProtocol::TCP;

        if (info.sourcePort == 80 || info.destinationPort == 80)
            info.protocol = WiFiPacketProtocol::HTTP;
        else if (info.sourcePort == 443 || info.destinationPort == 443)
            info.protocol = WiFiPacketProtocol::HTTPS;
    }

    storeCapturedPacket(info);
}

/**
 * @brief Stores packet metadata in the packet capture ring buffer.
 *
 * Applies the active capture filter before storing the packet. Packets that
 * do not match the selected filter are discarded and never consume a slot
 * in the ring buffer.
 *
 * When the ring buffer is full, new packets automatically replace the
 * oldest retained packet.
 *
 * @param packet Packet metadata to evaluate and store.
 */
void WiFiManager::storeCapturedPacket(const WiFiPacketInfo& packet)
{
    if (capturedPackets == nullptr || !matchesPacketCaptureFilter(packet))
        return;

    portENTER_CRITICAL(&packetMux);

    capturedPackets[packetWriteIndex] = packet;
    packetWriteIndex = (packetWriteIndex + 1) % MAX_CAPTURED_PACKETS;

    if (capturedPacketCount < MAX_CAPTURED_PACKETS)
        capturedPacketCount++;

    portEXIT_CRITICAL(&packetMux);
}

/**
 * @brief Releases all packet capture memory.
 *
 * Stops active promiscuous capture, clears the packet ring buffer and
 * releases its dynamically allocated memory.
 *
 * The active Wi-Fi Station connection remains unchanged.
 */
void WiFiManager::releasePacketCapture()
{
    stopPacketCapture();

    WiFiPacketInfo* buffer = nullptr;

    portENTER_CRITICAL(&packetMux);

    buffer = capturedPackets;
    capturedPackets = nullptr;

    packetWriteIndex = 0;
    capturedPacketCount = 0;
    capturedPacketSequence = 0;

    portEXIT_CRITICAL(&packetMux);

    delete[] buffer;

    logger.info("Packet capture buffer released.");
}

/**
 * @brief Sets the active packet capture filter.
 *
 * @param filter Packet capture filter to activate.
 */
void WiFiManager::setPacketCaptureFilter(WiFiPacketCaptureFilter filter)
{
    packetCaptureFilter = filter;
}

/**
 * @brief Returns the active packet capture filter.
 *
 * @return Currently active packet capture filter.
 */
WiFiPacketCaptureFilter WiFiManager::getPacketCaptureFilter() const
{
    return packetCaptureFilter;
}

/**
 * @brief Returns whether a captured packet matches the active filter.
 *
 * Interesting suppresses management and control frame noise while retaining
 * all 802.11 data frames and recognized higher-level protocols.
 *
 * @param packet Parsed packet metadata.
 * @return true when the packet should be retained.
 * @return false when the packet should be discarded.
 */
bool WiFiManager::matchesPacketCaptureFilter(const WiFiPacketInfo& packet) const
{
    switch (packetCaptureFilter)
    {
        case WiFiPacketCaptureFilter::Interesting:
            return packet.frameType == 2 ||
                   packet.protocol == WiFiPacketProtocol::ARP ||
                   packet.protocol == WiFiPacketProtocol::IPv4 ||
                   packet.protocol == WiFiPacketProtocol::IPv6 ||
                   packet.protocol == WiFiPacketProtocol::ICMP ||
                   packet.protocol == WiFiPacketProtocol::TCP ||
                   packet.protocol == WiFiPacketProtocol::UDP ||
                   packet.protocol == WiFiPacketProtocol::DNS ||
                   packet.protocol == WiFiPacketProtocol::MDNS ||
                   packet.protocol == WiFiPacketProtocol::DHCP ||
                   packet.protocol == WiFiPacketProtocol::HTTP ||
                   packet.protocol == WiFiPacketProtocol::HTTPS ||
                   packet.protocol == WiFiPacketProtocol::Protected;

        case WiFiPacketCaptureFilter::Data:
            return packet.frameType == 2;

        case WiFiPacketCaptureFilter::Management:
            return packet.frameType == 0;

        case WiFiPacketCaptureFilter::All:
            return true;
    }

    return true;
}