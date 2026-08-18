#pragma once

#include <Arduino.h>

/**
 * @class SoftTouch
 * @brief Software SPI driver for the XPT2046 touch controller.
 *
 * This driver communicates with the XPT2046 using software SPI.
 * It does not use one of the ESP32 hardware SPI controllers.
 *
 * This allows the hardware SPI controllers to remain available
 * for the display and SD card.
 */
class SoftTouch
{
public:
    /**
     * @brief Constructs the software touch controller.
     *
     * @param mosi MOSI pin used by the touch controller.
     * @param miso MISO pin used by the touch controller.
     * @param clock Clock pin used by the touch controller.
     * @param cs Chip-select pin used by the touch controller.
     * @param irq Interrupt pin used by the touch controller.
     */
    SoftTouch( uint8_t mosi, uint8_t miso, uint8_t clock, uint8_t cs, uint8_t irq );

    /**
     * @brief Initializes the touch controller pins.
     *
     * @return true if initialization completed successfully.
     */
    bool start();

    /**
     * @brief Checks whether the touchscreen is currently pressed.
     *
     * The XPT2046 IRQ line is active LOW.
     *
     * @return true if the screen is being touched.
     */
    bool touched() const;

    /**
     * @brief Reads the raw X and Y touch coordinates.
     *
     * Values returned by the XPT2046 are 12-bit ADC values
     * in the range 0 to 4095.
     *
     * These values still need to be calibrated and mapped to
     * the display resolution.
     *
     * @param x Destination for the raw X coordinate.
     * @param y Destination for the raw Y coordinate.
     *
     * @return true if a touch position was read.
     * @return false if the screen is not currently touched.
     */
    bool read(uint16_t& x, uint16_t& y);

private:
    /**
     * @brief Transfers one byte using software SPI.
     *
     * @param value Byte to transmit.
     *
     * @return Byte received from the touch controller.
     */
    uint8_t transfer(uint8_t value);

    /**
     * @brief Reads a 12-bit ADC value from the XPT2046.
     *
     * @param command XPT2046 command byte.
     *
     * @return 12-bit ADC value.
     */
    uint16_t readADC(uint8_t command);

    uint8_t mosiPin;
    uint8_t misoPin;
    uint8_t clockPin;
    uint8_t csPin;
    uint8_t irqPin;

    bool running = false;
};