#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/**
 * @class NodeDisplayManager
 * @brief Manages the Ch3rryN0de OLED display.
 */
class NodeDisplayManager
{
public:
    /**
     * @brief Initializes the OLED display.
     *
     * @return True when the display initialized successfully.
     */
    bool begin();

    /**
     * @brief Clears the display.
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

    Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    void prepareScreen();
    void printStatus(const char* label, bool ok);
};