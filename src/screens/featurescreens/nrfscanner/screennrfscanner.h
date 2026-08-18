/**
 * @file screennrfscanner.h
 * @brief NRF Scanner screen interface.
 *
 * This file defines the NRF Scanner screen used to configure
 * and visualize NRF24 radio scanning functionality.
 */

#pragma once

#include <Arduino.h>
#include <lvgl.h>

class ScreenManager;
class NRFScanner;

/**
 * @class ScreenNrfScanner
 * @brief Provides the Ch3rryB0mb NRF Scanner screen.
 *
 * The screen provides scan mode and channel controls, start and stop
 * controls and live RF activity visualization.
 *
 * RF scanning is delegated to the NRFScanner feature.
 */
class ScreenNrfScanner
{
public:
    /**
     * @brief Creates the NRF Scanner screen.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param nrfScanner Reference to the NRFScanner feature.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(ScreenManager& screenManager, NRFScanner& nrfScanner);

private:
    /**
     * @enum ScanMode
     * @brief Scan modes displayed by the mode dropdown.
     */
    enum class ScanMode : uint8_t
    {
        FullSpectrum = 0,
        NrfChannel,
        WifiBand
    };

    /**
     * @struct BackContext
     * @brief Holds references required by the back navigation event.
     */
    struct BackContext
    {
        ScreenManager* screenManager;
        NRFScanner* nrfScanner;
    };

    /**
     * @brief Dropdown used to select the scanner operating mode.
     */
    static lv_obj_t* modeDropdown;

    /**
     * @brief Dropdown used to select the channel or frequency range.
     */
    static lv_obj_t* channelDropdown;

    /**
     * @brief Container used for RF signal visualization.
     */
    static lv_obj_t* signalContainer;

    /**
     * @brief Chart used to display RF activity.
     */
    static lv_obj_t* signalChart;

    /**
     * @brief Chart series used to draw RF activity.
     */
    static lv_chart_series_t* signalSeries;

    /**
     * @brief Chart series used to display peak RF activity.
     */
    static lv_chart_series_t* peakSeries;

    /**
     * @brief Timer used to update scanning and visualization.
     */
    static lv_timer_t* updateTimer;

    /**
     * @brief Context used by the back button.
     */
    static BackContext backContext;

    /**
     * @brief Buffer containing generated NRF channel options.
     */
    static char nrfChannelOptions[1536];

    /**
     * @brief Handles changes to the scanner mode dropdown.
     *
     * @param event Pointer to the LVGL event.
     */
    static void modeChanged(lv_event_t* event);

    /**
     * @brief Handles changes to the channel dropdown.
     *
     * @param event Pointer to the LVGL event.
     */
    static void channelChanged(lv_event_t* event);

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
     * @brief Handles the peak clear button event.
     *
     * Clears all stored spectrum peak values.
     *
     * @param event Pointer to the LVGL event.
     */
    static void clearClicked(lv_event_t* event);

    /**
     * @brief Periodically updates the scanner and chart.
     *
     * @param timer Pointer to the LVGL timer.
     */
    static void updateScanner(lv_timer_t* timer);

    /**
     * @brief Updates the channel dropdown for the selected scan mode.
     *
     * @param mode Selected scanner operating mode.
     */
    static void updateChannelDropdown(ScanMode mode);

    /**
     * @brief Generates the NRF channel dropdown options.
     */
    static void buildNrfChannelOptions();

    /**
     * @brief Renders the latest scanner results.
     *
     * @param nrfScanner Reference to the NRFScanner feature.
     */
    static void renderResults(NRFScanner& nrfScanner);

    /**
     * @brief Handles the NRF Scanner back button event.
     *
     * Stops scanning, removes the update timer and navigates back.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);
};