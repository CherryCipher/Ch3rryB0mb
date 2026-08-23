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

#include "featurescreens/nrfscanner/screennrfscanner.h"

#include "featurescreens/cc1101scanner/screencc1101scanner.h"

#include "featurescreens/bleexplorer/screenbleexplorer.h"
#include "featurescreens/blefoxhunt/screenblefoxhunt.h"

/**
 * @brief Constructs a new ScreenManager.
 *
 * Stores references to the application's Logger and feature container.
 *
 * @param logger Reference to the application's Logger.
 * @param features Reference to the application's feature container.
 */
ScreenManager::ScreenManager( Logger& logger, Features& features )
    : logger(logger),
      features(features)
{
}

/**
 * @brief Starts the ScreenManager.
 *
 * Displays the main menu as the initial application screen without
 * creating a navigation history entry.
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
 *
 * Called when the application is shutting down.
 */
void ScreenManager::stop()
{
}

/**
 * @brief Shows an application screen.
 *
 * Stores the currently active screen in the navigation history and
 * displays the requested screen.
 *
 * @param screen Screen that should be displayed.
 */
void ScreenManager::show( Screen screen )
{
    showInternal(screen, true);
}

/**
 * @brief Creates and displays an application screen.
 *
 * Creates the requested screen using its corresponding screen class.
 *
 * When addToHistory is enabled, the currently active screen is pushed
 * onto the navigation history stack before the new screen is loaded.
 *
 * @param screen Screen that should be displayed.
 * @param addToHistory true to store the current screen in navigation
 * history, false when navigating backwards.
 */
void ScreenManager::showInternal( Screen screen, bool addToHistory )
{
    lv_obj_t* newScreen = nullptr;

    // Create requested screen.
    switch (screen)
    {
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

        case Screen::BLEExplorer:
            newScreen = ScreenBLEExplorer::create(*this, features.bleExplorer, features.bleFoxHunt);
            break;

        case Screen::BLEFoxHunt:
            newScreen = ScreenBLEFoxHunt::create(*this, features.bleFoxHunt);
            break;
        
        case Screen::NRFScanner:
            newScreen = ScreenNrfScanner::create(*this, features.nrfScanner);
            break;

        case Screen::CC1101Scanner:
            newScreen = ScreenCC1101Scanner::create(*this, features.cc1101Scanner);
            break;
    }

    if (newScreen == nullptr)
    {
        logger.error("Failed to create requested screen.");
        return;
    }

    // Store the current screen in navigation history when moving
    // forward.
    if (addToHistory && currentScreenObject != nullptr)
    {
        if (historySize < MAX_HISTORY)
        {
            screenHistory[historySize] = currentScreen;
            historySize++;
        }
        else
            logger.error("Screen navigation history is full.");
    }

    // Store the newly active application screen.
    currentScreen = screen;

    switchScreen(newScreen);
}

/**
 * @brief Returns to the previously active screen.
 *
 * Removes the most recent screen from the navigation history stack
 * and displays it without creating a new history entry.
 *
 * If the navigation history is empty, this method does nothing.
 */
void ScreenManager::back()
{
    if (historySize == 0)
        return;

    // Pop the most recent screen from history.
    historySize--;

    Screen targetScreen = screenHistory[historySize];

    // Display the previous screen without adding the current screen
    // back into navigation history.
    showInternal(targetScreen, false);
}

/**
 * @brief Replaces the currently active LVGL screen.
 *
 * Loads the provided screen and removes the previously active
 * LVGL screen object from memory.
 *
 * @param newScreen Pointer to the new LVGL screen object.
 */
void ScreenManager::switchScreen( lv_obj_t* newScreen )
{
    if (newScreen == nullptr)
    {
        logger.error("New screen is null, nothing to show.");

        return;
    }

    lv_obj_t* oldScreen = currentScreenObject;

    currentScreenObject = newScreen;

    lv_screen_load(currentScreenObject);

    logger.info("Showing screen.");

    if (oldScreen != nullptr)
        lv_obj_delete(oldScreen);
}