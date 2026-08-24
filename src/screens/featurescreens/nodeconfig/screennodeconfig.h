/**
 * @file screennodeconfig.h
 * @brief Declaration of the Ch3rryN0de configuration screen.
 *
 * The screen allows the user to configure the radio and operating mode
 * for a selected Ch3rryN0de.
 */

#pragma once

#include <lvgl.h>

#include "app/features/configurenode/configurenode.h"

class ScreenManager;

/**
 * @class ScreenNodeConfig
 * @brief Provides configuration controls for a selected Ch3rryN0de.
 */
class ScreenNodeConfig
{
public:
    /**
     * @brief Creates the node configuration screen.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param configureNode Reference to the ConfigureNode feature.
     *
     * @return Pointer to the created LVGL screen.
     */
    static lv_obj_t* create(ScreenManager& screenManager, ConfigureNode& configureNode);

private:
    /**
     * @struct Context
     * @brief Callback context for the node configuration screen.
     */
    struct Context
    {
        ScreenManager* screenManager = nullptr;
        ConfigureNode* configureNode = nullptr;
    };

    static Context context;

    static lv_obj_t* radioButton;
    static lv_obj_t* modeButton;
    static lv_obj_t* parameterLabel;

    /**
     * @brief Updates the configuration controls.
     */
    static void updateControls();

    /**
     * @brief Handles the radio selection button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void radioClicked(lv_event_t* event);

    /**
     * @brief Handles the mode selection button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void modeClicked(lv_event_t* event);

    /**
     * @brief Handles the start button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void startClicked(lv_event_t* event);

    /**
     * @brief Handles the back button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);

    /**
     * @brief Cleans up static screen references.
     *
     * @param event Pointer to the LVGL event.
     */
    static void screenDeleted(lv_event_t* event);
};