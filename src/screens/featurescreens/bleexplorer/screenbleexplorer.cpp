/**
 * @file screenbleexplorer.cpp
 * @brief Implementation of the BLE Explorer screen.
 */

#include "screenbleexplorer.h"

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/bleexplorer/bleexplorer.h"
#include "app/features/blefoxhunt/blefoxhunt.h"

lv_obj_t* ScreenBLEExplorer::deviceContainer = nullptr;
lv_obj_t* ScreenBLEExplorer::scanButton = nullptr;
lv_obj_t* ScreenBLEExplorer::statusLabel = nullptr;
lv_obj_t* ScreenBLEExplorer::emptyLabel = nullptr;
lv_timer_t* ScreenBLEExplorer::refreshTimer = nullptr;

ScreenBLEExplorer::RefreshContext ScreenBLEExplorer::refreshContext;

uint8_t ScreenBLEExplorer::renderedDeviceCount = 0;

/**
 * @brief Creates the BLE Explorer screen.
 */
lv_obj_t* ScreenBLEExplorer::create( ScreenManager& screenManager, BLEExplorer& bleExplorer, BLEFoxHunt& bleFoxHunt )
{
    lv_obj_t* screen = UIWidgets::createScreen();

    UIWidgets::addHeader(screen, 0, 0, "BLE EXPL0RER");

    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &screenManager);

    scanButton = UIWidgets::addButton(screen, 15, 50, "SCAN", 90, 35);
    lv_obj_add_event_cb(scanButton, scanClicked, LV_EVENT_CLICKED, &refreshContext);

    statusLabel = UIWidgets::addText(screen, 115, 58, "IDLE | 0", 115);

    deviceContainer = UIWidgets::createScrollContainer(screen, 10, 95, 220, 210);

    refreshContext.screenManager = &screenManager;
    refreshContext.bleExplorer = &bleExplorer;
    refreshContext.bleFoxHunt = &bleFoxHunt;

    renderedDeviceCount = 0;

    emptyLabel = UIWidgets::addText( deviceContainer, 5, 5, "Press SCAN to discover BLE devices.", 195 );

    refreshTimer = lv_timer_create( refreshTimerCallback, 500, &refreshContext );

    lv_obj_add_event_cb( screen, screenDeleted, LV_EVENT_DELETE, &bleExplorer );

    updateStatus(bleExplorer);

    return screen;
}

/**
 * @brief Starts BLE scanning.
 *
 * Clears the previous device rows once and prepares the container for
 * a new scan. Device rows are subsequently appended as devices appear.
 *
 * @param bleExplorer Reference to the BLE Explorer feature.
 */
void ScreenBLEExplorer::startScan(BLEExplorer& bleExplorer)
{
    if (!bleExplorer.startScan())
    {
        if (deviceContainer != nullptr)
        {
            lv_obj_clean(deviceContainer);
            emptyLabel = UIWidgets::addText( deviceContainer, 5, 5, "BLE scan failed.\n(disconnect to wifi if connected)", 195 );
        }

        renderedDeviceCount = 0;

        updateStatus(bleExplorer);
        return;
    }

    if (deviceContainer != nullptr)
    {
        lv_obj_clean(deviceContainer);
        emptyLabel = UIWidgets::addText(  deviceContainer, 5, 5, "Scanning for BLE devices...", 195 );
    }

    renderedDeviceCount = 0;

    if (scanButton != nullptr)
    {
        lv_obj_t* label = lv_obj_get_child(scanButton, 0);
        if (label != nullptr)
            lv_label_set_text(label, "STOP");
    }

    updateStatus(bleExplorer);
}

/**
 * @brief Stops BLE scanning.
 *
 * Stops the active scan while keeping the currently displayed device
 * rows available for selection.
 *
 * @param bleExplorer Reference to the BLE Explorer feature.
 */
void ScreenBLEExplorer::stopScan(BLEExplorer& bleExplorer)
{
    bleExplorer.stopScan();

    if (scanButton != nullptr)
    {
        lv_obj_t* label = lv_obj_get_child(scanButton, 0);

        if (label != nullptr)
            lv_label_set_text(label, "SCAN");
    }

    updateDevices(bleExplorer);
    updateStatus(bleExplorer);
}

/**
 * @brief Updates the displayed BLE device rows.
 *
 * Existing rows are updated in place. New LVGL buttons are created only
 * when additional BLE devices are discovered.
 *
 * This avoids repeatedly destroying and recreating the complete list
 * while scanning, reducing heap churn and UI processing overhead.
 *
 * @param bleExplorer Reference to the BLE Explorer feature.
 */
void ScreenBLEExplorer::updateDevices(BLEExplorer& bleExplorer)
{
    if (deviceContainer == nullptr)
        return;

    uint8_t deviceCount = bleExplorer.getDeviceCount();

    if (deviceCount == 0)
    {
        if (emptyLabel == nullptr)
            emptyLabel = UIWidgets::addText( deviceContainer, 5, 5, bleExplorer.isScanning() ? "Scanning for BLE devices..." : "No BLE devices found.", 195 );
        return;
    }

    if (emptyLabel != nullptr)
    {
        lv_obj_delete(emptyLabel);
        emptyLabel = nullptr;
    }

    /*
     * Update rows that already exist.
     */
    for (uint8_t i = 0; i < renderedDeviceCount && i < deviceCount; i++)
    {
        lv_obj_t* row = lv_obj_get_child(deviceContainer, i);

        if (row != nullptr)
            updateDeviceRow(row, bleExplorer, i);
    }

    /*
     * Create rows only for newly discovered devices.
     */
    for (uint8_t i = renderedDeviceCount; i < deviceCount; i++)
    {
        const BLEDeviceInfo& device = bleExplorer.getDevice(i);

        String name = device.name;

        if (name.length() == 0)
            name = "< unknown >";

        if (name.length() > 20)
            name = name.substring(0, 17) + "...";

        String text = name + "\n" + String(device.rssi) + " dBm | " + device.address;

        lv_obj_t* row = UIWidgets::addButton( deviceContainer, 5, 5 + (i * 58), text.c_str(), 195, 52 );

        /*
         * Only the device index is stored in the callback.
         *
         * The feature and ScreenManager references already live in the
         * shared refreshContext, so no per-row heap allocation is needed.
         */
        lv_obj_add_event_cb( row, deviceClicked, LV_EVENT_CLICKED, reinterpret_cast<void*>(static_cast<uintptr_t>(i)) );
    }

    renderedDeviceCount = deviceCount;
}

/**
 * @brief Updates a single BLE device row.
 *
 * Only the existing label text is changed. No LVGL objects are destroyed
 * or allocated while refreshing live RSSI values.
 *
 * @param row BLE device button.
 * @param bleExplorer Reference to the BLE Explorer feature.
 * @param index Device index represented by the row.
 */
void ScreenBLEExplorer::updateDeviceRow(
    lv_obj_t* row,
    BLEExplorer& bleExplorer,
    uint8_t index
)
{
    const BLEDeviceInfo& device = bleExplorer.getDevice(index);

    String name = device.name;

    if (name.length() == 0)
        name = "< unknown >";

    if (name.length() > 20)
        name = name.substring(0, 17) + "...";

    String text = name + "\n" + String(device.rssi) + " dBm | " + device.address;

    lv_obj_t* label = lv_obj_get_child(row, 0);

    if (label != nullptr)
        lv_label_set_text(label, text.c_str());
}

/**
 * @brief Updates the BLE Explorer status label.
 *
 * @param bleExplorer Reference to the BLE Explorer feature.
 */
void ScreenBLEExplorer::updateStatus(BLEExplorer& bleExplorer)
{
    if (statusLabel == nullptr)
        return;

    char buffer[24];

    snprintf( buffer, sizeof(buffer), "%s | %u", bleExplorer.isScanning() ? "SCAN" : "IDLE", bleExplorer.getDeviceCount());

    lv_label_set_text(statusLabel, buffer);
}

/**
 * @brief Handles the SCAN/STOP button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenBLEExplorer::scanClicked(lv_event_t* event)
{
    RefreshContext* context =
        static_cast<RefreshContext*>(lv_event_get_user_data(event));

    if (context == nullptr ||
        context->bleExplorer == nullptr)
        return;

    if (context->bleExplorer->isScanning())
        stopScan(*context->bleExplorer);
    else
        startScan(*context->bleExplorer);
}

/**
 * @brief Handles selection of a discovered BLE device.
 *
 * Copies the selected BLE device identity into BLEFoxHunt before
 * navigating away from the Explorer.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenBLEExplorer::deviceClicked(lv_event_t* event)
{
    if (refreshContext.screenManager == nullptr || refreshContext.bleExplorer == nullptr || refreshContext.bleFoxHunt == nullptr)
        return;

    uint8_t index = static_cast<uint8_t>(
        reinterpret_cast<uintptr_t>(lv_event_get_user_data(event))
    );

    BLEExplorer& bleExplorer = *refreshContext.bleExplorer;

    if (index >= bleExplorer.getDeviceCount())
        return;

    if (!bleExplorer.selectDevice(index))
        return;

    const BLEDeviceInfo& device = bleExplorer.getDevice(index);

    /*
     * Copy target information before leaving Explorer because its screen
     * cleanup releases the BLE scan cache.
     */
    String address = device.address;
    String name = device.name;

    if (name.length() == 0)
        name = "< unknown >";

    refreshContext.bleFoxHunt->setTarget(address, name);

    bleExplorer.stopScan();

    refreshContext.screenManager->show(Screen::BLEFoxHunt);
}

/**
 * @brief Handles the BLE Explorer back button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenBLEExplorer::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager =
        static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    screenManager->back();
}

/**
 * @brief Periodically refreshes BLE scan results.
 *
 * Existing rows only receive updated label text. New rows are appended
 * when additional devices are discovered.
 *
 * @param timer Pointer to the LVGL timer.
 */
void ScreenBLEExplorer::refreshTimerCallback(lv_timer_t* timer)
{
    RefreshContext* context =
        static_cast<RefreshContext*>(lv_timer_get_user_data(timer));

    if (context == nullptr || context->bleExplorer == nullptr)
        return;

    updateDevices(*context->bleExplorer);
    updateStatus(*context->bleExplorer);
}

/**
 * @brief Cleans up BLE Explorer resources when the screen is deleted.
 *
 * Shuts down the BLE subsystem, removes the refresh timer and clears all
 * stored UI references associated with the BLE Explorer screen.
 *
 * @param event Pointer to the LVGL delete event.
 */
void ScreenBLEExplorer::screenDeleted(lv_event_t* event)
{
    BLEExplorer* bleExplorer =
        static_cast<BLEExplorer*>(lv_event_get_user_data(event));

    if (bleExplorer != nullptr)
        bleExplorer->shutdown();

    if (refreshTimer != nullptr)
    {
        lv_timer_delete(refreshTimer);
        refreshTimer = nullptr;
    }

    refreshContext.screenManager = nullptr;
    refreshContext.bleExplorer = nullptr;
    refreshContext.bleFoxHunt = nullptr;

    deviceContainer = nullptr;
    scanButton = nullptr;
    statusLabel = nullptr;
    emptyLabel = nullptr;

    renderedDeviceCount = 0;
}