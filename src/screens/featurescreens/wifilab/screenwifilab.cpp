#include "screenwifilab.h"
#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"

/**
 * @brief Creates the wifilab screen.
 *
 * The ScreenWifiLab creates the wifi lab screen for scanning and inspecting AP's
 * It also handles user interface events that originate from this screen.
 *
 * @param screenManager Reference to the application ScreenManager.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenWifiLab::create(ScreenManager& screenManager)
{
    // Screen background.
    lv_obj_t* screen = UIWidgets::createScreen();

    // Header.
    UIWidgets::addHeader(screen, 0, 0, "WIFILAB" );

    // Back button.
    lv_obj_t* backButton = UIWidgets::addButton( screen, 150, 5, "< BACK", 80, 30 );

    lv_obj_add_event_cb(
        backButton,
        backClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    return screen;
}

/**
 * @brief Handles the WifiLab back button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenWifiLab::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>( lv_event_get_user_data(event) );

    if (screenManager == nullptr)
        return;

    screenManager->back();
}