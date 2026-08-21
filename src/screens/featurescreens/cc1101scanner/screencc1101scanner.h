/**
 * @file screencc1101scanner.h
 * @brief CC1101 Sub-GHz Scanner screen interface.
 *
 * This file defines the Sub-GHz Scanner screen used to configure
 * and visualize continuous CC1101 spectrum scanning.
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
 * The screen provides center frequency and scan range controls,
 * start and stop controls and continuous RSSI visualization.
 *
 * Spectrum scanning is performed incrementally so LVGL can process
 * touch input and screen updates between individual RF measurements.
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
     * @brief Input field used to configure the center frequency.
     */
    static lv_obj_t* frequencyInput;

    /**
     * @brief Dropdown used to select the frequency scan range.
     */
    static lv_obj_t* rangeDropdown;

    /**
     * @brief On-screen keyboard used for frequency input.
     */
    static lv_obj_t* keyboard;

    /**
     * @brief Container used for Sub-GHz signal visualization.
     */
    static lv_obj_t* signalContainer;

    /**
     * @brief Chart used to display RSSI measurements.
     */
    static lv_obj_t* signalChart;

    /**
     * @brief Chart series used to draw RSSI measurements.
     */
    static lv_chart_series_t* signalSeries;

    /**
     * @brief Timer used to schedule individual spectrum measurements.
     */
    static lv_timer_t* updateTimer;

    /**
     * @brief Context used by the back navigation button.
     */
    static BackContext backContext;

    /**
     * @brief Handles the scanner start button event.
     *
     * Reads the selected center frequency and scan range, starts the
     * incremental scanner and enables periodic scan updates.
     *
     * @param event Pointer to the LVGL event.
     */
    static void startClicked(lv_event_t* event);

    /**
     * @brief Handles the scanner stop button event.
     *
     * Stops the scanner and pauses periodic scan updates.
     *
     * @param event Pointer to the LVGL event.
     */
    static void stopClicked(lv_event_t* event);

    /**
     * @brief Handles periodic spectrum scan updates.
     *
     * Performs one RF measurement per callback and refreshes the graph
     * after a complete spectrum sweep has finished.
     *
     * @param timer Pointer to the LVGL timer.
     */
    static void updateTimerCallback(lv_timer_t* timer);

    /**
     * @brief Renders the latest complete CC1101 spectrum sweep.
     *
     * @param cc1101Scanner Reference to the CC1101Scanner feature.
     */
    static void renderResults(CC1101Scanner& cc1101Scanner);

    /**
     * @brief Converts the selected range option into MHz.
     *
     * @param selected Selected range dropdown index.
     *
     * @return Frequency range on either side of the center frequency.
     */
    static float getSelectedRange(uint16_t selected);

    /**
     * @brief Handles focus events for the center frequency input.
     *
     * Connects the on-screen keyboard to the frequency input.
     *
     * @param event Pointer to the LVGL event.
     */
    static void inputFocused(lv_event_t* event);

    /**
     * @brief Handles completion or cancellation of keyboard input.
     *
     * @param event Pointer to the LVGL event.
     */
    static void keyboardFinished(lv_event_t* event);

    /**
     * @brief Handles the Sub-GHz Scanner back button event.
     *
     * Stops scanning, pauses the update timer and navigates back
     * to the previous screen.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);
};