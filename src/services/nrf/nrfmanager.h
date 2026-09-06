/**
 * @file nrfmanager.h
 * @brief Declaration of the NRF24 radio manager.
 *
 * Provides shared NRF24 functionality including RF activity scanning,
 * packet transmission and packet reception.
 */

#pragma once

#include <Arduino.h>
#include <RF24.h>
#include "../logger/logger.h"
#include "../spi/spimanager.h"

/**
 * @class NRFManager
 * @brief Manages NRF24 radio functionality.
 *
 * NRFManager is responsible for initializing and controlling the NRF24
 * radio used by Ch3rryB0mb and Ch3rryN0de.
 *
 * It provides RF activity scanning for the complete NRF24 spectrum,
 * individual NRF24 channels and individual 2.4 GHz Wi-Fi bands.
 *
 * The manager also provides generic NRF24 packet transmission and
 * reception so higher level features can use the radio without
 * accessing the underlying RF24 instance directly.
 *
 * RF activity is measured using the NRF24 Received Power Detector (RPD).
 * Scan results therefore represent activity percentages rather than
 * absolute signal strength values such as RSSI or dBm.
 */
class NRFManager
{
public:
    /**
     * @brief Number of RF channels supported by the NRF24.
     *
     * NRF24 channels range from 0 through 125.
     */
    static constexpr uint8_t NRF_CHANNEL_COUNT = 126;

    /**
     * @brief Number of supported 2.4 GHz Wi-Fi channels.
     *
     * Ch3rryB0mb currently supports Wi-Fi channels 1 through 13.
     */
    static constexpr uint8_t WIFI_CHANNEL_COUNT = 13;

    /**
     * @brief Number of NRF24 channels sampled around a Wi-Fi channel.
     *
     * A Wi-Fi channel scan covers 21 NRF24 channels, corresponding
     * to approximately 20 MHz of spectrum around the channel center.
     */
    static constexpr uint8_t WIFI_SCAN_WIDTH = 21;

    /**
     * @brief Maximum NRF24 payload size in bytes.
     */
    static constexpr uint8_t MAX_PAYLOAD_SIZE = 32;

    /**
     * @brief Constructs a new NRFManager.
     *
     * @param logger Reference to the application's Logger.
     * @param spiManager Reference to the application's SPIManager.
     */
    NRFManager(Logger& logger, SPIManager& spiManager);

    /**
     * @brief Initializes the NRF24 radio.
     *
     * Verifies that the shared SPI infrastructure is available,
     * initializes the NRF24 radio and places it in an inactive state.
     *
     * @return true if the NRF24 radio was initialized successfully.
     * @return false otherwise.
     */
    bool start();

    /**
     * @brief Stops the NRFManager.
     *
     * Stops listening, powers down the NRF24 radio and marks the
     * manager as inactive.
     *
     * @return true when the NRFManager is stopped.
     */
    bool stop();

    /**
     * @brief Returns whether the NRFManager is currently running.
     *
     * @return true if the NRFManager is running.
     * @return false otherwise.
     */
    bool isRunning() const;

/**
 * @brief Configures the NRF24 for packet transmission.
 *
 * Stops listening and configures the radio for transmission using
 * the selected NRF24 channel and destination address.
 *
 * When acknowledged is enabled, 16-bit CRC, automatic acknowledgements
 * and retransmissions are enabled. When disabled, packets are transmitted
 * without acknowledgements for beacon-style communication.
 *
 * The destination address must contain five address bytes.
 *
 * @param channel NRF24 channel between 0 and 125.
 * @param address Pointer to the five-byte destination address.
 * @param acknowledged Enables reliable acknowledged transmission.
 *
 * @return true if the transmitter was configured successfully.
 * @return false if the manager is not running or the configuration is invalid.
 */
bool configureTransmitter(uint8_t channel, const uint8_t* address, bool acknowledged = false);

    /**
     * @brief Configures the NRF24 for acknowledged packet reception.
     *
     * Configures the radio for 1 Mbps communication using a static
     * 32-byte payload, 16-bit CRC and automatic acknowledgements.
     * Reading pipe 1 is opened using the supplied address and the
     * radio immediately enters listening mode.
     *
     * The address must contain five address bytes.
     *
     * @param channel NRF24 channel between 0 and 125.
     * @param address Pointer to the five-byte receiver address.
     *
     * @return true if the receiver was configured successfully.
     * @return false if the manager is not running or the configuration is invalid.
     */
    bool configureReceiver(uint8_t channel, const uint8_t* address);

    /**
     * @brief Returns whether an NRF24 packet is waiting in the receive FIFO.
     *
     * This function is non-blocking and can safely be polled from the
     * application's update loop.
     *
     * @return true when a packet is available.
     * @return false when no packet is available or the manager is not running.
     */
    bool available();

    /**
     * @brief Reads an available NRF24 packet.
     *
     * @param data Pointer to the destination buffer.
     * @param length Maximum number of bytes to read.
     *
     * @return true when a packet was read.
     * @return false if no packet is available or the arguments are invalid.
     */
    bool receive(void* data, uint8_t length);

    /**
     * @brief Transmits a packet using the current NRF24 configuration.
     *
     * Payloads may contain between 1 and 32 bytes.
     *
     * @param data Pointer to the payload data.
     * @param length Number of payload bytes to transmit.
     *
     * @return true if the packet was transmitted successfully.
     * @return false if the manager is not running, the payload is invalid
     *         or transmission failed.
     */
    bool send(const void* data, uint8_t length);

    /**
     * @brief Scans the complete NRF24 frequency range.
     *
     * Scans channels 0 through 125 and stores an activity percentage
     * for every channel.
     *
     * @param results Array receiving 126 activity values from 0 to 100.
     * @param samplesPerChannel Number of measurements performed per channel.
     *
     * @return true if the scan completed successfully.
     * @return false if the manager is not running or samplesPerChannel is zero.
     */
    bool scanSpectrum(uint8_t results[NRF_CHANNEL_COUNT], uint16_t samplesPerChannel = 20);

    /**
     * @brief Measures RF activity on one NRF24 channel.
     *
     * Repeatedly samples the selected channel and calculates the
     * percentage of samples in which RF activity was detected.
     *
     * @param channel NRF24 channel between 0 and 125.
     * @param activity Reference receiving an activity value from 0 to 100.
     * @param samples Number of measurements performed on the channel.
     *
     * @return true if the measurement completed successfully.
     * @return false if the manager is not running, the channel is invalid
     *         or samples is zero.
     */
    bool scanNrfChannel(uint8_t channel, uint8_t& activity, uint16_t samples = 100);

    /**
     * @brief Scans the frequency range occupied by a Wi-Fi channel.
     *
     * Converts the selected 2.4 GHz Wi-Fi channel to its corresponding
     * NRF24 frequency range and measures activity across 21 NRF24 channels.
     *
     * @param wifiChannel Wi-Fi channel between 1 and 13.
     * @param results Array receiving 21 activity values from 0 to 100.
     * @param startNrfChannel Reference receiving the first NRF24 channel
     *        represented by the results array.
     * @param samplesPerChannel Number of measurements performed per NRF channel.
     *
     * @return true if the scan completed successfully.
     * @return false if the manager is not running, the Wi-Fi channel is
     *         invalid or samplesPerChannel is zero.
     */
    bool scanWifiChannel(uint8_t wifiChannel, uint8_t results[WIFI_SCAN_WIDTH], uint8_t& startNrfChannel, uint16_t samplesPerChannel = 20);

private:
    /**
     * @brief Time spent listening during one RF activity sample.
     *
     * The RF24 scanner example uses approximately 128 microseconds.
     */
    static constexpr uint16_t SAMPLE_TIME_US = 128;

    /**
     * @brief Reference to the application's Logger instance.
     */
    Logger& logger;

    /**
     * @brief Reference to the application's SPIManager.
     */
    SPIManager& spiManager;

    /**
     * @brief NRF24 radio instance.
     */
    RF24 radio;

    /**
     * @brief Indicates whether the NRFManager is currently running.
     */
    bool running = false;

    /**
     * @brief Measures RF activity on a single NRF24 channel.
     *
     * Performs repeated RPD measurements and converts the number
     * of detected samples to a percentage between 0 and 100.
     *
     * @param channel NRF24 channel between 0 and 125.
     * @param samples Number of measurements to perform.
     *
     * @return Activity percentage between 0 and 100.
     */
    uint8_t measureChannel(uint8_t channel, uint16_t samples);
};