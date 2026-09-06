/**
 * @file screennodefinder.h
 * @brief Declaration of the Ch3rryN0de finder screen.
 *
 * The screen displays Ch3rryN0de devices discovered by the ConfigureNode
 * feature and allows the user to select a node for configuration.
 */

#pragma once

#include <lvgl.h>

#include "app/features/configurenode/configurenode.h"

class ScreenManager;

/**
 * @class ScreenNodeFinder
 * @brief Displays and selects discovered Ch3rryN0de devices.
 */
class ScreenNodeFinder
{
public:
    /**
     * @brief Creates the Ch3rryN0de finder screen.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param configureNode Reference to the ConfigureNode feature.
     *
     * @return Pointer to the created LVGL screen.
     */
    static lv_obj_t* create(ScreenManager& screenManager, ConfigureNode& configureNode);

private:
    /**
     * @struct Context
     * @brief Shared callback context for the node finder screen.
     */
    struct Context
    {
        ScreenManager* screenManager = nullptr;
        ConfigureNode* configureNode = nullptr;
    };

    /**
     * @struct NodeButtonContext
     * @brief Context associated with a discovered node button.
     */
    struct NodeButtonContext
    {
        uint8_t index = 0;
    };

    static Context context;
    static NodeButtonContext nodeButtonContexts[ConfigureNode::MAX_NODES];

    static lv_obj_t* nodeContainer;
    static lv_obj_t* statusLabel;
    static lv_timer_t* refreshTimer;

    static uint8_t renderedNodeCount;

    /**
     * @brief Starts a new node scan.
     *
     * @param configureNode Reference to the ConfigureNode feature.
     */
    static void startScan(ConfigureNode& configureNode);

    /**
     * @brief Updates the displayed node list.
     *
     * @param configureNode Reference to the ConfigureNode feature.
     */
    static void updateNodes(ConfigureNode& configureNode);

    /**
     * @brief Updates the scan status label.
     *
     * @param configureNode Reference to the ConfigureNode feature.
     */
    static void updateStatus(ConfigureNode& configureNode);

    /**
     * @brief Handles a discovered node button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void nodeClicked(lv_event_t* event);

    /**
     * @brief Handles the scan button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void scanClicked(lv_event_t* event);

    /**
     * @brief Handles the back button event.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);

    /**
     * @brief Periodically refreshes discovered nodes.
     *
     * @param timer Pointer to the LVGL timer.
     */
    static void refreshTimerCallback(lv_timer_t* timer);

    /**
     * @brief Cleans up screen resources when the screen is deleted.
     *
     * @param event Pointer to the LVGL event.
     */
    static void screenDeleted(lv_event_t* event);
};