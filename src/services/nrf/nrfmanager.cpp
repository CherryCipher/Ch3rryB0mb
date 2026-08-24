/**
 * @file nrfmanager.cpp
 * @brief Implementation of the NRF24 radio manager.
 */

#include "nrfmanager.h"

/**
 * @brief Constructs a new NRFManager.
 *
 * Stores references to the application's Logger and SPIManager
 * and configures the NRF24 CE and CSN pins.
 *
 * @param logger Reference to the application's Logger.
 * @param spiManager Reference to the application's SPIManager.
 */
NRFManager::NRFManager(Logger& logger, SPIManager& spiManager)
    : logger(logger), spiManager(spiManager), radio(NRF_CE, NRF_CSN)
{
}

/**
 * @brief Initializes the NRF24 radio.
 *
 * Verifies that the shared SPI infrastructure is available,
 * initializes the NRF24 using the hardware SPI bus and places
 * the radio in an inactive state.
 *
 * @return true if the NRF24 radio was initialized successfully.
 * @return false otherwise.
 */
bool NRFManager::start()
{
    if (running) return true;

    logger.info("Starting NRFManager.");

    if (!spiManager.isRunning()) {
        logger.error("SPIManager is not running.");
        return false;
    }

    if (!radio.begin(&spiManager.getHardwareBus())) {
        logger.error("NRF24 radio not detected.");
        return false;
    }

    radio.stopConstCarrier();
    radio.stopListening();
    radio.setAutoAck(false);
    radio.disableCRC();
    radio.powerDown();

    running = true;

    logger.info("NRF24 radio detected.");
    logger.info("NRFManager started.");

    return true;
}

/**
 * @brief Stops the NRFManager.
 *
 * Stops listening, powers down the NRF24 radio and marks the
 * manager as inactive.
 *
 * @return true when the NRFManager is stopped.
 */
bool NRFManager::stop()
{
    if (!running) return true;

    radio.stopListening();
    radio.powerDown();

    running = false;

    logger.info("NRFManager stopped.");

    return true;
}

/**
 * @brief Returns whether the NRFManager is currently running.
 *
 * @return true if the NRFManager is running.
 * @return false otherwise.
 */
bool NRFManager::isRunning() const
{
    return running;
}

/**
 * @brief Configures the NRF24 for packet transmission.
 *
 * @param channel NRF24 channel between 0 and 125.
 * @param address Pointer to the five-byte destination address.
 *
 * @return true if the transmitter was configured successfully.
 * @return false otherwise.
 */
bool NRFManager::configureTransmitter(uint8_t channel, const uint8_t* address)
{
    if (!running) {
        logger.error("NRFManager is not running.");
        return false;
    }

    if (channel >= NRF_CHANNEL_COUNT) {
        logger.error("Invalid NRF channel.");
        return false;
    }

    if (address == nullptr) {
        logger.error("Invalid NRF destination address.");
        return false;
    }

    radio.stopListening();
    radio.powerUp();

    radio.setChannel(channel);
    radio.setAutoAck(false);
    radio.disableCRC();
    radio.setDataRate(RF24_1MBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.openWritingPipe(address);

    logger.info(String("NRF transmitter configured on channel ") + channel + ".");

    return true;
}

/**
 * @brief Transmits a packet using the current NRF24 configuration.
 *
 * @param data Pointer to the payload data.
 * @param length Number of payload bytes to transmit.
 *
 * @return true if the packet was transmitted successfully.
 * @return false otherwise.
 */
bool NRFManager::send(const void* data, uint8_t length)
{
    if (!running) {
        logger.error("NRFManager is not running.");
        return false;
    }

    if (data == nullptr) {
        logger.error("NRF payload is null.");
        return false;
    }

    if (length == 0 || length > MAX_PAYLOAD_SIZE) {
        logger.error("Invalid NRF payload length.");
        return false;
    }

    if (!radio.write(data, length)) {
        logger.warning("NRF packet transmission failed.");
        return false;
    }

    return true;
}

/**
 * @brief Scans the complete NRF24 frequency range.
 *
 * @param results Array receiving 126 activity values from 0 to 100.
 * @param samplesPerChannel Number of measurements performed per channel.
 *
 * @return true if the scan completed successfully.
 * @return false otherwise.
 */
bool NRFManager::scanSpectrum(uint8_t results[NRF_CHANNEL_COUNT], uint16_t samplesPerChannel)
{
    if (!running) {
        logger.error("NRFManager is not running.");
        return false;
    }

    if (samplesPerChannel == 0) {
        logger.error("Spectrum scan requires at least one sample.");
        return false;
    }

    logger.info("Starting full NRF spectrum scan.");

    radio.powerUp();

    for (uint8_t channel = 0; channel < NRF_CHANNEL_COUNT; channel++)
        results[channel] = measureChannel(channel, samplesPerChannel);

    radio.powerDown();

    logger.info("Full NRF spectrum scan completed.");

    return true;
}

/**
 * @brief Measures RF activity on one NRF24 channel.
 *
 * @param channel NRF24 channel between 0 and 125.
 * @param activity Reference receiving an activity value from 0 to 100.
 * @param samples Number of measurements performed on the channel.
 *
 * @return true if the measurement completed successfully.
 * @return false otherwise.
 */
bool NRFManager::scanNrfChannel(uint8_t channel, uint8_t& activity, uint16_t samples)
{
    if (!running) {
        logger.error("NRFManager is not running.");
        return false;
    }

    if (channel >= NRF_CHANNEL_COUNT) {
        logger.error("Invalid NRF channel.");
        return false;
    }

    if (samples == 0) {
        logger.error("NRF channel scan requires at least one sample.");
        return false;
    }

    radio.powerUp();

    activity = measureChannel(channel, samples);

    radio.powerDown();

    return true;
}

/**
 * @brief Scans the frequency range occupied by a Wi-Fi channel.
 *
 * @param wifiChannel Wi-Fi channel between 1 and 13.
 * @param results Array receiving 21 activity values from 0 to 100.
 * @param startNrfChannel Reference receiving the first NRF24 channel.
 * @param samplesPerChannel Number of measurements performed per NRF channel.
 *
 * @return true if the scan completed successfully.
 * @return false otherwise.
 */
bool NRFManager::scanWifiChannel(uint8_t wifiChannel, uint8_t results[WIFI_SCAN_WIDTH], uint8_t& startNrfChannel, uint16_t samplesPerChannel)
{
    if (!running) {
        logger.error("NRFManager is not running.");
        return false;
    }

    if (wifiChannel < 1 || wifiChannel > WIFI_CHANNEL_COUNT) {
        logger.error("Invalid Wi-Fi channel.");
        return false;
    }

    if (samplesPerChannel == 0) {
        logger.error("Wi-Fi channel scan requires at least one sample.");
        return false;
    }

    // Wi-Fi channel 1 is centered at 2412 MHz.
    // Each following channel moves 5 MHz higher.
    uint8_t centerNrfChannel = 12 + ((wifiChannel - 1) * 5);

    // Scan 10 MHz below through 10 MHz above the center frequency.
    startNrfChannel = centerNrfChannel - (WIFI_SCAN_WIDTH / 2);

    logger.info("Starting Wi-Fi channel scan.");

    radio.powerUp();

    for (uint8_t index = 0; index < WIFI_SCAN_WIDTH; index++) {
        uint8_t nrfChannel = startNrfChannel + index;
        results[index] = measureChannel(nrfChannel, samplesPerChannel);
    }

    radio.powerDown();

    logger.info("Wi-Fi channel scan completed.");

    return true;
}

/**
 * @brief Measures RF activity on a single NRF24 channel.
 *
 * @param channel NRF24 channel between 0 and 125.
 * @param samples Number of measurements to perform.
 *
 * @return Activity percentage between 0 and 100.
 */
uint8_t NRFManager::measureChannel(uint8_t channel, uint16_t samples)
{
    uint16_t hits = 0;

    radio.setChannel(channel);

    for (uint16_t sample = 0; sample < samples; sample++) {
        radio.startListening();
        delayMicroseconds(SAMPLE_TIME_US);

        if (radio.testRPD()) hits++;

        radio.stopListening();
    }

    return static_cast<uint8_t>((static_cast<uint32_t>(hits) * 100) / samples);
}