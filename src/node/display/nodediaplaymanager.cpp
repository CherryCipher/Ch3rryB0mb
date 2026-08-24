#include "nodedisplaymanager.h"

#include <Wire.h>

static constexpr uint8_t OLED_SDA = 21;
static constexpr uint8_t OLED_SCL = 25;

/**
 * @brief Initializes the OLED display.
 *
 * @return True when the display initialized successfully.
 */
bool NodeDisplayManager::begin()
{
    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) return false;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(true);
    display.display();

    return true;
}

/**
 * @brief Clears the display.
 */
void NodeDisplayManager::clear()
{
    display.clearDisplay();
    display.display();
}

/**
 * @brief Shows the node boot screen.
 */
void NodeDisplayManager::showBoot()
{
    prepareScreen();

    display.setTextSize(2);
    display.println("C3N0");

    display.setTextSize(1);
    display.println();
    display.println("BOOTING...");

    display.display();
}

/**
 * @brief Shows the hardware initialization status.
 *
 * @param nrfOk True when the NRF24 initialized successfully.
 * @param cc1101Ok True when the CC1101 initialized successfully.
 * @param bleOk True when BLE initialized successfully.
 */
void NodeDisplayManager::showHardwareStatus(bool nrfOk, bool cc1101Ok, bool bleOk)
{
    prepareScreen();

    display.setTextSize(1);
    display.println("C3N0 HARDWARE");
    display.println();

    printStatus("NRF24", nrfOk);
    printStatus("CC1101", cc1101Ok);
    printStatus("BLE", bleOk);

    display.display();
}

/**
 * @brief Shows BLE configuration mode.
 */
void NodeDisplayManager::showConfigMode()
{
    prepareScreen();

    display.setTextSize(1);
    display.println("C3N0");
    display.println("----------------");
    display.println("CONFIG MODE");
    display.println();
    display.println("BLE ACTIVE");
    display.println("WAITING FOR C3B0");

    display.display();
}

/**
 * @brief Shows the active NRF mode.
 *
 * @param mode Active mode name.
 * @param channel Active NRF channel.
 */
void NodeDisplayManager::showNRFMode(const char* mode, uint8_t channel)
{
    prepareScreen();

    display.setTextSize(1);
    display.println("C3N0 - NRF24");
    display.println("----------------");

    display.print("MODE: ");
    display.println(mode);

    display.print("CH:   ");
    display.println(channel);

    display.display();
}

/**
 * @brief Shows the active CC1101 mode.
 *
 * @param mode Active mode name.
 * @param frequency Active frequency in MHz.
 */
void NodeDisplayManager::showCC1101Mode(const char* mode, float frequency)
{
    prepareScreen();

    display.setTextSize(1);
    display.println("C3N0 - CC1101");
    display.println("----------------");

    display.print("MODE: ");
    display.println(mode);

    display.print("FREQ: ");
    display.print(frequency, 3);
    display.println(" MHz");

    display.display();
}

/**
 * @brief Shows the active BLE mode.
 *
 * @param mode Active BLE mode name.
 */
void NodeDisplayManager::showBLEMode(const char* mode)
{
    prepareScreen();

    display.setTextSize(1);
    display.println("C3N0 - BLE");
    display.println("----------------");

    display.print("MODE: ");
    display.println(mode);

    display.display();
}

/**
 * @brief Shows a received message.
 *
 * @param message Message to display.
 */
void NodeDisplayManager::showMessage(const char* message)
{
    prepareScreen();

    display.setTextSize(1);
    display.println("C3N0 MESSAGE");
    display.println("----------------");
    display.println(message);

    display.display();
}

/**
 * @brief Prepares the display for drawing a new screen.
 */
void NodeDisplayManager::prepareScreen()
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
}

/**
 * @brief Prints a hardware status row.
 *
 * @param label Hardware label.
 * @param ok Hardware status.
 */
void NodeDisplayManager::printStatus(const char* label, bool ok)
{
    display.print(label);
    display.print(": ");

    if (ok) display.println("OK");
    else display.println("FAIL");
}