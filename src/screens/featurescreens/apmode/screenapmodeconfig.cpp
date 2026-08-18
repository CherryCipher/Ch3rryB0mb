/**
 * @file screenapmodeconfig.cpp
 * @brief Implementation of the AP Mode configuration screen.
 *
 * This file creates the AP Mode configuration interface and handles
 * configuration screen events.
 */

#include "screenapmodeconfig.h"

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/apmode/apmode.h"

/**
 * @brief Save context containing references required by the SAVE callback.
 */
ScreenAPModeConfig::SaveContext ScreenAPModeConfig::saveContext = { nullptr, nullptr };

/**
 * @brief SSID input field used by the configuration screen.
 */
lv_obj_t* ScreenAPModeConfig::ssidInput = nullptr;

/**
 * @brief Password input field used by the configuration screen.
 */
lv_obj_t* ScreenAPModeConfig::passwordInput = nullptr;

/**
 * @brief Input field used to configure the WiFi channel.
 */
lv_obj_t* ScreenAPModeConfig::channelInput = nullptr;

/**
 * @brief Toggle used to configure whether the Access Point is hidden.
 */
lv_obj_t* ScreenAPModeConfig::hiddenToggle = nullptr;

/**
 * @brief Input field used to configure the maximum number of AP clients.
 */
lv_obj_t* ScreenAPModeConfig::maxClientsInput = nullptr;

/**
 * @brief On-screen touch keyboard used by the configuration screen.
 */
lv_obj_t* ScreenAPModeConfig::keyboard = nullptr;

/**
 * @brief Creates the AP Mode configuration screen.
 *
 * Creates a fixed header with a scrollable content area below it.
 * The current AP configuration is loaded into the input fields when
 * the screen is created.
 *
 * @param screenManager Reference to the application ScreenManager.
 * @param apMode Reference to the AP Mode feature.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenAPModeConfig::create(ScreenManager& screenManager, APMode& apMode)
{
    // Create screen and fixed header.
    lv_obj_t* screen = UIWidgets::createScreen();
    UIWidgets::addHeader(screen, 0, 0, "CONFIG AP");

    // Back button remains outside the scroll container.
    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &screenManager);

    // Scrollable configuration area below the 40px header.
    lv_obj_t* content = UIWidgets::createScrollContainer(screen, 0, 40, 240, 280);

    // Load current AP configuration.
    const WiFiAPConfig& config = apMode.getConfig();

    // SSID.
    UIWidgets::addText(content, 15, 15, "SSID", 210);
    ssidInput = UIWidgets::addInput(content, 15, 40, config.ssid.c_str(), 210);

    // Password.
    UIWidgets::addText(content, 15, 95, "PASSWORD", 210);
    passwordInput = UIWidgets::addInput(content, 15, 120, config.password.c_str(), 210, true);

    // WiFi channel.
    UIWidgets::addText(content, 15, 175, "CHANNEL (1-13)", 210);
    channelInput = UIWidgets::addInput(content, 15, 200, String(config.channel).c_str(), 210);

    // Hidden SSID.
    UIWidgets::addText(content, 15, 255, "HIDDEN", 210);
    hiddenToggle = UIWidgets::addToggle(content, 15, 280, config.hidden);

    // Maximum connected clients.
    UIWidgets::addText(content, 15, 335, "MAX CLIENTS (1-10)", 210);
    maxClientsInput = UIWidgets::addInput(content, 15, 360, String(config.maxClients).c_str(), 210);

    // Save button.
    lv_obj_t* saveButton = UIWidgets::addButton(content, 15, 415, "SAVE", 210, 40);

    // Configure SAVE callback context.
    saveContext.apMode = &apMode;
    saveContext.screenManager = &screenManager;
    lv_obj_add_event_cb(saveButton, saveClicked, LV_EVENT_CLICKED, &saveContext);

    //We add a space so the keyboard does not cover up the bottom content
    UIWidgets::addSpacer(content, 0, 470, 1, 150);

    // Create the on-screen keyboard.
    keyboard = UIWidgets::addKeyboard(screen);

    lv_obj_add_event_cb(keyboard, keyboardFinished, LV_EVENT_READY, nullptr);
    lv_obj_add_event_cb(keyboard, keyboardFinished, LV_EVENT_CANCEL, nullptr);

    // Connect input fields to the keyboard.
    lv_obj_add_event_cb(ssidInput, inputFocused, LV_EVENT_FOCUSED, nullptr);
    lv_obj_add_event_cb(passwordInput, inputFocused, LV_EVENT_FOCUSED, nullptr);
    lv_obj_add_event_cb(channelInput, inputFocused, LV_EVENT_FOCUSED, nullptr);
    lv_obj_add_event_cb(maxClientsInput, inputFocused, LV_EVENT_FOCUSED, nullptr);

    return screen;
}

/**
 * @brief Handles the BACK button event.
 *
 * Retrieves the ScreenManager instance from the LVGL event user data
 * and requests navigation to the previously active screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPModeConfig::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    screenManager->back();
}

/**
 * @brief Handles focus events for text input fields.
 *
 * Connects the on-screen keyboard to the focused textarea, shows
 * the keyboard and scrolls the selected input into view.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPModeConfig::inputFocused(lv_event_t* event)
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
 * @brief Handles completion of on-screen keyboard input.
 *
 * Disconnects the keyboard from the active textarea and hides it.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPModeConfig::keyboardFinished(lv_event_t* event)
{
    if (keyboard == nullptr)
        return;

    lv_keyboard_set_textarea(keyboard, nullptr);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief Handles the SAVE button event.
 *
 * Reads the edited configuration values, validates numeric settings,
 * stores the updated configuration in APMode and returns to the previous
 * screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPModeConfig::saveClicked(lv_event_t* event)
{
    SaveContext* context = static_cast<SaveContext*>(lv_event_get_user_data(event));

    if (context == nullptr ||
        context->apMode == nullptr ||
        context->screenManager == nullptr ||
        ssidInput == nullptr ||
        passwordInput == nullptr ||
        channelInput == nullptr ||
        hiddenToggle == nullptr ||
        maxClientsInput == nullptr)
    {
        return;
    }

    // Create an editable copy of the current configuration.
    WiFiAPConfig newConfig = context->apMode->getConfig();

    // Read text values.
    newConfig.ssid = lv_textarea_get_text(ssidInput);
    newConfig.password = lv_textarea_get_text(passwordInput);

    // Read numeric values.
    int channel = atoi(lv_textarea_get_text(channelInput));
    int maxClients = atoi(lv_textarea_get_text(maxClientsInput));

    // Validate channel range.
    if (channel < 1) channel = 1;
    if (channel > 13) channel = 13;

    // Validate maximum client range.
    if (maxClients < 1) maxClients = 1;
    if (maxClients > 10) maxClients = 10;

    // Store remaining configuration.
    newConfig.channel = channel;
    newConfig.hidden = lv_obj_has_state(hiddenToggle, LV_STATE_CHECKED);
    newConfig.maxClients = maxClients;

    // Store the new configuration and return to AP Mode.
    context->apMode->setConfig(newConfig);
    context->screenManager->back();
}