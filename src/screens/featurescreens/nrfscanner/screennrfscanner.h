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

/**
 * @class ScreenNrfScanner
 * @brief Provides the Ch3rryB0mb NRF Scanner screen.
 *
 * The ScreenNrfScanner provides controls for selecting scan modes
 * and channels and contains a live signal visualization area.
 */
class ScreenNrfScanner
{
public:
    /**
     * @brief Creates the NRF Scanner screen.
     *
     * Creates the NRF Scanner interface with mode and channel
     * selectors, signal visualization and navigation controls.
     *
     * @param screenManager Reference to the application ScreenManager.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(ScreenManager& screenManager);

private:
    /**
     * @brief Available NRF Scanner operating modes.
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
     * @brief Container used for live RF signal visualization.
     */
    static lv_obj_t* signalContainer;

    /**
     * @brief Chart used to display live RF activity.
     */
    static lv_obj_t* signalChart;

    /**
     * @brief Buffer containing the generated NRF channel options.
     */
    static char nrfChannelOptions[1536];

    /**
     * @brief Handles changes to the scanner mode dropdown.
     *
     * Updates the channel dropdown based on the selected scan mode.
     *
     * @param event Pointer to the LVGL event.
     */
    static void modeChanged(lv_event_t* event);

    /**
     * @brief Updates the channel dropdown for the selected scan mode.
     *
     * @param mode Selected scanner operating mode.
     */
    static void updateChannelDropdown(ScanMode mode);

    /**
     * @brief Generates the NRF channel dropdown options.
     *
     * Generates selectable NRF channels from channel 0 through 125.
     */
    static void buildNrfChannelOptions();

    /**
     * @brief Handles the NRF Scanner back button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);
};