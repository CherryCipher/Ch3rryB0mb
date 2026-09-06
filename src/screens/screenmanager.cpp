/**
 * @file screenmanager.cpp
 * @brief Implementation of application screen navigation.
 */

#include "screenmanager.h"

#include "../app/features/features.h"

#include "featurescreens/mainmenu/screenmainmenu.h"

#include "featurescreens/apmode/screenapmode.h"
#include "featurescreens/apmode/screenapmodeconfig.h"

#include "featurescreens/wifilab/screenwifilab.h"
#include "featurescreens/wifilab/screenconnect.h"
#include "featurescreens/packetviewer/screenpacketviewer.h"

#include "featurescreens/nrfscanner/screennrfscanner.h"
#include "featurescreens/nrfsend/screennrfsend.h"

#include "featurescreens/cc1101scanner/screencc1101scanner.h"

#include "featurescreens/bleexplorer/screenbleexplorer.h"
#include "featurescreens/blefoxhunt/screenblefoxhunt.h"

#include "featurescreens/nodefinder/screennodefinder.h"
#include "featurescreens/nodeconfig/screennodeconfig.h"

/**
 * @brief Constructs a new ScreenManager.
 *
 * @param logger Reference to the application's Logger.
 * @param features Reference to the application's feature container.
 */
ScreenManager::ScreenManager(Logger& logger, Features& features)
    : logger(logger),
      features(features)
{
}

/**
 * @brief Starts the ScreenManager.
 *
 * @return true when the initial screen has been created successfully.
 */
bool ScreenManager::start()
{
    showInternal(Screen::MainMenu, false);

    logger.info("ScreenManager started.");

    return true;
}

/**
 * @brief Stops the ScreenManager.
 */
void ScreenManager::stop()
{
}

/**
 * @brief Shows an application screen.
 *
 * @param screen Screen that should be displayed.
 */
void ScreenManager::show(Screen screen)
{
    showInternal(screen, true);
}

/**
 * @brief Creates and displays an application screen.
 *
 * @param screen Screen that should be displayed.
 * @param addToHistory true to store the current screen in navigation history.
 */
void ScreenManager::showInternal(Screen screen, bool addToHistory)
{
    lv_obj_t* newScreen = nullptr;

    switch (screen) {
        case Screen::MainMenu:
            newScreen = ScreenMainMenu::create(*this, features);
            break;

        case Screen::APMode:
            newScreen = ScreenAPMode::create(*this, features.apMode);
            break;

        case Screen::ApModeConfig:
            newScreen = ScreenAPModeConfig::create(*this, features.apMode);
            break;

        case Screen::WifiLab:
            if (features.wifiLab.isConnected())
                newScreen = ScreenConnect::create(*this, features.wifiLab);
            else
                newScreen = ScreenWifiLab::create(*this, features.wifiLab);
            break;

        case Screen::WifiConnect:
            newScreen = ScreenConnect::create(*this, features.wifiLab);
            break;

        case Screen::PacketViewer:
            newScreen = ScreenPacketViewer::create(*this, features.packetViewer);
            break;

        case Screen::BLEExplorer:
            newScreen = ScreenBLEExplorer::create(*this, features.bleExplorer, features.bleFoxHunt);
            break;

        case Screen::BLEFoxHunt:
            newScreen = ScreenBLEFoxHunt::create(*this, features.bleFoxHunt);
            break;

        case Screen::NRFScanner:
            newScreen = ScreenNrfScanner::create(*this, features.nrfScanner);
            break;

        case Screen::NRFSend:
            newScreen = ScreenNRFSend::create(*this, features.nrfSend);
            break;

        case Screen::CC1101Scanner:
            newScreen = ScreenCC1101Scanner::create(*this, features.cc1101Scanner);
            break;

        case Screen::NodeFinder:
            newScreen = ScreenNodeFinder::create(*this, features.configureNode);
            break;

        case Screen::NodeConfig:
            newScreen = ScreenNodeConfig::create(*this, features.configureNode);
            break;
    }

    if (newScreen == nullptr) {
        logger.error("Failed to create requested screen.");
        return;
    }

    if (addToHistory && currentScreenObject != nullptr) {
        if (historySize < MAX_HISTORY) {
            screenHistory[historySize] = currentScreen;
            historySize++;
        } else
            logger.error("Screen navigation history is full.");
    }

    currentScreen = screen;

    switchScreen(newScreen);
}

/**
 * @brief Returns to the previously active screen.
 */
void ScreenManager::back()
{
    if (historySize == 0) return;

    historySize--;

    Screen targetScreen = screenHistory[historySize];
    showInternal(targetScreen, false);
}

/**
 * @brief Replaces the currently active LVGL screen.
 *
 * @param newScreen Pointer to the new LVGL screen object.
 */
void ScreenManager::switchScreen(lv_obj_t* newScreen)
{
    if (newScreen == nullptr) {
        logger.error("New screen is null, nothing to show.");
        return;
    }

    lv_obj_t* oldScreen = currentScreenObject;

    currentScreenObject = newScreen;
    lv_screen_load(currentScreenObject);

    logger.info("Showing screen.");

    if (oldScreen != nullptr) lv_obj_delete(oldScreen);
}

/**
 * @brief Returns directly to the main menu.
 */
void ScreenManager::home()
{
    historySize = 0;
    showInternal(Screen::MainMenu, false);
}