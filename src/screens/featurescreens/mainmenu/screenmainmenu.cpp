#include "screenmainmenu.h"

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"

ScreenMainMenu::MainMenuPage ScreenMainMenu::currentPage = ScreenMainMenu::MainMenuPage::WiFi;

ScreenManager* ScreenMainMenu::screenManager = nullptr;
Features* ScreenMainMenu::features = nullptr;

lv_obj_t* ScreenMainMenu::pageContainer = nullptr;
lv_obj_t* ScreenMainMenu::pageLabel = nullptr;

/**
 * @brief Creates the main menu screen.
 *
 * Creates the paged Ch3rryB0mb main menu with a fixed header,
 * feature content area and page navigation footer.
 *
 * @param manager Reference to the application ScreenManager.
 * @param appFeatures Reference to the application Features.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenMainMenu::create(ScreenManager& manager, Features& appFeatures)
{
    screenManager = &manager;
    features = &appFeatures;

    lv_obj_t* screen = UIWidgets::createScreen();

    UIWidgets::addHeader(screen, 0, 0, "CH3RRYB0MB", features->isWiFiConnected() ? LV_SYMBOL_WIFI : nullptr);

    pageContainer = lv_obj_create(screen);
    lv_obj_set_pos(pageContainer, 0, 50);
    lv_obj_set_size(pageContainer, 240, 205);
    lv_obj_set_style_bg_opa(pageContainer, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(pageContainer, 0, 0);
    lv_obj_set_style_pad_all(pageContainer, 0, 0);
    lv_obj_clear_flag(pageContainer, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* previousButton = UIWidgets::addButton(screen, 15, 265, "<", 45, 40);
    lv_obj_add_event_cb(previousButton, previousPageClicked, LV_EVENT_CLICKED, nullptr);

    pageLabel = UIWidgets::addText(screen, 70, 277, "", 100);
    lv_obj_set_style_text_align(pageLabel, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_t* nextButton = UIWidgets::addButton(screen, 180, 265, ">", 45, 40);
    lv_obj_add_event_cb(nextButton, nextPageClicked, LV_EVENT_CLICKED, nullptr);

    renderPage();

    return screen;
}

/**
 * @brief Renders the currently selected main menu page.
 */
void ScreenMainMenu::renderPage()
{
    if (pageContainer == nullptr || pageLabel == nullptr) return;

    lv_obj_clean(pageContainer);
    lv_label_set_text(pageLabel, getPageName(currentPage));

    switch (currentPage) {
        case MainMenuPage::WiFi: renderWiFiPage(); break;
        case MainMenuPage::BLE: renderBLEPage(); break;
        case MainMenuPage::NRF: renderNRFPage(); break;
        case MainMenuPage::SubGHz: renderSubGHzPage(); break;
        case MainMenuPage::Node: renderNodePage(); break;
        default: break;
    }
}

/**
 * @brief Renders the WiFi feature page.
 */
void ScreenMainMenu::renderWiFiPage()
{
    lv_obj_t* apModeButton = UIWidgets::addButton(pageContainer, 15, 10, "> AP Mode", 100, 45);
    lv_obj_add_event_cb(apModeButton, apModeClicked, LV_EVENT_CLICKED, screenManager);

    lv_obj_t* wifiLabButton = UIWidgets::addButton(pageContainer, 125, 10, "> WifiL4b", 100, 45);
    lv_obj_add_event_cb(wifiLabButton, wifiLabClicked, LV_EVENT_CLICKED, screenManager);

    lv_obj_t* packetViewerButton = UIWidgets::addButton(pageContainer, 15, 65, "> P4ckets", 100, 45);
    lv_obj_add_event_cb(packetViewerButton, packetViewerClicked, LV_EVENT_CLICKED, screenManager);
}

/**
 * @brief Renders the BLE feature page.
 */
void ScreenMainMenu::renderBLEPage()
{
    lv_obj_t* bleExplorerButton = UIWidgets::addButton(pageContainer, 15, 10, "> BLE Exp", 100, 45);
    lv_obj_add_event_cb(bleExplorerButton, bleExplorerClicked, LV_EVENT_CLICKED, screenManager);
}

/**
 * @brief Renders the NRF24 feature page.
 */
void ScreenMainMenu::renderNRFPage()
{
    if (features == nullptr) return;

    if (!features->nrfScanner.isAvailable()) {
        UIWidgets::addText(pageContainer, 15, 20, "NRF24 NOT AVAILABLE", 210);
        return;
    }

    lv_obj_t* nrfScannerButton = UIWidgets::addButton(pageContainer, 15, 10, "> NRF Sc4n", 100, 45);
    lv_obj_add_event_cb(nrfScannerButton, nrfScannerClicked, LV_EVENT_CLICKED, screenManager);
}

/**
 * @brief Renders the SubGHz feature page.
 */
void ScreenMainMenu::renderSubGHzPage()
{
    if (features == nullptr) return;

    if (!features->cc1101Scanner.isAvailable()) {
        UIWidgets::addText(pageContainer, 15, 20, "CC1101 NOT AVAILABLE", 210);
        return;
    }

    lv_obj_t* cc1101ScannerButton = UIWidgets::addButton(pageContainer, 15, 10, "> Sub Sc4n", 100, 45);
    lv_obj_add_event_cb(cc1101ScannerButton, cc1101ScannerClicked, LV_EVENT_CLICKED, screenManager);
}

/**
 * @brief Renders the node feature page.
 */
void ScreenMainMenu::renderNodePage()
{
    lv_obj_t* findNodeButton = UIWidgets::addButton(pageContainer, 15, 10, "> Find Node", 100, 45);
    lv_obj_add_event_cb(findNodeButton, findNodeClicked, LV_EVENT_CLICKED, screenManager);
}

/**
 * @brief Returns the display name for a main menu page.
 *
 * @param page Page to get the name for.
 *
 * @return Page display name.
 */
const char* ScreenMainMenu::getPageName(MainMenuPage page)
{
    switch (page) {
        case MainMenuPage::WiFi: return "WIFI";
        case MainMenuPage::BLE: return "BLE";
        case MainMenuPage::NRF: return "NRF";
        case MainMenuPage::SubGHz: return "SUBGHZ";
        case MainMenuPage::Node: return "NODE";
        default: return "";
    }
}

/**
 * @brief Handles navigation to the previous main menu page.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::previousPageClicked(lv_event_t* event)
{
    uint8_t page = static_cast<uint8_t>(currentPage);
    uint8_t count = static_cast<uint8_t>(MainMenuPage::Count);

    page = page == 0 ? count - 1 : page - 1;
    currentPage = static_cast<MainMenuPage>(page);

    renderPage();
}

/**
 * @brief Handles navigation to the next main menu page.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::nextPageClicked(lv_event_t* event)
{
    uint8_t page = static_cast<uint8_t>(currentPage);
    uint8_t count = static_cast<uint8_t>(MainMenuPage::Count);

    page = (page + 1) % count;
    currentPage = static_cast<MainMenuPage>(page);

    renderPage();
}

/**
 * @brief Handles the AP Mode menu button event.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::apModeClicked(lv_event_t* event)
{
    ScreenManager* manager = static_cast<ScreenManager*>(lv_event_get_user_data(event));
    if (manager == nullptr) return;

    manager->show(Screen::APMode);
}

/**
 * @brief Handles the WifiLab menu button event.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::wifiLabClicked(lv_event_t* event)
{
    ScreenManager* manager = static_cast<ScreenManager*>(lv_event_get_user_data(event));
    if (manager == nullptr) return;

    manager->show(Screen::WifiLab);
}

/**
 * @brief Handles the Packet Viewer menu button event.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::packetViewerClicked(lv_event_t* event)
{
    ScreenManager* manager = static_cast<ScreenManager*>(lv_event_get_user_data(event));
    if (manager == nullptr) return;

    manager->show(Screen::PacketViewer);
}

/**
 * @brief Handles the NRF Scanner menu button event.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::nrfScannerClicked(lv_event_t* event)
{
    ScreenManager* manager = static_cast<ScreenManager*>(lv_event_get_user_data(event));
    if (manager == nullptr) return;

    manager->show(Screen::NRFScanner);
}

/**
 * @brief Handles the SubGHz Scanner menu button event.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::cc1101ScannerClicked(lv_event_t* event)
{
    ScreenManager* manager = static_cast<ScreenManager*>(lv_event_get_user_data(event));
    if (manager == nullptr) return;

    manager->show(Screen::CC1101Scanner);
}

/**
 * @brief Handles the BLE Explorer menu button event.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::bleExplorerClicked(lv_event_t* event)
{
    ScreenManager* manager = static_cast<ScreenManager*>(lv_event_get_user_data(event));
    if (manager == nullptr) return;

    manager->show(Screen::BLEExplorer);
}

/**
 * @brief Handles the Find Node menu button event.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::findNodeClicked(lv_event_t* event)
{
    ScreenManager* manager = static_cast<ScreenManager*>(lv_event_get_user_data(event));
    if (manager == nullptr) return;

    manager->show(Screen::NodeFinder);
}