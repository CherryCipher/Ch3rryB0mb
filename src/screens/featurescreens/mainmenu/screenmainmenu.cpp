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
 * @param features Reference to features.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenMainMenu::create(ScreenManager& screenManager, Features& features)
{
    // Screen background.
    lv_obj_t* screen = UIWidgets::createScreen();

    // Header.
    UIWidgets::addHeader(screen, 0, 0, "CH3RRYB0MB", features.isWiFiConnected() ? LV_SYMBOL_WIFI : nullptr);

    // Ap Mode Button
    lv_obj_t* apModeButton = UIWidgets::addButton(screen, 15, 60, "> AP Mode", 100, 45);

    // AP Mode navigation event.
    lv_obj_add_event_cb(
        apModeButton,
        apModeClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    // WifiLab Button
    lv_obj_t* wifiLabButton = UIWidgets::addButton(screen, 125, 60, "> WifiL4b", 100, 45);

    // AP Mode navigation event.
    lv_obj_add_event_cb(
        wifiLabButton,
        wifiLabClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    // BLE scan Button
    lv_obj_t* bleEXPButton = UIWidgets::addButton(screen, 15, 115, "> BLE Exp", 100, 45);

    // AP Mode navigation event.
    lv_obj_add_event_cb(
        bleEXPButton,
        bleExplorerClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    // Packet Viewer Button
    lv_obj_t* packetViewerButton = UIWidgets::addButton(screen, 125, 115, "> P4ckets", 100, 45);
    lv_obj_add_event_cb(packetViewerButton, packetViewerClicked, LV_EVENT_CLICKED, &screenManager);

    //Modules Label
    UIWidgets::addText(screen, 15, 170, "MODULES:", 210); 

    //Since we might not have an nrf module available check if it is
    if(features.nrfScanner.isAvailable())
    {
        // NRF Scanner Button
        lv_obj_t* nrfScannerButton = UIWidgets::addButton(screen, 15, 190, "> NRF Sc4n", 100, 45);

        
        // AP Mode navigation event.
        lv_obj_add_event_cb(
            nrfScannerButton,
            nrfScannerClicked,
            LV_EVENT_CLICKED,
            &screenManager
        );
    }

    //Since we might not have an cc1101 module available check if it is
    if(features.cc1101Scanner.isAvailable())
    {
        // NRF Scanner Button
        lv_obj_t* cc1101ScannerButton = UIWidgets::addButton(screen, 125, 190, "> Sub Sc4n", 100, 45);

        
        // AP Mode navigation event.
        lv_obj_add_event_cb(
            cc1101ScannerButton,
            cc1101ScannerClicked,
            LV_EVENT_CLICKED,
            &screenManager
        );
    }

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

/**
 * @brief Handles the WifiLab menu button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the WifiLab screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::wifiLabClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>( lv_event_get_user_data(event) );

    if (screenManager == nullptr)
        return;

    screenManager->show(Screen::WifiLab);
}

/**
 * @brief Handles the Packet Viewer menu button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the Packet Viewer screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::packetViewerClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    screenManager->show(Screen::PacketViewer);
}

/**
 * @brief Handles the NRF Scanner menu button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the NRF Scanner screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::nrfScannerClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>( lv_event_get_user_data(event) );

    if (screenManager == nullptr)
        return;

    screenManager->show(Screen::NRFScanner);
}

/**
 * @brief Handles the cc1101 Scanner menu button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the cc1101 Scanner screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::cc1101ScannerClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>( lv_event_get_user_data(event) );

    if (screenManager == nullptr)
        return;

    screenManager->show(Screen::CC1101Scanner);
}


/**
 * @brief Handles the BLE Scanner/Foxhunt menu button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the BLE Scanner/Foxhunt screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::bleExplorerClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    screenManager->show(Screen::BLEExplorer);
}