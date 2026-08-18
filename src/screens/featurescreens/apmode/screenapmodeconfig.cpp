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
 * @brief SSID input field used by the configuration screen.
 */
lv_obj_t* ScreenAPModeConfig::ssidInput = nullptr;

/**
 * @brief Password input field used by the configuration screen.
 */
lv_obj_t* ScreenAPModeConfig::passwordInput = nullptr;

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
    lv_obj_t* saveButton = UIWidgets::addButton(content, 15, 320, "SAVE & APPLY", 210, 40);

    lv_obj_add_event_cb(
        saveButton,
        saveClicked,
        LV_EVENT_CLICKED,
        &apMode
    );

    return screen;
}

/**
 * @brief Handles the SAVE button event.
 *
 * Retrieves the APMode instance associated with the screen.
 * Reading and applying the edited configuration will be implemented
 * when APMode configuration update support is added.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPModeConfig::saveClicked(lv_event_t* event)
{
    APMode* apMode =
        static_cast<APMode*>(
            lv_event_get_user_data(event)
        );

    if (apMode == nullptr)
    {
        return;
    }

    //
    // Configuration save/apply functionality will be added here.
    //
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