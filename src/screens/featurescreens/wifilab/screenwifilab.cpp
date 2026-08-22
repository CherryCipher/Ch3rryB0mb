/**
 * @file screenwifilab.cpp
 * @brief Implementation of the Wi-Fi Lab screen.
 *
 * This file creates the Wi-Fi Lab interface, performs network scans
 * through the WiFiLab feature and displays discovered Access Points.
 */

#include "screenwifilab.h"

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/wifilab/wifilab.h"

/**
 * @brief Container used to display discovered Wi-Fi networks.
 */
lv_obj_t* ScreenWifiLab::networkContainer = nullptr;

/**
 * @brief Scanbutton used by the AP Mode screen.
 * 
 * its a member so we can show and hide the button
 */
lv_obj_t* ScreenWifiLab::scanButton = nullptr;

/**
 * @brief Creates the Wi-Fi Lab screen.
 *
 * Creates the Wi-Fi Lab interface with BACK and REFRESH controls,
 * a scrollable network list and an initial Wi-Fi scan.
 *
 * @param screenManager Reference to the application ScreenManager.
 * @param wifiLab Reference to the WiFiLab feature.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenWifiLab::create(ScreenManager& screenManager, WiFiLab& wifiLab)
{
    // Screen background.
    lv_obj_t* screen = UIWidgets::createScreen();

    // Header.
    UIWidgets::addHeader(screen, 0, 0, "WIFIL4B");

    // Back button.
    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);

    lv_obj_add_event_cb(
        backButton,
        backClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    scanButton = UIWidgets::addButton(screen, 15, 50, "SCAN", 100, 35);

    lv_obj_add_event_cb(
        scanButton,
        scanClicked,
        LV_EVENT_CLICKED,
        &wifiLab
    );

    // Scrollable container for discovered networks. Filled with scanNetworks() and renderNetworks()
    networkContainer = UIWidgets::createScrollContainer(screen, 10, 95, 220, 210);

    return screen;
}

/**
 * @brief Scans for nearby Wi-Fi networks and updates the screen.
 *
 * Requests a new scan through the WiFiLab feature. When the scan
 * succeeds, the displayed network list is rebuilt.
 *
 * @param wifiLab Reference to the WiFiLab feature.
 */
void ScreenWifiLab::scanNetworks(WiFiLab& wifiLab)
{
    lv_obj_t* screen = lv_scr_act();

    lv_obj_t* statusBox = UIWidgets::addStatusBox(screen, "SCANNING...");

    lv_obj_add_event_cb(
        screen,
        screenDeleted,
        LV_EVENT_DELETE,
        &wifiLab
    );

    // Force LVGL to draw the status box before starting the blocking scan.
    lv_refr_now(nullptr);

    bool success = wifiLab.scan();

    lv_obj_del(statusBox);

    if (!success)
    {
        if (networkContainer == nullptr)
            return;

        lv_obj_clean(networkContainer);
        UIWidgets::addText(networkContainer, 5, 5, "Wi-Fi scan failed.", 200);

        return;
    }

    renderNetworks(wifiLab);
}

/**
 * @brief Displays the current Wi-Fi scan results.
 *
 * Removes previously displayed network entries and creates a new
 * text entry for every Access Point returned by the WiFiLab feature.
 *
 * Each entry currently displays the SSID, channel and RSSI value.
 *
 * @param wifiLab Reference to the WiFiLab feature.
 */
void ScreenWifiLab::renderNetworks(WiFiLab& wifiLab)
{
    if (networkContainer == nullptr)
        return;

    lv_obj_clean(networkContainer);

    int networkCount = wifiLab.getNetworkCount();

    if (networkCount == 0)
    {
        UIWidgets::addText(networkContainer, 5, 5, "No Wi-Fi networks found.", 200);
        return;
    }

    for (int i = 0; i < networkCount; i++)
    {
        const WiFiNetwork& network = wifiLab.getNetwork(i);

        String ssid = network.ssid;

        if (ssid.length() == 0)
            ssid = "< hidden >";

        if (ssid.length() > 22)
            ssid = ssid.substring(0, 19) + "...";

        String text =
            ssid + "\n"
            "CH " + String(network.channel) +
            "   " + String(network.rssi) + " dBm" +
            "   " + getSecurityName(network.encryption) + "\n" +
            network.bssid;

        //42 height for 2 lines, 58 for 3, 74 for 4
        lv_obj_t* networkLabel = UIWidgets::addText(networkContainer, 5, 5 + (i * 74), text.c_str(), 195);

        //if the network is open color it red!
        if (network.encryption == WIFI_AUTH_OPEN)
            lv_obj_set_style_text_color(networkLabel, lv_color_hex(0xFF1744), LV_PART_MAIN);
    }
}

/**
 * @brief Handles the Wi-Fi scan button event.
 *
 * Requests a network scan through the WiFiLab feature.
 * After the first successful scan, the button label changes
 * from SCAN to REFRESH for subsequent scans.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenWifiLab::scanClicked(lv_event_t* event)
{
    WiFiLab* wifiLab = static_cast<WiFiLab*>(lv_event_get_user_data(event));

    if (wifiLab == nullptr)
        return;

    scanNetworks(*wifiLab);

    lv_obj_t* label = lv_obj_get_child(scanButton, 0);

    if (label != nullptr)
        lv_label_set_text(label, "REFRESH");
}

/**
 * @brief Handles the Wi-Fi Lab back button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenWifiLab::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    screenManager->back();
}

/**
 * @brief Returns a readable name for a Wi-Fi authentication mode.
 *
 * Converts the ESP32 Wi-Fi authentication mode to a short
 * human-readable string for display in the Wi-Fi network list.
 *
 * @param security ESP32 Wi-Fi authentication mode.
 *
 * @return Human-readable security name.
 */
const char* ScreenWifiLab::getSecurityName(wifi_auth_mode_t security)
{
    switch (security)
    {
        case WIFI_AUTH_OPEN: return "OPEN";
        case WIFI_AUTH_WEP: return "WEP";
        case WIFI_AUTH_WPA_PSK: return "WPA";
        case WIFI_AUTH_WPA2_PSK: return "WPA2";
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
        case WIFI_AUTH_WPA3_PSK: return "WPA3";
        case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Cleans up WiFi Lab resources when the screen is deleted.
 *
 * Shuts down the Wi-Fi subsystem and clears stored UI references when
 * leaving the WiFi Lab screen.
 *
 * @param event Pointer to the LVGL delete event.
 */
void ScreenWifiLab::screenDeleted(lv_event_t* event)
{
    WiFiLab* wifiLab = static_cast<WiFiLab*>(lv_event_get_user_data(event));

    if (wifiLab != nullptr)
        wifiLab->shutdown();

    networkContainer = nullptr;
    scanButton = nullptr;
}