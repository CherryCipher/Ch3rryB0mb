#pragma once

#include <lvgl.h>

class ScreenManager;

/**
 * @class ScreenMainMenu
 * @brief Provides the Ch3rryB0mb main menu screen.
 *
 * The ScreenMainMenu creates the main application menu and handles
 * user interface events that originate from this screen.
 *
 * Navigation itself is delegated to the ScreenManager.
 */
class ScreenMainMenu
{
public:
    /**
     * @brief Creates the main menu screen.
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
     * @brief Handles the AP Mode menu button event.
     *
     * Retrieves the ScreenManager from the LVGL event user data
     * and requests navigation to the AP Mode screen.
     *
     * @param event Pointer to the LVGL event.
     */
    static void apModeClicked(lv_event_t* event);
};