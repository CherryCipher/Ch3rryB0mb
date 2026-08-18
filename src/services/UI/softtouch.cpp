#include "softtouch.h"

/**
 * @brief XPT2046 command used to read the X position.
 */
static constexpr uint8_t XPT2046_CMD_X = 0xD0;

/**
 * @brief XPT2046 command used to read the Y position.
 */
static constexpr uint8_t XPT2046_CMD_Y = 0x90;

/**
 * @brief Constructs the software touch controller.
 */
SoftTouch::SoftTouch(
    uint8_t mosi,
    uint8_t miso,
    uint8_t clock,
    uint8_t cs,
    uint8_t irq
)
    : mosiPin(mosi),
      misoPin(miso),
      clockPin(clock),
      csPin(cs),
      irqPin(irq)
{
}

/**
 * @brief Initializes the software SPI touch controller.
 */
bool SoftTouch::start()
{
    pinMode(mosiPin, OUTPUT);
    pinMode(misoPin, INPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(csPin, OUTPUT);
    pinMode(irqPin, INPUT_PULLUP);

    //
    // SPI idle state.
    //
    digitalWrite(clockPin, LOW);
    digitalWrite(mosiPin, LOW);
    digitalWrite(csPin, HIGH);

    running = true;

    return true;
}

/**
 * @brief Checks whether the touchscreen is being pressed.
 */
bool SoftTouch::touched() const
{
    if (!running)
    {
        return false;
    }

    //
    // XPT2046 IRQ is active LOW.
    //
    return digitalRead(irqPin) == LOW;
}

/**
 * @brief Reads the raw X and Y touch coordinates.
 */
bool SoftTouch::read(uint16_t& x, uint16_t& y)
{
    if (!running || !touched())
    {
        return false;
    }

    digitalWrite(csPin, LOW);

    //
    // Read the raw 12-bit touch coordinates.
    //
    x = readADC(XPT2046_CMD_X);
    y = readADC(XPT2046_CMD_Y);

    digitalWrite(csPin, HIGH);

    return true;
}

/**
 * @brief Transfers one byte using software SPI.
 */
uint8_t SoftTouch::transfer(uint8_t value)
{
    uint8_t result = 0;

    //
    // XPT2046 uses SPI mode 0.
    // Data is shifted MSB first.
    //
    for (int bit = 7; bit >= 0; --bit)
    {
        digitalWrite(
            mosiPin,
            (value & (1 << bit)) ? HIGH : LOW
        );

        digitalWrite(clockPin, HIGH);

        result <<= 1;

        if (digitalRead(misoPin))
        {
            result |= 1;
        }

        digitalWrite(clockPin, LOW);
    }

    return result;
}

/**
 * @brief Reads one 12-bit ADC channel from the XPT2046.
 */
uint16_t SoftTouch::readADC(uint8_t command)
{
    //
    // Send ADC channel selection command.
    //
    transfer(command);

    //
    // The XPT2046 returns the 12-bit ADC result
    // left-aligned across the following two bytes.
    //
    uint16_t value =
        static_cast<uint16_t>(transfer(0x00)) << 8;

    value |= transfer(0x00);

    //
    // Discard unused bits and keep the 12-bit result.
    //
    value >>= 3;

    return value & 0x0FFF;
}