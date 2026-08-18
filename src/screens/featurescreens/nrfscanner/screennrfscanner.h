/**
 * @file screennrfscanner.h
 * @brief NRF Scanner screen interface.
 *
 * This file defines the NRF Scanner screen used to display
 * NRF24 radio scanning functionality.
 */

#pragma once
#include <lvgl.h>

class ScreenManager;

/**
 * @class ScreenNrfScanner
 * @brief Provides the Ch3rryB0mb NRF Scanner screen.
 *
 * The ScreenNrfScanner provides the user interface for
 * NRF24 radio scanning functionality.
 */
class ScreenNrfScanner
{
public:
    /**
     * @brief Creates the NRF Scanner screen.
     *
     * Creates the NRF Scanner interface with a header
     * and navigation controls.
     *
     * @param screenManager Reference to the application ScreenManager.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(ScreenManager& screenManager);

private:
    /**
     * @brief Handles the NRF Scanner back button event.
     *
     * Retrieves the ScreenManager from the LVGL event user data
     * and requests navigation to the previous screen.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);
};