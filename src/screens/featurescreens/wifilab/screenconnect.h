/**
 * @file screenconnect.h
 * @brief Wi-Fi connection screen interface.
 *
 * This file defines the screen used to connect Ch3rryB0mb
 * to a Wi-Fi network selected in WiFi Lab.
 */

#pragma once

#include <lvgl.h>

class ScreenManager;
class WiFiLab;

/**
 * @class ScreenConnect
 * @brief Provides the Wi-Fi connection screen.
 *
 * ScreenConnect displays the network selected in WiFi Lab and allows
 * the user to enter a password and start a Station connection.
 *
 * Connection handling is delegated to WiFiLab. The screen periodically
 * checks the connection state without blocking the LVGL interface.
 */
class ScreenConnect
{
public:
    /**
     * @brief Creates the Wi-Fi connection screen.
     *
     * Displays information about the selected network, creates the
     * password input when required and provides the CONNECT control.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param wifiLab Reference to the WiFiLab feature.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(ScreenManager& screenManager, WiFiLab& wifiLab);

private:
    /**
     * @struct ConnectContext
     * @brief Context required by Wi-Fi connection callbacks.
     */
    struct ConnectContext
    {
        ScreenManager* screenManager = nullptr;
        WiFiLab* wifiLab = nullptr;
    };

    /**
     * @brief Context shared by connection callbacks.
     */
    static ConnectContext connectContext;

    /**
     * @brief Password input field.
     */
    static lv_obj_t* passwordInput;

    /**
     * @brief CONNECT button.
     */
    static lv_obj_t* connectButton;

    /**
     * @brief Connection status label.
     */
    static lv_obj_t* statusLabel;

    /**
     * @brief On-screen keyboard used for password entry.
     */
    static lv_obj_t* keyboard;

    /**
     * @brief Timer used to monitor an active Wi-Fi connection attempt.
     */
    static lv_timer_t* connectionTimer;

    /**
     * @brief Time at which the current connection attempt started.
     */
    static uint32_t connectionStartedAt;

    /**
     * @brief Indicates whether a connection attempt is currently active.
     */
    static bool connecting;

    /**
     * @brief Maximum time allowed for a Wi-Fi connection attempt.
     */
    static constexpr uint32_t CONNECTION_TIMEOUT_MS = 15000;

    /**
     * @brief Handles the CONNECT button event.
     *
     * Reads the password field and starts a non-blocking connection
     * through the WiFiLab feature.
     *
     * @param event Pointer to the LVGL event.
     */
    static void connectClicked(lv_event_t* event);

    /**
     * @brief Handles the BACK button event.
     *
     * Returns to the previous screen through ScreenManager.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);

    /**
     * @brief Handles focus on the password input.
     *
     * Connects the keyboard to the textarea and displays it.
     *
     * @param event Pointer to the LVGL event.
     */
    static void inputFocused(lv_event_t* event);

    /**
     * @brief Handles keyboard READY and CANCEL events.
     *
     * Disconnects the keyboard from the textarea and hides it.
     *
     * @param event Pointer to the LVGL event.
     */
    static void keyboardFinished(lv_event_t* event);

    /**
     * @brief Monitors the current Wi-Fi connection attempt.
     *
     * Updates the screen when the Station interface connects or when
     * the connection attempt exceeds the configured timeout.
     *
     * @param timer Pointer to the LVGL timer.
     */
    static void connectionTimerCallback(lv_timer_t* timer);

    /**
     * @brief Updates the connection status displayed on screen.
     *
     * @param text Status text to display.
     */
    static void setStatus(const char* text);

    /**
     * @brief Restores the CONNECT button after a failed attempt.
     */
    static void resetConnectButton();

    /**
     * @brief Cleans up screen-specific LVGL references.
     *
     * The Wi-Fi connection itself is intentionally left active when
     * leaving this screen.
     *
     * @param event Pointer to the LVGL delete event.
     */
    static void screenDeleted(lv_event_t* event);
};