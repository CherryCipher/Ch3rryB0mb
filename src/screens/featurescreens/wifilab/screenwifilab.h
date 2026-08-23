/**
 * @file screenwifilab.h
 * @brief Wi-Fi Lab screen interface.
 *
 * This file defines the Wi-Fi Lab screen used to scan for,
 * display and select nearby wireless Access Points.
 */

#pragma once
#include <lvgl.h>
#include <WiFi.h>

class ScreenManager;
class WiFiLab;

/**
 * @class ScreenWifiLab
 * @brief Provides the Ch3rryB0mb Wi-Fi Lab screen.
 *
 * The ScreenWifiLab displays nearby wireless Access Points discovered
 * through the WiFiLab feature.
 *
 * Network rows can be selected to prepare a network for use by other
 * Wi-Fi features.
 *
 * The screen is responsible only for presentation and user interaction.
 * Wi-Fi operations are delegated to the WiFiLab feature.
 */
class ScreenWifiLab
{
public:
    /**
     * @brief Creates the Wi-Fi Lab screen.
     *
     * Creates the screen controls and a scrollable list used to display
     * discovered Access Points.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param wifiLab Reference to the WiFiLab feature.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(ScreenManager& screenManager, WiFiLab& wifiLab);

private:
    /**
     * @struct ScreenContext
     * @brief Context shared by Wi-Fi Lab callbacks.
     */
    struct ScreenContext
    {
        ScreenManager* screenManager = nullptr;
        WiFiLab* wifiLab = nullptr;
    };

    /**
     * @brief Scan button.
     */
    static lv_obj_t* scanButton;

    /**
     * @brief Container used to display discovered Wi-Fi networks.
     *
     * Stored as a screen member so the network list can be cleared
     * and rebuilt after a new scan.
     */
    static lv_obj_t* networkContainer;

    /**
     * @brief Context shared by Wi-Fi Lab callbacks.
     */
    static ScreenContext screenContext;

    /**
     * @brief Scans for nearby Wi-Fi networks and updates the screen.
     *
     * Requests a new scan through the WiFiLab feature and rebuilds
     * the displayed network list when the scan succeeds.
     *
     * @param wifiLab Reference to the WiFiLab feature.
     */
    static void scanNetworks(WiFiLab& wifiLab);

    /**
     * @brief Displays the current Wi-Fi scan results.
     *
     * Clears the existing contents of the network container and
     * creates a selectable row for every discovered Access Point.
     *
     * @param wifiLab Reference to the WiFiLab feature.
     */
    static void renderNetworks(WiFiLab& wifiLab);

    /**
     * @brief Handles the Wi-Fi Lab scan button event.
     *
     * Performs a new Wi-Fi scan and refreshes the displayed
     * network list.
     *
     * @param event Pointer to the LVGL event.
     */
    static void scanClicked(lv_event_t* event);

    /**
     * @brief Handles selection of a discovered Wi-Fi network.
     *
     * Selects the network represented by the clicked row through
     * the WiFiLab feature.
     *
     * @param event Pointer to the LVGL event.
     */
    static void networkClicked(lv_event_t* event);

    /**
     * @brief Handles the Wi-Fi Lab back button event.
     *
     * Retrieves the ScreenManager from the LVGL event user data
     * and requests navigation to the previous screen.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);

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
    static const char* getSecurityName(wifi_auth_mode_t security);

    /**
     * @brief Cleans up Wi-Fi Lab screen resources.
     *
     * Releases the Wi-Fi subsystem when no Station connection is active,
     * allowing other wireless features to reuse the ESP32 radio resources.
     *
     * An active Wi-Fi connection is intentionally preserved when leaving
     * the screen.
     *
     * @param event Pointer to the LVGL delete event.
     */
    static void screenDeleted(lv_event_t* event);
};