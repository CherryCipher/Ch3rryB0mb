#include "screenmainmenu.h"
#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"

/**
 * @brief Creates the main menu screen.
 *
 * Creates the main Ch3rryB0mb menu and adds the currently
 * available application navigation options.
 *
 * @param screenManager Reference to the application ScreenManager.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenMainMenu::create(ScreenManager& screenManager)
{
    // Screen background.
    lv_obj_t* screen = UIWidgets::createScreen();

    // Header.
    UIWidgets::addHeader(screen, 0, 0, "Ch3rryB0mb" );

    // Start button.
    lv_obj_t* apModeButton = UIWidgets::addButton(screen, 15, 75, "> AP Mode", 100, 45);

    // AP Mode navigation event.
    lv_obj_add_event_cb(
        apModeButton,
        apModeClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    return screen;
}

/**
 * @brief Handles the AP Mode menu button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the AP Mode screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::apModeClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>( lv_event_get_user_data(event) );

    if (screenManager == nullptr)
        return;

    screenManager->show(Screen::APMode);
}