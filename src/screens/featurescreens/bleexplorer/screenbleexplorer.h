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
 *
 * Device rows are created only when new devices are discovered.
 * Existing rows are updated in place to avoid repeatedly allocating
 * and destroying LVGL objects while scanning.
 */
class ScreenBLEExplorer
{
public:
    /**
     * @brief Creates the BLE Explorer screen.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param bleExplorer Reference to the BLE Explorer feature.
     * @param bleFoxHunt Reference to the BLE Fox Hunt feature.
     *
     * @return Pointer to the created LVGL screen.
     */
    static lv_obj_t* create( ScreenManager& screenManager, BLEExplorer& bleExplorer, BLEFoxHunt& bleFoxHunt );

private:
    /**
     * @struct RefreshContext
     * @brief Context shared by BLE Explorer callbacks.
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
     * @brief Placeholder displayed while no devices are available.
     */
    static lv_obj_t* emptyLabel;

    /**
     * @brief Timer used to refresh BLE scan results.
     */
    static lv_timer_t* refreshTimer;

    /**
     * @brief Context shared by the refresh timer and device callbacks.
     */
    static RefreshContext refreshContext;

    /**
     * @brief Number of BLE device rows currently created.
     */
    static uint8_t renderedDeviceCount;

    /**
     * @brief Starts BLE scanning.
     *
     * @param bleExplorer Reference to the BLE Explorer feature.
     */
    static void startScan(BLEExplorer& bleExplorer);

    /**
     * @brief Stops BLE scanning.
     *
     * @param bleExplorer Reference to the BLE Explorer feature.
     */
    static void stopScan(BLEExplorer& bleExplorer);

    /**
     * @brief Updates the displayed BLE device rows.
     *
     * Creates rows only for newly discovered devices and updates the
     * label text of existing rows in place.
     *
     * @param bleExplorer Reference to the BLE Explorer feature.
     */
    static void updateDevices(BLEExplorer& bleExplorer);

    /**
     * @brief Updates a single BLE device row.
     *
     * @param row BLE device button.
     * @param bleExplorer Reference to the BLE Explorer feature.
     * @param index Device index represented by the row.
     */
    static void updateDeviceRow( lv_obj_t* row, BLEExplorer& bleExplorer, uint8_t index );

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
     * @param event Pointer to the LVGL delete event.
     */
    static void screenDeleted(lv_event_t* event);
};