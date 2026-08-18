#pragma once

#include <lvgl.h>

class ScreenManager;

/**
 * @class ScreenWifiLab
 * @brief Provides the Ch3rryB0mb Wifi Lab Screen.
 *
 * The ScreenWifiLab creates the wifi lab screen for scanning and inspecting AP's
 * It also handles user interface events that originate from this screen.
 *
 * Navigation itself is delegated to the ScreenManager.
 */
class ScreenWifiLab
{
public:
    /**
     * @brief Creates the wifilab screen.
     *
     * Creates the LVGL objects required for the main menu and
     * connects its navigation events to the provided ScreenManager.
     *
     * @param screenManager Reference to the application ScreenManager.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(ScreenManager& screenManager);

private:
    /**
     * @brief Handles the WifiLab back button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);
};