/**
 * @file screennrfscanner.cpp
 * @brief Implementation of the NRF Scanner screen.
 *
 * This file creates the NRF Scanner interface and handles
 * screen navigation events.
 */

#include "screennrfscanner.h"
#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"

/**
 * @brief Creates the NRF Scanner screen.
 *
 * Creates the NRF Scanner interface with a header
 * and BACK navigation control.
 *
 * @param screenManager Reference to the application ScreenManager.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenNrfScanner::create(ScreenManager& screenManager)
{
    // Screen background.
    lv_obj_t* screen = UIWidgets::createScreen();

    // Header.
    UIWidgets::addHeader(screen, 0, 0, "NRF SC4NNER");

    // Back button.
    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);

    lv_obj_add_event_cb(
        backButton,
        backClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    return screen;
}

/**
 * @brief Handles the NRF Scanner back button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNrfScanner::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    screenManager->back();
}