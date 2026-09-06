/**
 * @file nrfsend.h
 * @brief Declaration of the NRF Send feature.
 *
 * Provides application-level NRF24 message transmission for Ch3rryB0mb.
 */

#pragma once

#include <Arduino.h>
#include "services/nrf/nrfmanager.h"

class Services;

/**
 * @class NRFSend
 * @brief Controls reliable NRF24 message transmission.
 *
 * NRFSend provides the application logic for sending short text messages
 * over NRF24. It configures the shared NRFManager for acknowledged
 * transmission using the shared Ch3rryN0de address.
 */
class NRFSend
{
public:
    /**
     * @enum SendResult
     * @brief Possible results of an NRF24 message transmission.
     */
    enum class SendResult : uint8_t
    {
        Success = 0,
        NRFUnavailable,
        InvalidChannel,
        EmptyMessage,
        MessageTooLong,
        ConfigurationFailed,
        NoAcknowledgement
    };

    /**
     * @brief Maximum text length in bytes.
     *
     * One byte of the 32-byte NRF24 payload is reserved for the
     * null terminator.
     */
    static constexpr uint8_t MAX_MESSAGE_LENGTH = NRFManager::MAX_PAYLOAD_SIZE - 1;

    /**
     * @brief Default NRF24 communication channel.
     */
    static constexpr uint8_t DEFAULT_CHANNEL = 30;

    /**
     * @brief Constructs the NRF Send feature.
     *
     * @param services Reference to the shared application services.
     */
    explicit NRFSend(Services& services);

    /**
     * @brief Sets the NRF24 transmission channel.
     *
     * @param channel NRF24 channel between 0 and 125.
     *
     * @return true if the channel was accepted.
     * @return false otherwise.
     */
    bool setChannel(uint8_t channel);

    /**
     * @brief Returns the selected NRF24 transmission channel.
     *
     * @return Selected NRF24 channel.
     */
    uint8_t getChannel() const;

    /**
     * @brief Sends a text message using acknowledged NRF24 communication.
     *
     * Configures the NRF24 transmitter using the selected channel and
     * shared Ch3rryN0de address. The message is transmitted as a fixed
     * 32-byte null-terminated payload.
     *
     * @param message Text message to transmit.
     *
     * @return Result of the transmission attempt.
     */
    SendResult sendMessage(const String& message);

    /**
     * @brief Returns whether the NRF24 radio is available.
     *
     * @return true if NRFManager is running.
     * @return false otherwise.
     */
    bool isAvailable() const;

private:
    /**
     * @brief Reference to the shared application services.
     */
    Services& services;

    /**
     * @brief Selected NRF24 transmission channel.
     */
    uint8_t channel = DEFAULT_CHANNEL;
};