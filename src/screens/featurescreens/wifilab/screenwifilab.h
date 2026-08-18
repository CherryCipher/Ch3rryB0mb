/**
 * @file screenwifilab.h
 * @brief Wi-Fi Lab screen interface.
 *
 * This file defines the Wi-Fi Lab screen used to scan for and
 * display nearby wireless Access Points.
 */

#pragma once

#include <lvgl.h>

class ScreenManager;
class WiFiLab;

/**
 * @class ScreenWifiLab
 * @brief Provides the Ch3rryB0mb Wi-Fi Lab screen.
 *
 * The ScreenWifiLab displays nearby wireless Access Points discovered
 * through the WiFiLab feature.
 *
 * The screen is responsible only for presentation and user interaction.
 * Wi-Fi scanning itself is delegated to the WiFiLab feature.
 */
class ScreenWifiLab
{
public:
    /**
     * @brief Creates the Wi-Fi Lab screen.
     *
     * Creates the screen controls, performs an initial Wi-Fi scan
     * and displays the discovered Access Points.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param wifiLab Reference to the WiFiLab feature.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(ScreenManager& screenManager, WiFiLab& wifiLab);

private:
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
     * creates a new row for every discovered Access Point.
     *
     * @param wifiLab Reference to the WiFiLab feature.
     */
    static void renderNetworks(WiFiLab& wifiLab);

    /**
     * @brief Handles the Wi-Fi Lab refresh button event.
     *
     * Performs a new Wi-Fi scan and refreshes the displayed
     * network list.
     *
     * @param event Pointer to the LVGL event.
     */
    static void scanClicked(lv_event_t* event);

    /**
     * @brief Handles the Wi-Fi Lab back button event.
     *
     * Retrieves the ScreenManager from the LVGL event user data
     * and requests navigation to the previous screen.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);
};