/**
 * @file screenapmodeconfig.h
 * @brief AP Mode configuration screen for Ch3rryB0mb.
 *
 * This file defines the ScreenAPModeConfig class responsible for
 * displaying and editing the Access Point configuration.
 */

#pragma once

#include <lvgl.h>

class ScreenManager;
class APMode;

/**
 * @class ScreenAPModeConfig
 * @brief Provides the AP Mode configuration screen.
 *
 * ScreenAPModeConfig allows the user to edit settings used by AP Mode.
 *
 * The screen uses a scrollable content container so configuration
 * fields can extend beyond the physical display area while the header
 * remains fixed.
 */
class ScreenAPModeConfig
{
public:
    /**
     * @brief Creates the AP Mode configuration screen.
     *
     * Creates the standard Ch3rryB0mb screen, fixed header,
     * scrollable configuration area and input controls.
     *
     * The current AP configuration is used to initialize the
     * configuration fields.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param apMode Reference to the AP Mode feature.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(
        ScreenManager& screenManager,
        APMode& apMode
    );

private:
    /**
     * @brief SSID input field.
     */
    static lv_obj_t* ssidInput;

    /**
     * @brief Access Point password input field.
     */
    static lv_obj_t* passwordInput;

    /**
     * @brief Handles the SAVE button event.
     *
     * The actual configuration update functionality will be connected
     * to APMode when configuration apply support is implemented.
     *
     * @param event Pointer to the LVGL event.
     */
    static void saveClicked(lv_event_t* event);

    /**
     * @brief Handles the BACK button event.
     *
     * Requests navigation to the previous screen through ScreenManager.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);
};