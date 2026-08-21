/**
 * @file screencc1101scanner.h
 * @brief CC1101 Sub-GHz Scanner screen interface.
 *
 * This file defines the Sub-GHz Scanner screen used to control
 * CC1101 radio scanning functionality.
 */

#pragma once

#include <Arduino.h>
#include <lvgl.h>

class ScreenManager;
class CC1101Scanner;

/**
 * @class ScreenCC1101Scanner
 * @brief Provides the Ch3rryB0mb Sub-GHz Scanner screen.
 *
 * The screen provides basic start and stop controls for the CC1101
 * scanner feature.
 *
 * Sub-GHz scanning is delegated to the CC1101Scanner feature.
 */
class ScreenCC1101Scanner
{
public:
    /**
     * @brief Creates the Sub-GHz Scanner screen.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param cc1101Scanner Reference to the CC1101Scanner feature.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(ScreenManager& screenManager, CC1101Scanner& cc1101Scanner);

private:
    /**
     * @struct BackContext
     * @brief Holds references required by the back navigation event.
     */
    struct BackContext
    {
        ScreenManager* screenManager;
        CC1101Scanner* cc1101Scanner;
    };

    /**
     * @brief Context used by the back button.
     */
    static BackContext backContext;

    /**
     * @brief Handles the scanner start button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void startClicked(lv_event_t* event);

    /**
     * @brief Handles the scanner stop button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void stopClicked(lv_event_t* event);

    /**
     * @brief Handles the Sub-GHz Scanner back button event.
     *
     * Stops scanning and navigates back to the previous screen.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);
};