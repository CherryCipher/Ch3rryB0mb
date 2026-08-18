#pragma once

#include <lvgl.h>
#include <cstdint>
#include "../services/logger/logger.h"
#include "../src/app/features/features.h"

class Features;

/**
 * @enum Screen
 * @brief Identifies the available application screens.
 *
 * Used by the ScreenManager to identify and switch between
 * the different Ch3rryB0mb application screens.
 */
enum class Screen : uint8_t
{
    MainMenu,
    APMode,
    ApModeConfig
};

/**
 * @class ScreenManager
 * @brief Manages application screen navigation and lifecycle.
 *
 * The ScreenManager is responsible for showing application screens,
 * keeping track of the current and previous screen and safely replacing
 * the active LVGL screen.
 *
 * Individual screen classes are responsible for creating their own
 * layout, controls and event handlers.
 *
 * @code
 * screenManager.show(Screen::APMode);
 * screenManager.back();
 * @endcode
 */
class ScreenManager
{
public:
    /**
     * @brief Constructs a new Screen Manager.
     *
     * @param logger Reference to the application's Logger.
     */
    ScreenManager(Logger& logger, Features& features);

    /**
     * @brief Starts the screen manager.
     *
     * Displays the initial application screen.
     *
     * The UIManager must be started before this method is called,
     * because screen creation requires an initialized LVGL instance.
     *
     * @return true if the screen manager was started successfully.
     */
    bool start();

    /**
     * @brief Shows an application screen.
     *
     * Creates the requested screen through its corresponding screen
     * class and makes it the active LVGL screen.
     *
     * @param screen Screen that should be displayed.
     */
    void show(Screen screen);

    /**
     * @brief Returns to the previously active screen.
     *
     * If no previous screen is available, this method does nothing.
     */
    void back();

    /**
    * @brief Stops the ScreenManager
    *
    * This function should be called when the application is shutting down
    */
    void stop();

private:
    /**
     * @brief Replaces the currently active LVGL screen.
     *
     * Loads the new screen and deletes the previously active
     * screen from memory.
     *
     * @param newScreen Pointer to the new LVGL screen object.
     */
    void switchScreen(lv_obj_t* newScreen);

    /**
     * @brief Currently active application screen.
     */
    Screen currentScreen = Screen::MainMenu;

    /**
     * @brief Previously active application screen.
     */
    Screen previousScreen = Screen::MainMenu;

    /**
     * @brief Indicates whether a previous screen is available.
     */
    bool hasPreviousScreen = false;

    /**
     * @brief Pointer to the currently active LVGL screen object.
     */
    lv_obj_t* currentScreenObject = nullptr;

    /**
    * @brief Reference to the application's Logger instance.
    *
    * Used for logging messages related to web server operations.
    */
    Logger& logger;

    /**
     * @brief Reference to the application's feature container.
     *
     * Used to provide screens access to the application features
     * they require.
     */
    Features& features;
};