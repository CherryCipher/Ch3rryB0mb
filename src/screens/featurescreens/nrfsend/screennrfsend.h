/**
 * @file screennrfsend.h
 * @brief NRF24 message transmission screen for Ch3rryB0mb.
 *
 * Provides controls for selecting an NRF24 channel, entering a short
 * text message and transmitting it using acknowledged communication.
 */

#pragma once

#include <lvgl.h>

class ScreenManager;
class NRFSend;

/**
 * @class ScreenNRFSend
 * @brief Provides the NRF24 message transmission interface.
 *
 * ScreenNRFSend allows the user to select a communication channel,
 * enter a text message and transmit it using the NRFSend feature.
 *
 * Transmission results are displayed directly on the screen, including
 * successful acknowledgements and communication errors.
 */
class ScreenNRFSend
{
public:
    /**
     * @brief Creates the NRF Send screen.
     *
     * Creates the standard Ch3rryB0mb header, scrollable content area,
     * channel input, message input, SEND button and status display.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param nrfSend Reference to the NRFSend feature.
     *
     * @return Pointer to the created LVGL screen.
     */
    static lv_obj_t* create(ScreenManager& screenManager, NRFSend& nrfSend);

private:
    /**
     * @brief Lowest channel allowed for transmission from the UI.
     */
    static constexpr uint8_t MIN_CHANNEL = 2;

    /**
     * @brief Highest channel allowed for transmission from the UI.
     */
    static constexpr uint8_t MAX_CHANNEL = 80;

    /**
     * @struct Context
     * @brief Holds references required by screen callbacks.
     */
    struct Context
    {
        ScreenManager* screenManager = nullptr;
        NRFSend* nrfSend = nullptr;
    };

    /**
     * @brief Callback context used by the screen.
     */
    static Context context;

    /**
     * @brief NRF24 channel input field.
     */
    static lv_obj_t* channelInput;

    /**
     * @brief Message input field.
     */
    static lv_obj_t* messageInput;

    /**
     * @brief Status label showing transmission results.
     */
    static lv_obj_t* statusLabel;

    /**
     * @brief On-screen keyboard used by input fields.
     */
    static lv_obj_t* keyboard;

    /**
     * @brief Handles focus events for screen input fields.
     *
     * Connects the keyboard to the selected textarea and selects
     * an appropriate keyboard mode.
     *
     * @param event Pointer to the LVGL event.
     */
    static void inputFocused(lv_event_t* event);

    /**
     * @brief Handles keyboard READY and CANCEL events.
     *
     * Disconnects the keyboard and hides it.
     *
     * @param event Pointer to the LVGL event.
     */
    static void keyboardFinished(lv_event_t* event);

    /**
     * @brief Handles the SEND button event.
     *
     * Validates the selected channel, reads the message and requests
     * acknowledged transmission through NRFSend.
     *
     * @param event Pointer to the LVGL event.
     */
    static void sendClicked(lv_event_t* event);

    /**
     * @brief Handles the BACK button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);

    /**
     * @brief Updates the status display.
     *
     * @param text Text to display.
     */
    static void showStatus(const char* text);

    /**
     * @brief Clears static screen references when the screen is deleted.
     *
     * @param event Pointer to the LVGL event.
     */
    static void screenDeleted(lv_event_t* event);
};