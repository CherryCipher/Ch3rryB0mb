#include "nodedisplaymanager.h"

#include "hardware/modulepins.h"

/**
 * @brief Initializes the optional node OLED display.
 *
 * Initializes the I2C bus, probes the configured OLED address and starts
 * the SSD1306 display when present. Absence of the OLED is not considered
 * an error because the display is optional.
 *
 * @return Always true because the node may operate without an OLED.
 */
bool NodeDisplayManager::begin()
{
    available = false;

    if (!Wire.begin(ModulePins::OLED_SDA, ModulePins::OLED_SCL, I2C_FREQUENCY)) {
        Serial.println("[OLED] I2C initialization failed. Continuing without OLED.");
        return true;
    }

    Wire.beginTransmission(OLED_ADDRESS);
    uint8_t result = Wire.endTransmission();

    if (result != 0) {
        Serial.println("[OLED] OLED not detected. Continuing without OLED.");
        return true;
    }

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS, true, false)) {
        Serial.println("[OLED] OLED detected but initialization failed. Continuing without OLED.");
        return true;
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(true);
    display.display();

    available = true;

    Serial.println("[OLED] OLED initialized.");

    return true;
}

/**
 * @brief Returns whether the OLED is available.
 *
 * @return true when the OLED was detected and initialized successfully.
 */
bool NodeDisplayManager::isAvailable() const
{
    return available;
}

/**
 * @brief Clears the display when available.
 */
void NodeDisplayManager::clear()
{
    if (!available) return;

    display.clearDisplay();
    display.display();
}

/**
 * @brief Shows the node boot screen.
 */
void NodeDisplayManager::showBoot()
{
    if (!available) return;

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
    if (!available) return;

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
    if (!available) return;

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
    if (!available) return;

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
    if (!available) return;

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
    if (!available) return;

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
    if (!available) return;

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
    if (!available) return;

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
    if (!available) return;

    display.print(label);
    display.print(": ");

    if (ok) display.println("OK");
    else display.println("FAIL");
}