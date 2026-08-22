/**
 * @file screenblefoxhunt.h
 * @brief BLE Fox Hunt screen interface.
 */

#pragma once

#include <lvgl.h>

class ScreenManager;
class BLEFoxHunt;

/**
 * @class ScreenBLEFoxHunt
 * @brief Displays live signal information for a selected BLE target.
 *
 * The screen periodically reads BLEFoxHunt measurements and displays
 * RSSI, peak RSSI, signal strength and last-seen information.
 */
class ScreenBLEFoxHunt
{
public:
    /**
     * @brief Creates the BLE Fox Hunt screen.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param bleFoxHunt Reference to the BLE Fox Hunt feature.
     *
     * @return Pointer to the created LVGL screen.
     */
    static lv_obj_t* create(ScreenManager& screenManager, BLEFoxHunt& bleFoxHunt);

private:
    /**
     * @struct ScreenContext
     * @brief Context shared by Fox Hunt screen callbacks.
     */
    struct ScreenContext
    {
        ScreenManager* screenManager = nullptr;
        BLEFoxHunt* bleFoxHunt = nullptr;
    };

    /**
     * @brief Shared screen callback context.
     */
    static ScreenContext screenContext;

    /**
     * @brief Target name label.
     */
    static lv_obj_t* targetNameLabel;

    /**
     * @brief Target BLE address label.
     */
    static lv_obj_t* targetAddressLabel;

    /**
     * @brief Current RSSI label.
     */
    static lv_obj_t* rssiLabel;

    /**
     * @brief Peak RSSI label.
     */
    static lv_obj_t* peakLabel;

    /**
     * @brief Signal strength text label.
     */
    static lv_obj_t* signalLabel;

    /**
     * @brief Last-seen label.
     */
    static lv_obj_t* lastSeenLabel;

    /**
     * @brief Visual signal strength bar.
     */
    static lv_obj_t* signalBar;

    /**
     * @brief Timer used to refresh Fox Hunt measurements.
     */
    static lv_timer_t* updateTimer;

    /**
     * @brief Indicates whether Fox Hunt scanning has been started.
     */
    static bool huntStarted;

    /**
     * @brief Refreshes all Fox Hunt UI values.
     *
     * @param bleFoxHunt Reference to the BLE Fox Hunt feature.
     */
    static void render(BLEFoxHunt& bleFoxHunt);

    /**
     * @brief Handles periodic Fox Hunt updates.
     *
     * The first timer callback starts BLE scanning after the previous
     * screen has completed its cleanup. Subsequent callbacks update
     * the target measurements.
     *
     * @param timer Pointer to the LVGL timer.
     */
    static void updateTimerCallback(lv_timer_t* timer);

    /**
     * @brief Handles the Fox Hunt back button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);

    /**
     * @brief Cleans up Fox Hunt resources when the screen is deleted.
     *
     * @param event Pointer to the LVGL delete event.
     */
    static void screenDeleted(lv_event_t* event);
};