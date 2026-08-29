#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/**
 * @class NodeDisplayManager
 * @brief Manages the optional Ch3rryN0de OLED display.
 *
 * The display is detected during initialization. If no OLED is connected,
 * the node continues running normally and all display operations are ignored.
 */
class NodeDisplayManager
{
public:
    /**
     * @brief Initializes the optional OLED display.
     *
     * Initializes the I2C bus and checks whether an OLED is present at the
     * configured address. The node remains operational when no display is found.
     *
     * @return Always true because the OLED is optional.
     */
    bool begin();

    /**
     * @brief Returns whether the OLED is available.
     *
     * @return true when the OLED was detected and initialized successfully.
     */
    bool isAvailable() const;

    /**
     * @brief Clears the display when available.
     */
    void clear();

    /**
     * @brief Shows the node boot screen.
     */
    void showBoot();

    /**
     * @brief Shows the hardware initialization status.
     *
     * @param nrfOk True when the NRF24 initialized successfully.
     * @param cc1101Ok True when the CC1101 initialized successfully.
     * @param bleOk True when BLE initialized successfully.
     */
    void showHardwareStatus(bool nrfOk, bool cc1101Ok, bool bleOk);

    /**
     * @brief Shows BLE configuration mode.
     */
    void showConfigMode();

    /**
     * @brief Shows the active NRF mode.
     *
     * @param mode Active mode name.
     * @param channel Active NRF channel.
     */
    void showNRFMode(const char* mode, uint8_t channel);

    /**
     * @brief Shows the active CC1101 mode.
     *
     * @param mode Active mode name.
     * @param frequency Active frequency in MHz.
     */
    void showCC1101Mode(const char* mode, float frequency);

    /**
     * @brief Shows the active BLE mode.
     *
     * @param mode Active BLE mode name.
     */
    void showBLEMode(const char* mode);

    /**
     * @brief Shows a received message.
     *
     * @param message Message to display.
     */
    void showMessage(const char* message);

private:
    static constexpr uint8_t SCREEN_WIDTH = 128;
    static constexpr uint8_t SCREEN_HEIGHT = 64;
    static constexpr uint8_t OLED_ADDRESS = 0x3C;
    static constexpr int8_t OLED_RESET = -1;
    static constexpr uint32_t I2C_FREQUENCY = 100000;

    Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    bool available = false;

    void prepareScreen();
    void printStatus(const char* label, bool ok);
};