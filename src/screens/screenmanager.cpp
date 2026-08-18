#include "screenmanager.h"

#include "../app/features/features.h"

#include "featurescreens/mainmenu/screenmainmenu.h"
#include "featurescreens/apmode/screenapmode.h"
#include "featurescreens/apmode/screenapmodeconfig.h"

/**
 * @brief Constructs a new ScreenManager.
 *
 * Stores a reference to the application's Logger.
 *
 * @param logger Reference to the application's Logger.
 */
ScreenManager::ScreenManager(Logger& logger, Features& features)
    : logger(logger),
      features(features)
{
}

/**
 * @brief Starts the screen manager.
 *
 * Displays the main menu as the initial application screen.
 *
 * @return true when the initial screen has been created successfully.
 */
bool ScreenManager::start()
{
    show(Screen::MainMenu);

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
 * Creates the requested screen using its corresponding screen class.
 * The current screen is stored as the previous screen before the newly
 * created screen is loaded.
 *
 * @param screen Screen that should be displayed.
 */
void ScreenManager::show(Screen screen)
{
    lv_obj_t* newScreen = nullptr;

    switch (screen)
    {
        case Screen::MainMenu:
            newScreen = ScreenMainMenu::create(*this);
            break;

        case Screen::APMode:
            newScreen = ScreenAPMode::create(*this, features.apMode);
            break;

        case Screen::ApModeConfig:
            newScreen = ScreenAPModeConfig::create(*this, features.apMode);
            break;
    }

    if (newScreen == nullptr)
    {
        return;
    }

    if (currentScreenObject != nullptr)
    {
        previousScreen = currentScreen;
        hasPreviousScreen = true;
    }

    currentScreen = screen;

    switchScreen(newScreen);
}

/**
 * @brief Returns to the previously active screen.
 *
 * Displays the previously active application screen if one exists.
 */
void ScreenManager::back()
{
    if (!hasPreviousScreen)
    {
        return;
    }

    Screen targetScreen = previousScreen;

    //
    // Prevent show() from treating the current screen as a new
    // navigation history entry when navigating backwards.
    //
    hasPreviousScreen = false;

    show(targetScreen);
}

/**
 * @brief Replaces the currently active LVGL screen.
 *
 * Loads the provided screen and removes the previously active
 * LVGL screen object from memory.
 *
 * @param newScreen Pointer to the new LVGL screen object.
 */
void ScreenManager::switchScreen(lv_obj_t* newScreen)
{
    if (newScreen == nullptr)
    {
        logger.error("New screen is null, nothing to show.");
        return;
    }

    lv_obj_t* oldScreen = currentScreenObject;

    currentScreenObject = newScreen;

    lv_screen_load(currentScreenObject);
    logger.info("Showing screen");

    if (oldScreen != nullptr)
    {
        lv_obj_delete(oldScreen);
    }
}