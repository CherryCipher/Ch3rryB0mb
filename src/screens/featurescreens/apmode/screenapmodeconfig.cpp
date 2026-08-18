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
* @brief Savecontext holds the references for the save feature
*/
ScreenAPModeConfig::SaveContext ScreenAPModeConfig::saveContext = {
    nullptr,
    nullptr
};

/**
* @brief SSID input field used by the configuration screen.
*/
lv_obj_t* ScreenAPModeConfig::ssidInput = nullptr;

/**
* @brief Password input field used by the configuration screen.
*/
lv_obj_t* ScreenAPModeConfig::passwordInput = nullptr;

/**
* @brief Onscreen touch Keyboard used by the configuration screen.
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
lv_obj_t* ScreenAPModeConfig::create(
    ScreenManager& screenManager,
    APMode& apMode
)
{
    // Create screen.
    lv_obj_t* screen = UIWidgets::createScreen();

    // Header.
    UIWidgets::addHeader(screen, 0, 0, "CONFIG AP");

    // Back button remains outside the scroll container so it stays
    // visible while the configuration fields are scrolled.
    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);

    lv_obj_add_event_cb(
        backButton,
        backClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    // Scrollable configuration area below the fixed header. -40px for header
    lv_obj_t* content = UIWidgets::createScrollContainer(screen, 0, 40, 240, 280);

    // Load current AP configuration.
    const WiFiAPConfig& config = apMode.getConfig();

    // SSID.
    UIWidgets::addText(content, 15, 15, "SSID", 210);

    //adeed to content not to screen to add it to the scrollarea
    ssidInput = UIWidgets::addInput(content, 15, 40, config.ssid.c_str(), 210);


    // Password.
    //adeed to content not to screen to add it to the scrollarea
    UIWidgets::addText(content, 15, 95, "PASSWORD", 210);

    passwordInput = UIWidgets::addInput(content, 15, 120, config.password.c_str(), 210, true);

    // Future configuration fields.
    //adeed to content not to screen to add it to the scrollarea
    UIWidgets::addText(content, 15, 175, "CHANNEL", 210);

    UIWidgets::addText(content, 15, 220, "HIDDEN", 210);

    UIWidgets::addText(content, 15, 265, "MAX CLIENTS", 210);

    // Save button.
    lv_obj_t* saveButton = UIWidgets::addButton(content, 15, 320, "SAVE", 210, 40);

    //create savecontext, so we can save and Back
    saveContext.apMode = &apMode;
    saveContext.screenManager = &screenManager;

    lv_obj_add_event_cb(
        saveButton,
        saveClicked,
        LV_EVENT_CLICKED,
        &saveContext
    );

    //Create the onscreen keyboard
    keyboard = UIWidgets::addKeyboard(screen);

    lv_obj_add_flag(
        keyboard,
        LV_OBJ_FLAG_HIDDEN
    );

    lv_obj_add_event_cb(
        keyboard,
        keyboardFinished,
        LV_EVENT_READY,
        nullptr
    );

    lv_obj_add_event_cb(
        keyboard,
        keyboardFinished,
        LV_EVENT_CANCEL,
        nullptr
    );

    //SSID focus event
    lv_obj_add_event_cb(
        ssidInput,
        inputFocused,
        LV_EVENT_FOCUSED,
        nullptr
    );

    //password focus event
    lv_obj_add_event_cb(
        passwordInput,
        inputFocused,
        LV_EVENT_FOCUSED,
        nullptr
    );

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
    ScreenManager* screenManager =
        static_cast<ScreenManager*>(
            lv_event_get_user_data(event)
        );

    if (screenManager == nullptr)
    {
        return;
    }

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
    {
        return;
    }

    lv_obj_t* input =
    static_cast<lv_obj_t*>(
        lv_event_get_target(event)
    );

    lv_keyboard_set_textarea(
        keyboard,
        input
    );

    lv_obj_clear_flag(
        keyboard,
        LV_OBJ_FLAG_HIDDEN
    );

    lv_obj_scroll_to_view(
        input,
        LV_ANIM_ON
    );
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
    {
        return;
    }

    lv_keyboard_set_textarea(
        keyboard,
        nullptr
    );

    lv_obj_add_flag(
        keyboard,
        LV_OBJ_FLAG_HIDDEN
    );
}

/**
 * @brief Handles the SAVE button event.
 *
 * Reads the edited configuration values, stores the updated
 * configuration in APMode and returns to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPModeConfig::saveClicked(lv_event_t* event)
{
    SaveContext* context =
        static_cast<SaveContext*>(
            lv_event_get_user_data(event)
        );

    if (context == nullptr ||
        context->apMode == nullptr ||
        context->screenManager == nullptr ||
        ssidInput == nullptr ||
        passwordInput == nullptr)
    {
        return;
    }

    // Create an editable copy of the current configuration.
    WiFiAPConfig newConfig =
        context->apMode->getConfig();

    // Read the edited values.
    newConfig.ssid =
        lv_textarea_get_text(ssidInput);

    newConfig.password =
        lv_textarea_get_text(passwordInput);

    // Store the new configuration.
    context->apMode->setConfig(newConfig);

    // Return to AP Mode.
    context->screenManager->back();
}