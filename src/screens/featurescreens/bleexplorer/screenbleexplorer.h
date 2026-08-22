/**
 * @file screenbleexplorer.h
 * @brief BLE Explorer screen interface.
 */

#pragma once

#include <lvgl.h>

class ScreenManager;
class BLEExplorer;
class BLEFoxHunt;

/**
 * @class ScreenBLEExplorer
 * @brief Displays nearby Bluetooth Low Energy devices.
 *
 * The BLE Explorer screen provides controls for starting and stopping
 * BLE discovery and displays discovered BLE devices with live RSSI.
 */
class ScreenBLEExplorer
{
public:
    /**
     * @brief Creates the BLE Explorer screen.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param bleExplorer Reference to the BLE Explorer feature.
     *
     * @return Pointer to the created LVGL screen.
     */
    static lv_obj_t* create( ScreenManager& screenManager, BLEExplorer& bleExplorer, BLEFoxHunt& bleFoxHunt );

private:
    /**
     * @struct DeviceClickContext
     * @brief Context passed to BLE device button callbacks.
     */
    struct DeviceClickContext
    {
        ScreenManager* screenManager = nullptr;
        BLEExplorer* bleExplorer = nullptr;
        BLEFoxHunt* bleFoxHunt = nullptr;
        uint8_t index = 0;
    };

    /**
     * @struct RefreshContext
     * @brief Context used by the BLE Explorer refresh timer.
     */
    struct RefreshContext
    {
        ScreenManager* screenManager = nullptr;
        BLEExplorer* bleExplorer = nullptr;
        BLEFoxHunt* bleFoxHunt = nullptr;
    };

    /**
     * @brief BLE device list container.
     */
    static lv_obj_t* deviceContainer;

    /**
     * @brief BLE scan button.
     */
    static lv_obj_t* scanButton;

    /**
     * @brief BLE scan status label.
     */
    static lv_obj_t* statusLabel;

    /**
     * @brief Timer used to refresh BLE scan results.
     */
    static lv_timer_t* refreshTimer;

    /**
     * @brief Context used by the refresh timer.
     */
    static RefreshContext refreshContext;

    /**
     * @brief Last rendered number of BLE devices.
     */
    static uint8_t lastDeviceCount;

    /**
     * @brief Starts BLE scanning.
     *
     * @param screenManager Reference to the ScreenManager.
     * @param bleExplorer Reference to the BLE Explorer feature.
     */
    static void startScan(ScreenManager& screenManager, BLEExplorer& bleExplorer);

    /**
     * @brief Stops BLE scanning.
     *
     * Stops the active BLE scan and refreshes the displayed device list
     * while preserving the Fox Hunt context for selectable device rows.
     *
     * @param screenManager Reference to the ScreenManager.
     * @param bleExplorer Reference to the BLE Explorer feature.
     * @param bleFoxHunt Reference to the BLE Fox Hunt feature.
     */
    static void stopScan(
        ScreenManager& screenManager,
        BLEExplorer& bleExplorer,
        BLEFoxHunt& bleFoxHunt
    );

    /**
     * @brief Rebuilds the displayed BLE device list.
     *
     * @param screenManager Reference to the ScreenManager.
     * @param bleExplorer Reference to the BLE Explorer feature.
     */
    static void renderDevices(
    ScreenManager& screenManager,
    BLEExplorer& bleExplorer,
    BLEFoxHunt& bleFoxHunt
    );

    /**
     * @brief Updates the BLE Explorer status label.
     *
     * @param bleExplorer Reference to the BLE Explorer feature.
     */
    static void updateStatus(BLEExplorer& bleExplorer);

    /**
     * @brief Handles the SCAN/STOP button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void scanClicked(lv_event_t* event);

    /**
     * @brief Handles selection of a discovered BLE device.
     *
     * @param event Pointer to the LVGL event.
     */
    static void deviceClicked(lv_event_t* event);

    /**
     * @brief Releases a BLE device click context.
     *
     * @param event Pointer to the LVGL event.
     */
    static void deviceDeleted(lv_event_t* event);

    /**
     * @brief Handles the BLE Explorer back button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);

    /**
     * @brief Periodically refreshes BLE scan results.
     *
     * @param timer Pointer to the LVGL timer.
     */
    static void refreshTimerCallback(lv_timer_t* timer);

    /**
     * @brief Cleans up resources when the screen is deleted.
     *
     * @param event Pointer to the LVGL event.
     */
    static void screenDeleted(lv_event_t* event);
};