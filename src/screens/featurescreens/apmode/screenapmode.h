#pragma once

#include <lvgl.h>

class ScreenManager;

/**
 * @class ScreenAPMode
 * @brief Provides the Ch3rryB0mb AP Mode screen.
 *
 * The ScreenAPMode creates the AP Mode user interface and handles
 * user interface events that belong specifically to AP Mode.
 *
 * Navigation is delegated to the ScreenManager.
 */
class ScreenAPMode
{
public:
    /**
     * @brief Creates the AP Mode screen.
     *
     * Creates the LVGL objects required for the AP Mode screen and
     * connects its controls to the provided ScreenManager.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param apMode Reference to the AP MOde feature.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(ScreenManager& screenManager, Features& feature);

private:
    /**
     * @brief Handles the AP Mode start button event.
     *
     * Called when the user activates the START control.
     *
     * @param event Pointer to the LVGL event.
     */
    static void startClicked(lv_event_t* event);

    /**
     * @brief Handles the AP Mode stop button event.
     *
     * Called when the user activates the STOP control.
     *
     * @param event Pointer to the LVGL event.
     */
    static void stopClicked(lv_event_t* event);

    /**
     * @brief Handles the AP Mode back button event.
     *
     * Retrieves the ScreenManager from the LVGL event user data
     * and requests navigation to the previous screen.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);

    /**
    * @brief Reference to the application's Features to hand of to screens.
    *.
    */
    Features& features;
};