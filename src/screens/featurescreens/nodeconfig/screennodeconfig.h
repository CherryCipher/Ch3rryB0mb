/**
 * @file screennodeconfig.h
 * @brief Declaration of the Ch3rryN0de configuration screen.
 *
 * Provides radio and mode configuration for NRF24 and CC1101
 * Ch3rryN0de sessions.
 */

#pragma once

#include <lvgl.h>
#include "app/features/configurenode/configurenode.h"

class ScreenManager;

/**
 * @class ScreenNodeConfig
 * @brief Provides configuration controls for a selected Ch3rryN0de.
 *
 * The visible settings automatically change based on the selected
 * radio and operating mode.
 *
 * NRF24 uses a channel with a calculated frequency. CC1101 uses a
 * configurable frequency. Beacon mode additionally exposes a
 * transmission interval.
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
     * @brief Lowest NRF24 transmission channel exposed by the UI.
     */
    static constexpr uint8_t MIN_NRF_CHANNEL = 2;

    /**
     * @brief Highest NRF24 transmission channel exposed by the UI.
     */
    static constexpr uint8_t MAX_NRF_CHANNEL = 80;

    /**
     * @brief Minimum beacon transmission interval in milliseconds.
     */
    static constexpr uint16_t MIN_INTERVAL = 100;

    /**
     * @brief Maximum beacon transmission interval in milliseconds.
     */
    static constexpr uint16_t MAX_INTERVAL = 60000;

    /**
     * @struct Context
     * @brief Callback context for the node configuration screen.
     */
    struct Context
    {
        ScreenManager* screenManager = nullptr;
        ConfigureNode* configureNode = nullptr;
        lv_obj_t* screen = nullptr;
    };

    static Context context;

    static lv_obj_t* radioButton;
    static lv_obj_t* modeButton;

    static lv_obj_t* channelLabel;
    static lv_obj_t* channelInput;

    static lv_obj_t* frequencyLabel;
    static lv_obj_t* frequencyValueLabel;
    static lv_obj_t* frequencyInput;

    static lv_obj_t* intervalLabel;
    static lv_obj_t* intervalInput;

    static lv_obj_t* startButton;
    static lv_obj_t* keyboard;

    /**
     * @brief Updates the visible configuration controls.
     */
    static void updateControls();

    /**
     * @brief Updates the calculated NRF24 frequency display.
     */
    static void updateNRFFrequency();

    /**
     * @brief Stores all configuration fields relevant to the active session.
     *
     * @return true when all relevant values are valid.
     * @return false otherwise.
     */
    static bool storeInputs();

    /**
     * @brief Stores the selected NRF24 channel.
     *
     * @return true when the channel is valid.
     */
    static bool storeChannel();

    /**
     * @brief Stores the selected CC1101 frequency.
     *
     * @return true when the frequency is valid.
     */
    static bool storeFrequency();

    /**
     * @brief Stores the selected beacon interval.
     *
     * @return true when the interval is valid.
     */
    static bool storeInterval();

    /**
     * @brief Returns whether a frequency is supported by the CC1101.
     *
     * @param frequency Frequency in MHz.
     *
     * @return true when the frequency is inside a supported range.
     */
    static bool isValidCC1101Frequency(float frequency);

    /**
     * @brief Handles focus on a configuration input.
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
     * @brief Updates the NRF24 frequency while the channel is edited.
     *
     * @param event Pointer to the LVGL event.
     */
    static void channelChanged(lv_event_t* event);

    /**
     * @brief Replaces the configuration controls with a success message.
     */
    static void showSuccess();

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
     * @brief Handles the START button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void startClicked(lv_event_t* event);

    /**
     * @brief Handles the CLOSE button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void closeClicked(lv_event_t* event);

    /**
     * @brief Handles the BACK button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);

    /**
     * @brief Clears static screen references when the screen is deleted.
     *
     * @param event Pointer to the LVGL event.
     */
    static void screenDeleted(lv_event_t* event);
};