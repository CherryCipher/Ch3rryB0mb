/**
 * @file screenmanager.h
 * @brief Application screen navigation manager.
 *
 * This file defines the ScreenManager responsible for creating,
 * displaying and navigating between Ch3rryB0mb application screens.
 */

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
    ApModeConfig,
    WifiLab,
    WifiConnect,
    PacketViewer,
    BLEExplorer,
    BLEFoxHunt,
    NRFScanner,
    CC1101Scanner,
    NodeFinder,
    NodeConfig
};

/**
 * @class ScreenManager
 * @brief Manages application screen navigation and lifecycle.
 *
 * The ScreenManager is responsible for creating application screens,
 * switching the active LVGL screen and maintaining a navigation history.
 *
 * Forward navigation stores the currently active screen in a small
 * history stack. Calling back() restores the most recently visited
 * screen without adding the current screen back into the history.
 *
 * Individual screen classes remain responsible for their own layout,
 * controls and event handlers.
 *
 * @code
 * screenManager.show(Screen::APMode);
 * screenManager.show(Screen::ApModeConfig);
 * screenManager.back();
 * @endcode
 */
class ScreenManager
{
public:
    /**
     * @brief Constructs a new ScreenManager.
     *
     * Stores references to the application Logger and feature container.
     *
     * @param logger Reference to the application's Logger.
     * @param features Reference to the application's feature container.
     */
    ScreenManager( Logger& logger, Features& features );

    /**
     * @brief Starts the ScreenManager.
     *
     * Displays the main menu as the initial application screen.
     *
     * The UIManager must be started before this method is called,
     * because screen creation requires an initialized LVGL instance.
     *
     * @return true if the ScreenManager was started successfully.
     */
    bool start();

    /**
     * @brief Stops the ScreenManager.
     *
     * Called when the application is shutting down.
     */
    void stop();

    /**
     * @brief Shows an application screen.
     *
     * Creates the requested screen and makes it the active LVGL screen.
     * The currently active screen is stored in the navigation history
     * before switching to the requested screen.
     *
     * @param screen Screen that should be displayed.
     */
    void show(Screen screen);

    /**
     * @brief Returns to the previously active screen.
     *
     * Removes the most recent screen from the navigation history stack
     * and displays it.
     *
     * If the history stack is empty, this method does nothing.
     */
    void back();

    /**
     * @brief Returns directly to the main menu.
     *
     * Clears the navigation history and shows the main menu without storing
     * the current screen in the history stack.
     */
    void home();

private:
    /**
     * @brief Maximum number of screens stored in navigation history.
     */
    static constexpr uint8_t MAX_HISTORY = 8;

    /**
     * @brief Creates and displays an application screen.
     *
     * This internal method performs the actual screen creation and
     * switching.
     *
     * When addToHistory is true, the currently active screen is stored
     * in the navigation history before switching.
     *
     * @param screen Screen that should be displayed.
     * @param addToHistory true to add the current screen to navigation
     * history, false when navigating backwards.
     */
    void showInternal( Screen screen, bool addToHistory );

    /**
     * @brief Replaces the currently active LVGL screen.
     *
     * Loads the new screen and deletes the previously active screen
     * object from memory.
     *
     * @param newScreen Pointer to the new LVGL screen object.
     */
    void switchScreen(lv_obj_t* newScreen);

    /**
     * @brief Navigation history stack.
     *
     * Stores previously active screens in the order in which they were
     * visited.
     */
    Screen screenHistory[MAX_HISTORY];

    /**
     * @brief Number of screens currently stored in navigation history.
     */
    uint8_t historySize = 0;

    /**
     * @brief Currently active application screen.
     */
    Screen currentScreen = Screen::MainMenu;

    /**
     * @brief Pointer to the currently active LVGL screen object.
     */
    lv_obj_t* currentScreenObject = nullptr;

    /**
     * @brief Reference to the application's Logger instance.
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