/**
 * @file screennrfscanner.h
 * @brief NRF Scanner screen interface.
 *
 * This file defines the NRF Scanner screen used to configure
 * and display NRF24 radio scanning functionality.
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
 * The ScreenNrfScanner provides controls for selecting scan modes
 * and channels and displays live RF activity returned by the
 * NRFScanner feature.
 *
 * The screen is responsible only for presentation and user interaction.
 * RF scanning itself is delegated to the NRFScanner feature.
 */
class ScreenNrfScanner
{
public:
    /**
     * @brief Creates the NRF Scanner screen.
     *
     * Creates scanner controls, signal visualization and navigation
     * controls and connects the screen to the NRFScanner feature.
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
     * @brief Scan modes displayed by the scanner mode dropdown.
     *
     * The numeric values correspond directly to NRFScanner::ScanMode.
     */
    enum class ScanMode : uint8_t
    {
        FullSpectrum = 0,
        NrfChannel,
        WifiBand
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
     * @brief Timer used to update scanning and signal visualization.
     */
    static lv_timer_t* updateTimer;

    /**
     * @brief Buffer containing generated NRF channel options.
     */
    static char nrfChannelOptions[1536];

    /**
     * @brief Handles changes to the scanner mode dropdown.
     *
     * Updates the NRFScanner feature and rebuilds the channel selector.
     *
     * @param event Pointer to the LVGL event.
     */
    static void modeChanged(lv_event_t* event);

    /**
     * @brief Handles changes to the channel dropdown.
     *
     * Updates either the selected NRF24 channel or Wi-Fi channel
     * depending on the current scanner mode.
     *
     * @param event Pointer to the LVGL event.
     */
    static void channelChanged(lv_event_t* event);

    /**
     * @brief Handles the scan button event.
     *
     * Starts or stops continuous RF scanning.
     *
     * @param event Pointer to the LVGL event.
     */
    static void scanClicked(lv_event_t* event);

    /**
     * @brief Periodically updates scanning and visualization.
     *
     * Calls NRFScanner::update() and renders the latest scan results.
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
     * @brief Generates NRF channel dropdown options.
     *
     * Generates selectable NRF channels from 0 through 125.
     */
    static void buildNrfChannelOptions();

    /**
     * @brief Renders the latest scanner results.
     *
     * Updates the chart based on the active scan mode.
     *
     * @param nrfScanner Reference to the NRFScanner feature.
     */
    static void renderResults(NRFScanner& nrfScanner);

    /**
     * @brief Handles the NRF Scanner back button event.
     *
     * Stops scanning, removes the update timer and navigates
     * to the previous screen.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);
};