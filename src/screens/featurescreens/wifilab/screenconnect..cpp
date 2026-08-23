/**
 * @file screenconnect.cpp
 * @brief Implementation of the Wi-Fi connection screen.
 *
 * This file creates the Wi-Fi connection interface and handles
 * non-blocking Station connection attempts.
 */

#include "screenconnect.h"

#include <Arduino.h>
#include <WiFi.h>

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/wifilab/wifilab.h"

/**
 * @brief Context shared by Wi-Fi connection callbacks.
 */
ScreenConnect::ConnectContext ScreenConnect::connectContext;

/**
 * @brief Password input field.
 */
lv_obj_t* ScreenConnect::passwordInput = nullptr;

/**
 * @brief CONNECT button.
 */
lv_obj_t* ScreenConnect::connectButton = nullptr;

/**
 * @brief Connection status label.
 */
lv_obj_t* ScreenConnect::statusLabel = nullptr;

/**
 * @brief On-screen keyboard used for password entry.
 */
lv_obj_t* ScreenConnect::keyboard = nullptr;

/**
 * @brief Timer used to monitor an active Wi-Fi connection attempt.
 */
lv_timer_t* ScreenConnect::connectionTimer = nullptr;

/**
 * @brief Time at which the current connection attempt started.
 */
uint32_t ScreenConnect::connectionStartedAt = 0;

/**
 * @brief Indicates whether a connection attempt is currently active.
 */
bool ScreenConnect::connecting = false;

/**
 * @brief Creates the Wi-Fi connection screen.
 *
 * Displays the currently selected Wi-Fi network and creates a password
 * field when authentication is required.
 *
 * Open networks omit the password field and can be connected directly.
 *
 * @param screenManager Reference to the application ScreenManager.
 * @param wifiLab Reference to the WiFiLab feature.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenConnect::create(ScreenManager& screenManager, WiFiLab& wifiLab)
{
    lv_obj_t* screen = UIWidgets::createScreen();

    UIWidgets::addHeader(screen, 0, 0, "WIFI C0NNECT");

    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &screenManager);

    connectContext.screenManager = &screenManager;
    connectContext.wifiLab = &wifiLab;

    if (wifiLab.isConnected())
    {
        String ssid = wifiLab.getConnectedSSID();
        String ip = wifiLab.getLocalIP().toString();
        String signal = String(wifiLab.getRSSI()) + " dBm";

        UIWidgets::addText(screen, 15, 60, "CONNECTED", 210);
        UIWidgets::addText(screen, 15, 90, ssid.c_str(), 210);
        UIWidgets::addText(screen, 15, 120, ("IP: " + ip).c_str(), 210);
        UIWidgets::addText(screen, 15, 145, ("SIGNAL: " + signal).c_str(), 210);

        connectButton = UIWidgets::addButton(screen, 15, 190, "DISCONNECT", 210, 40);
        lv_obj_add_event_cb(connectButton, disconnectClicked, LV_EVENT_CLICKED, &connectContext);

        lv_obj_add_event_cb(screen, screenDeleted, LV_EVENT_DELETE, nullptr);

        return screen;
    }

    if (!wifiLab.hasSelectedNetwork())
    {
        UIWidgets::addText(screen, 15, 60, "No Wi-Fi network selected.", 210);
        return screen;
    }

    statusLabel = UIWidgets::addText(screen, 15, 270, "READY", 210);

    const WiFiNetwork& network = wifiLab.getSelectedNetwork();

    UIWidgets::addText(screen, 15, 55, "NETWORK", 210);

    String ssid = network.ssid;
    if (ssid.length() == 0) ssid = "< hidden >";

    UIWidgets::addText(screen, 15, 78, ssid.c_str(), 210);

    String networkInfo = "CH " + String(network.channel) + " | " + String(network.rssi) + " dBm";
    UIWidgets::addText(screen, 15, 102, networkInfo.c_str(), 210);

    bool openNetwork = network.encryption == WIFI_AUTH_OPEN;

    if (openNetwork)
    {
        UIWidgets::addText(screen, 15, 132, "SECURITY: OPEN", 210);
        UIWidgets::addText(screen, 15, 157, "No password required.", 210);

        connectButton = UIWidgets::addButton(screen, 15, 195, "CONNECT", 210, 40);
    }
    else
    {
        UIWidgets::addText(screen, 15, 132, "PASSWORD", 210);

        passwordInput = UIWidgets::addInput(screen, 15, 157, "", 210, true);
        lv_obj_add_event_cb(passwordInput, inputFocused, LV_EVENT_FOCUSED, nullptr);

        connectButton = UIWidgets::addButton(screen, 15, 215, "CONNECT", 210, 40);

        keyboard = UIWidgets::addKeyboard(screen);
        lv_obj_add_event_cb(keyboard, keyboardFinished, LV_EVENT_READY, nullptr);
        lv_obj_add_event_cb(keyboard, keyboardFinished, LV_EVENT_CANCEL, nullptr);
    }

    lv_obj_add_event_cb(connectButton, connectClicked, LV_EVENT_CLICKED, &connectContext);

    connectionTimer = lv_timer_create(connectionTimerCallback, 250, &connectContext);
    lv_obj_add_event_cb(screen, screenDeleted, LV_EVENT_DELETE, nullptr);

    return screen;
}

/**
 * @brief Handles the CONNECT button event.
 *
 * Reads the entered password when required and starts a non-blocking
 * Wi-Fi connection through WiFiLab.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenConnect::connectClicked(lv_event_t* event)
{
    ConnectContext* context = static_cast<ConnectContext*>(lv_event_get_user_data(event));

    if (context == nullptr || context->wifiLab == nullptr || connecting)
        return;

    const WiFiNetwork& network = context->wifiLab->getSelectedNetwork();

    String password = "";

    if (network.encryption != WIFI_AUTH_OPEN)
    {
        if (passwordInput == nullptr)
            return;

        password = lv_textarea_get_text(passwordInput);
    }

    if (keyboard != nullptr)
    {
        lv_keyboard_set_textarea(keyboard, nullptr);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }

    if (!context->wifiLab->connect(password))
    {
        setStatus("FAILED TO START");
        return;
    }

    connecting = true;
    connectionStartedAt = millis();

    setStatus("CONNECTING...");

    lv_obj_t* label = lv_obj_get_child(connectButton, 0);
    if (label != nullptr) lv_label_set_text(label, "CONNECTING");

    lv_obj_add_state(connectButton, LV_STATE_DISABLED);
}

/**
 * @brief Handles the BACK button event.
 *
 * Returns to the previous application screen without changing the
 * active Wi-Fi Station connection.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenConnect::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    screenManager->back();
}

/**
 * @brief Handles the DISCONNECT button event.
 *
 * Disconnects the active Wi-Fi Station connection through WiFiLab
 * and returns to the Wi-Fi Lab scan screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenConnect::disconnectClicked(lv_event_t* event)
{
    ConnectContext* context = static_cast<ConnectContext*>(lv_event_get_user_data(event));

    if (context == nullptr || context->screenManager == nullptr || context->wifiLab == nullptr)
        return;

    context->wifiLab->disconnect();
    context->screenManager->show(Screen::WifiLab);
}

/**
 * @brief Handles focus on the password input.
 *
 * Connects the on-screen keyboard to the focused textarea, displays
 * the keyboard and scrolls the input into view.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenConnect::inputFocused(lv_event_t* event)
{
    if (keyboard == nullptr)
        return;

    lv_obj_t* input = static_cast<lv_obj_t*>(lv_event_get_target(event));

    if (input == nullptr)
        return;

    lv_keyboard_set_textarea(keyboard, input);
    lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_scroll_to_view(input, LV_ANIM_ON);
}

/**
 * @brief Handles completion of keyboard input.
 *
 * Disconnects the keyboard from the active textarea and hides it.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenConnect::keyboardFinished(lv_event_t* event)
{
    if (keyboard == nullptr)
        return;

    lv_keyboard_set_textarea(keyboard, nullptr);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief Monitors the current Wi-Fi connection attempt.
 *
 * Checks the WiFiLab connection state without blocking the UI.
 * Successful connections display the assigned local IP address.
 *
 * Connection attempts are cancelled after CONNECTION_TIMEOUT_MS.
 *
 * @param timer Pointer to the LVGL timer.
 */
void ScreenConnect::connectionTimerCallback(lv_timer_t* timer)
{
    ConnectContext* context = static_cast<ConnectContext*>(lv_timer_get_user_data(timer));

    if (context == nullptr || context->wifiLab == nullptr || !connecting)
        return;

    if (context->wifiLab->isConnected())
    {
        connecting = false;

        String status = "CONNECTED\nIP: " + context->wifiLab->getLocalIP().toString();
        setStatus(status.c_str());

        lv_obj_t* label = lv_obj_get_child(connectButton, 0);
        if (label != nullptr) lv_label_set_text(label, "CONNECTED");

        return;
    }

    if (millis() - connectionStartedAt < CONNECTION_TIMEOUT_MS)
        return;

    context->wifiLab->disconnect();

    connecting = false;

    setStatus("CONNECTION FAILED");
    resetConnectButton();
}

/**
 * @brief Updates the connection status displayed on screen.
 *
 * @param text Status text to display.
 */
void ScreenConnect::setStatus(const char* text)
{
    if (statusLabel != nullptr)
        lv_label_set_text(statusLabel, text);
}

/**
 * @brief Restores the CONNECT button after a failed connection attempt.
 */
void ScreenConnect::resetConnectButton()
{
    if (connectButton == nullptr)
        return;

    lv_obj_clear_state(connectButton, LV_STATE_DISABLED);

    lv_obj_t* label = lv_obj_get_child(connectButton, 0);
    if (label != nullptr) lv_label_set_text(label, "CONNECT");
}

/**
 * @brief Cleans up screen-specific LVGL references.
 *
 * Deletes the connection monitor timer and clears stored UI references.
 * An established Wi-Fi connection is intentionally not stopped.
 *
 * @param event Pointer to the LVGL delete event.
 */
void ScreenConnect::screenDeleted(lv_event_t* event)
{
    if (connectionTimer != nullptr)
    {
        lv_timer_delete(connectionTimer);
        connectionTimer = nullptr;
    }

    passwordInput = nullptr;
    connectButton = nullptr;
    statusLabel = nullptr;
    keyboard = nullptr;

    connecting = false;

    connectContext.screenManager = nullptr;
    connectContext.wifiLab = nullptr;
}