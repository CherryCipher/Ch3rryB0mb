/**
 * @file screenbleexplorer.cpp
 * @brief Implementation of the BLE Explorer screen.
 */

#include "screenbleexplorer.h"

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/bleexplorer/bleexplorer.h"

lv_obj_t* ScreenBLEExplorer::deviceContainer = nullptr;
lv_obj_t* ScreenBLEExplorer::scanButton = nullptr;
lv_obj_t* ScreenBLEExplorer::statusLabel = nullptr;
lv_timer_t* ScreenBLEExplorer::refreshTimer = nullptr;
ScreenBLEExplorer::RefreshContext ScreenBLEExplorer::refreshContext;
uint8_t ScreenBLEExplorer::lastDeviceCount = 0;

/**
 * @brief Creates the BLE Explorer screen.
 */
lv_obj_t* ScreenBLEExplorer::create(ScreenManager& screenManager, BLEExplorer& bleExplorer)
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

    lastDeviceCount = 0;

    refreshTimer = lv_timer_create(refreshTimerCallback, 1000, &refreshContext);

    lv_obj_add_event_cb(screen, screenDeleted, LV_EVENT_DELETE, &bleExplorer);

    updateStatus(bleExplorer);

    return screen;
}

/**
 * @brief Starts BLE scanning.
 */
void ScreenBLEExplorer::startScan(ScreenManager& screenManager, BLEExplorer& bleExplorer)
{
    if (!bleExplorer.startScan())
    {
        if (deviceContainer != nullptr)
        {
            lv_obj_clean(deviceContainer);
            UIWidgets::addText(deviceContainer, 5, 5, "BLE scan failed.", 195);
        }

        updateStatus(bleExplorer);
        return;
    }

    lastDeviceCount = 0;

    if (deviceContainer != nullptr)
    {
        lv_obj_clean(deviceContainer);
        UIWidgets::addText(deviceContainer, 5, 5, "Scanning for BLE devices...", 195);
    }

    if (scanButton != nullptr)
    {
        lv_obj_t* label = lv_obj_get_child(scanButton, 0);

        if (label != nullptr)
            lv_label_set_text(label, "STOP");
    }

    updateStatus(bleExplorer);

    (void)screenManager;
}

/**
 * @brief Stops BLE scanning.
 */
void ScreenBLEExplorer::stopScan(ScreenManager& screenManager, BLEExplorer& bleExplorer)
{
    bleExplorer.stopScan();

    if (scanButton != nullptr)
    {
        lv_obj_t* label = lv_obj_get_child(scanButton, 0);

        if (label != nullptr)
            lv_label_set_text(label, "SCAN");
    }

    renderDevices(screenManager, bleExplorer);
    updateStatus(bleExplorer);
}

/**
 * @brief Rebuilds the displayed BLE device list.
 */
void ScreenBLEExplorer::renderDevices(ScreenManager& screenManager, BLEExplorer& bleExplorer)
{
    if (deviceContainer == nullptr)
        return;

    lv_obj_clean(deviceContainer);

    uint8_t deviceCount = bleExplorer.getDeviceCount();

    if (deviceCount == 0)
    {
        UIWidgets::addText(
            deviceContainer,
            5,
            5,
            bleExplorer.isScanning() ? "Scanning for BLE devices..." : "No BLE devices found.",
            195
        );

        return;
    }

    for (uint8_t i = 0; i < deviceCount; i++)
    {
        const BLEDeviceInfo& device = bleExplorer.getDevice(i);

        String name = device.name;

        if (name.length() == 0)
            name = "< unknown >";

        if (name.length() > 20)
            name = name.substring(0, 17) + "...";

        String text =
            name + "\n" +
            String(device.rssi) + " dBm | " +
            device.address;

        lv_obj_t* deviceButton = UIWidgets::addButton(
            deviceContainer,
            5,
            5 + (i * 58),
            text.c_str(),
            195,
            52
        );

        DeviceClickContext* context = new DeviceClickContext();

        if (context == nullptr)
            continue;

        context->screenManager = &screenManager;
        context->bleExplorer = &bleExplorer;
        context->index = i;

        lv_obj_add_event_cb(deviceButton, deviceClicked, LV_EVENT_CLICKED, context);
        lv_obj_add_event_cb(deviceButton, deviceDeleted, LV_EVENT_DELETE, context);
    }
}

/**
 * @brief Updates the BLE Explorer status label.
 */
void ScreenBLEExplorer::updateStatus(BLEExplorer& bleExplorer)
{
    if (statusLabel == nullptr)
        return;

    String text =
        String(bleExplorer.isScanning() ? "SCAN" : "IDLE") +
        " | " +
        String(bleExplorer.getDeviceCount());

    lv_label_set_text(statusLabel, text.c_str());
}

/**
 * @brief Handles the SCAN/STOP button.
 */
void ScreenBLEExplorer::scanClicked(lv_event_t* event)
{
    RefreshContext* context = static_cast<RefreshContext*>(lv_event_get_user_data(event));

    if (context == nullptr || context->screenManager == nullptr || context->bleExplorer == nullptr)
        return;

    if (context->bleExplorer->isScanning())
        stopScan(*context->screenManager, *context->bleExplorer);
    else
        startScan(*context->screenManager, *context->bleExplorer);
}

/**
 * @brief Handles selection of a discovered BLE device.
 */
void ScreenBLEExplorer::deviceClicked(lv_event_t* event)
{
    DeviceClickContext* context = static_cast<DeviceClickContext*>(lv_event_get_user_data(event));

    if (context == nullptr || context->bleExplorer == nullptr || context->screenManager == nullptr)
        return;

    BLEExplorer& bleExplorer = *context->bleExplorer;

    if (!bleExplorer.selectDevice(context->index))
        return;

    const BLEDeviceInfo& device = bleExplorer.getDevice(context->index);

    String name = device.name;

    if (name.length() == 0)
        name = "< unknown >";

    String address = device.address;
    int8_t rssi = device.rssi;

    bleExplorer.stopScan();

    /*
     * The selected address is stored inside BLEExplorer.
     *
     * BLE Fox Hunt will use:
     * bleExplorer.getSelectedAddress()
     *
     * Navigation to BLEFoxHunt will be added when that screen exists.
     */

    Serial.println(
        "[BLE] Selected: " +
        name +
        " | " +
        address +
        " | RSSI " +
        String(rssi)
    );
}

/**
 * @brief Releases a BLE device click context.
 */
void ScreenBLEExplorer::deviceDeleted(lv_event_t* event)
{
    DeviceClickContext* context = static_cast<DeviceClickContext*>(lv_event_get_user_data(event));

    delete context;
}

/**
 * @brief Handles the BLE Explorer back button.
 */
void ScreenBLEExplorer::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    screenManager->back();
}

/**
 * @brief Periodically refreshes BLE scan results.
 */
void ScreenBLEExplorer::refreshTimerCallback(lv_timer_t* timer)
{
    RefreshContext* context = static_cast<RefreshContext*>(lv_timer_get_user_data(timer));

    if (context == nullptr || context->screenManager == nullptr || context->bleExplorer == nullptr)
        return;

    uint8_t deviceCount = context->bleExplorer->getDeviceCount();

    /*
     * While scanning the complete list is refreshed so the displayed
     * RSSI values remain live.
     */
    if (context->bleExplorer->isScanning() || deviceCount != lastDeviceCount)
    {
        renderDevices(*context->screenManager, *context->bleExplorer);
        lastDeviceCount = deviceCount;
    }

    updateStatus(*context->bleExplorer);
}

/**
 * @brief Cleans up BLE Explorer resources when the screen is deleted.
 *
 * Shuts down the BLE subsystem, removes the refresh timer and clears
 * all stored UI references associated with the BLE Explorer screen.
 *
 * This ensures BLE runtime memory is released when leaving the screen
 * and prevents callbacks from referencing deleted LVGL objects.
 *
 * @param event Pointer to the LVGL delete event.
 */
void ScreenBLEExplorer::screenDeleted(lv_event_t* event)
{
    BLEExplorer* bleExplorer = static_cast<BLEExplorer*>(lv_event_get_user_data(event));

    if (bleExplorer != nullptr)
        bleExplorer->shutdown();

    if (refreshTimer != nullptr)
    {
        lv_timer_delete(refreshTimer);
        refreshTimer = nullptr;
    }

    refreshContext.screenManager = nullptr;
    refreshContext.bleExplorer = nullptr;

    deviceContainer = nullptr;
    scanButton = nullptr;
    statusLabel = nullptr;
    lastDeviceCount = 0;
}