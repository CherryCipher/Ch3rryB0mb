/**
 * @file modulepins.h
 * @brief Board-specific bus and GPIO assignments for external modules.
 *
 * This file provides the central hardware pin configuration used by
 * external modules connected to Ch3rryB0mb and Ch3rryN0de.
 *
 * Both the hardware SPI bus and the physical GPIO assignments are
 * selected at compile time based on the target board.
 *
 * Hardware managers should never define their own physical GPIO
 * numbers or SPI bus identifiers. Instead, they should use the
 * constants exposed through the ModulePins namespace.
 *
 * @par Board selection
 * The ESP32-S3 Ch3rryN0de configuration is selected by defining:
 *
 * @code
 * -D C3N0_S3
 * @endcode
 *
 * Existing ESP32-based Ch3rryB0mb and Ch3rryN0de builds use the
 * default ESP32 configuration.
 *
 * @par Adding another board
 * To support another hardware target:
 *
 * 1. Add a unique build flag to its PlatformIO environment.
 * 2. Add a matching #elif section in this file.
 * 3. Define SPI_BUS and every module pin required by the shared managers.
 *
 * Managers such as SPIManager, NRFManager, CC1101Manager and
 * NodeDisplayManager should not require board-specific changes.
 */

#pragma once

#include <Arduino.h>
#include <SPI.h>

/**
 * @namespace ModulePins
 * @brief Board-specific hardware connections for external modules.
 *
 * The constants in this namespace represent logical module connections.
 * Shared managers use these names without needing to know which ESP32
 * variant or development board is currently being compiled.
 */
namespace ModulePins
{

#if defined(C3N0_S3)

    /**
     * @name Shared SPI bus
     * @brief Hardware SPI controller and GPIO pins shared by SPI modules.
     * @{
     */

    /**
     * @brief Hardware SPI controller used by the ESP32-S3 node.
     *
     * ESP32-S3 exposes FSPI as its primary general-purpose SPI controller.
     */
    static constexpr uint8_t SPI_BUS = FSPI;

    /**
     * @brief Shared SPI clock pin.
     */
    static constexpr uint8_t SPI_SCK = 12;

    /**
     * @brief Shared SPI MISO pin.
     */
    static constexpr uint8_t SPI_MISO = 13;

    /**
     * @brief Shared SPI MOSI pin.
     */
    static constexpr uint8_t SPI_MOSI = 11;

    /** @} */

    /**
     * @name NRF24
     * @brief GPIO pins dedicated to the NRF24 radio.
     * @{
     */

    /**
     * @brief NRF24 Chip Enable pin.
     */
    static constexpr uint8_t NRF_CE = 9;

    /**
     * @brief NRF24 Chip Select pin.
     */
    static constexpr uint8_t NRF_CSN = 10;

    /** @} */

    /**
     * @name CC1101
     * @brief GPIO pins dedicated to the CC1101 radio.
     * @{
     */

    /**
     * @brief CC1101 Chip Select pin.
     */
    static constexpr uint8_t CC1101_CSN = 40;

    /**
     * @brief CC1101 GDO0 digital output pin.
     */
    static constexpr uint8_t CC1101_GDO0 = 39;

    /** @} */

    /**
     * @name OLED
     * @brief GPIO pins used by the Ch3rryN0de OLED display.
     * @{
     */

    /**
     * @brief OLED I2C SDA pin.
     */
    static constexpr uint8_t OLED_SDA = 41;

    /**
     * @brief OLED I2C SCL pin.
     */
    static constexpr uint8_t OLED_SCL = 42;

    /** @} */

#else

    /**
     * @name Shared SPI bus
     * @brief Hardware SPI controller and GPIO pins used by existing ESP32 hardware.
     * @{
     */

    /**
     * @brief Hardware SPI controller used by the classic ESP32 hardware.
     *
     * Existing Ch3rryB0mb and ESP32-based Ch3rryN0de hardware use VSPI
     * for the shared external module bus.
     */
    static constexpr uint8_t SPI_BUS = VSPI;

    /**
     * @brief Shared SPI clock pin.
     */
    static constexpr uint8_t SPI_SCK = 18;

    /**
     * @brief Shared SPI MISO pin.
     */
    static constexpr uint8_t SPI_MISO = 19;

    /**
     * @brief Shared SPI MOSI pin.
     */
    static constexpr uint8_t SPI_MOSI = 23;

    /** @} */

    /**
     * @name NRF24
     * @brief GPIO pins dedicated to the NRF24 radio.
     * @{
     */

    /**
     * @brief NRF24 Chip Enable pin.
     */
    static constexpr uint8_t NRF_CE = 27;

    /**
     * @brief NRF24 Chip Select pin.
     */
    static constexpr uint8_t NRF_CSN = 22;

    /** @} */

    /**
     * @name CC1101
     * @brief GPIO pins dedicated to the CC1101 radio.
     * @{
     */

    /**
     * @brief CC1101 Chip Select pin.
     */
    static constexpr uint8_t CC1101_CSN = 16;

    /**
     * @brief CC1101 GDO0 digital output pin.
     */
    static constexpr uint8_t CC1101_GDO0 = 17;

    /** @} */

    /**
     * @name OLED
     * @brief GPIO pins used by the ESP32-based Ch3rryN0de OLED display.
     * @{
     */

    /**
     * @brief OLED I2C SDA pin.
     */
    static constexpr uint8_t OLED_SDA = 21;

    /**
     * @brief OLED I2C SCL pin.
     */
    static constexpr uint8_t OLED_SCL = 25;

    /** @} */

#endif

}