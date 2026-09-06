/**
 * @file nrfsend.cpp
 * @brief Implementation of the NRF Send feature.
 */

#include "nrfsend.h"

#include <cstring>
#include "services/services.h"
#include "node/nodeprotocol.h"

/**
 * @brief Constructs the NRF Send feature.
 *
 * @param services Reference to the shared application services.
 */
NRFSend::NRFSend(Services& services) : services(services)
{
}

/**
 * @brief Sets the NRF24 transmission channel.
 *
 * @param channel NRF24 channel between 0 and 125.
 *
 * @return true if the channel was accepted.
 * @return false otherwise.
 */
bool NRFSend::setChannel(uint8_t channel)
{
    if (channel >= NRFManager::NRF_CHANNEL_COUNT) return false;

    this->channel = channel;
    return true;
}

/**
 * @brief Returns the selected NRF24 transmission channel.
 *
 * @return Selected NRF24 channel.
 */
uint8_t NRFSend::getChannel() const
{
    return channel;
}

/**
 * @brief Sends a text message using acknowledged NRF24 communication.
 *
 * Starts the NRF24 service when required, configures the transmitter
 * using the selected channel and shared Ch3rryN0de address, then sends
 * the message as a fixed 32-byte payload.
 *
 * @param message Text message to transmit.
 *
 * @return Result of the transmission attempt.
 */
NRFSend::SendResult NRFSend::sendMessage(const String& message)
{
    if (!services.nrf.isRunning() && !services.nrf.start()) {
        services.logger.error("NRF Send: Failed to start NRFManager.");
        return SendResult::NRFUnavailable;
    }
    if (channel >= NRFManager::NRF_CHANNEL_COUNT) {
        services.logger.error("NRF Send: Invalid channel.");
        return SendResult::InvalidChannel;
    }
    if (message.length() == 0) {
        services.logger.warning("NRF Send: Message is empty.");
        return SendResult::EmptyMessage;
    }
    if (message.length() > MAX_MESSAGE_LENGTH) {
        services.logger.warning("NRF Send: Message exceeds 31 bytes.");
        return SendResult::MessageTooLong;
    }

    if (!services.nrf.configureTransmitter(channel, NodeProtocol::NRF_ADDRESS, true)) {
        services.logger.error("NRF Send: Failed to configure transmitter.");
        return SendResult::ConfigurationFailed;
    }

    char payload[NRFManager::MAX_PAYLOAD_SIZE] = {};
    memcpy(payload, message.c_str(), message.length());

    services.logger.info(String("NRF Send: Sending \"") + message + "\" on channel " + channel + ".");

    if (!services.nrf.send(payload, sizeof(payload))) {
        services.logger.warning("NRF Send: No acknowledgement received.");
        return SendResult::NoAcknowledgement;
    }

    services.logger.info("NRF Send: Message sent and acknowledged.");

    return SendResult::Success;
}

/**
 * @brief Returns whether the NRF24 radio is available.
 *
 * @return true if NRFManager is running.
 * @return false otherwise.
 */
bool NRFSend::isAvailable() const
{
    return services.nrf.isRunning();
}