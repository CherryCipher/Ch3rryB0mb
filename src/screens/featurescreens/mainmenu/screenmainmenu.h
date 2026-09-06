#pragma once

#include <lvgl.h>
#include "app/features/features.h"

class ScreenManager;

/**
 * @class ScreenMainMenu
 * @brief Provides the paged Ch3rryB0mb main menu screen.
 *
 * The main menu groups application features into separate pages.
 * Navigation between pages is handled locally while feature screen
 * navigation is delegated to the ScreenManager.
 */
class ScreenMainMenu
{
public:
    /**
     * @brief Creates the main menu screen.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param features Reference to the application Features.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(ScreenManager& screenManager, Features& features);

private:
    /**
     * @brief Available main menu pages.
     */
    enum class MainMenuPage : uint8_t {
        WiFi,
        BLE,
        NRF,
        SubGHz,
        Node,
        Count
    };

    static MainMenuPage currentPage;

    static ScreenManager* screenManager;
    static Features* features;

    static lv_obj_t* pageContainer;
    static lv_obj_t* pageLabel;

    /**
     * @brief Renders the currently selected main menu page.
     */
    static void renderPage();

    /**
     * @brief Renders the WiFi feature page.
     */
    static void renderWiFiPage();

    /**
     * @brief Renders the BLE feature page.
     */
    static void renderBLEPage();

    /**
     * @brief Renders the NRF24 feature page.
     */
    static void renderNRFPage();

    /**
     * @brief Renders the SubGHz feature page.
     */
    static void renderSubGHzPage();

    /**
     * @brief Renders the node feature page.
     */
    static void renderNodePage();

    /**
     * @brief Returns the display name for a main menu page.
     *
     * @param page Page to get the name for.
     *
     * @return Page display name.
     */
    static const char* getPageName(MainMenuPage page);

    /**
     * @brief Handles navigation to the previous main menu page.
     *
     * @param event Pointer to the LVGL event.
     */
    static void previousPageClicked(lv_event_t* event);

    /**
     * @brief Handles navigation to the next main menu page.
     *
     * @param event Pointer to the LVGL event.
     */
    static void nextPageClicked(lv_event_t* event);

    /**
     * @brief Handles the AP Mode menu button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void apModeClicked(lv_event_t* event);

    /**
     * @brief Handles the WifiLab menu button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void wifiLabClicked(lv_event_t* event);

    /**
     * @brief Handles the Packet Viewer menu button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void packetViewerClicked(lv_event_t* event);

    /**
     * @brief Handles the NRF Scanner menu button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void nrfScannerClicked(lv_event_t* event);

    /**
     * @brief Handles the SubGHz Scanner menu button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void cc1101ScannerClicked(lv_event_t* event);

    /**
     * @brief Handles the BLE Explorer menu button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void bleExplorerClicked(lv_event_t* event);

    /**
     * @brief Handles the Find Node menu button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void findNodeClicked(lv_event_t* event);
};