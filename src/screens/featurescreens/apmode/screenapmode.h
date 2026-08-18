#pragma once

#include <lvgl.h>

class ScreenManager;
class APMode;

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
     * connects its controls to the provided ScreenManager and APMode.
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
     * @brief AP start button.
     */
    static lv_obj_t* startButton;

    /**
     * @brief AP stop button.
     */
    static lv_obj_t* stopButton;

    /**
     * @brief Handles the AP Mode start button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void startClicked(lv_event_t* event);

    /**
     * @brief Handles the AP Mode stop button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void stopClicked(lv_event_t* event);

    /**
     * @brief AP configuration button.
     *
     * The button is disabled while AP Mode is running to prevent
     * configuration changes to an active access point.
     */
    static lv_obj_t* configButton;

    /**
     * @brief Handles the AP Mode config button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void configClicked(lv_event_t* event);

    /**
     * @brief Handles the AP Mode back button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);

    /**
     * @brief Label used to display the current AP status.
     *
     * The label is created when the screen is constructed and updated
     * whenever the access point is started or stopped.
     */
    static lv_obj_t* statusText;

    /**
     * @brief Updates the AP status information shown on the screen.
     *
     * Displays either the access point connection information or a
     * message indicating that AP Mode is currently stopped.
     *
     * @param apMode Reference to the AP Mode feature.
     */
    static void updateStatus(APMode& apMode);
};