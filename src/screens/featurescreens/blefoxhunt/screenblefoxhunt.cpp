/**
 * @file screenblefoxhunt.cpp
 * @brief Implementation of the BLE Fox Hunt screen.
 */

#include "screenblefoxhunt.h"

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/blefoxhunt/blefoxhunt.h"

ScreenBLEFoxHunt::ScreenContext ScreenBLEFoxHunt::screenContext;

lv_obj_t* ScreenBLEFoxHunt::targetNameLabel = nullptr;
lv_obj_t* ScreenBLEFoxHunt::targetAddressLabel = nullptr;
lv_obj_t* ScreenBLEFoxHunt::rssiLabel = nullptr;
lv_obj_t* ScreenBLEFoxHunt::peakLabel = nullptr;
lv_obj_t* ScreenBLEFoxHunt::signalLabel = nullptr;
lv_obj_t* ScreenBLEFoxHunt::lastSeenLabel = nullptr;
lv_obj_t* ScreenBLEFoxHunt::signalBar = nullptr;
lv_timer_t* ScreenBLEFoxHunt::updateTimer = nullptr;

bool ScreenBLEFoxHunt::huntStarted = false;

/**
 * @brief Creates the BLE Fox Hunt screen.
 */
lv_obj_t* ScreenBLEFoxHunt::create(ScreenManager& screenManager, BLEFoxHunt& bleFoxHunt)
{
    lv_obj_t* screen = UIWidgets::createScreen();

    screenContext.screenManager = &screenManager;
    screenContext.bleFoxHunt = &bleFoxHunt;

    UIWidgets::addHeader(screen, 0, 0, "BLE F0X HUNT");

    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &screenContext);

    targetNameLabel = UIWidgets::addText( screen, 15, 50, bleFoxHunt.getTargetName().c_str(), 210 );

    targetAddressLabel = UIWidgets::addText( screen, 15, 72, bleFoxHunt.getTargetAddress().c_str(), 210 );

    UIWidgets::addText(screen, 15, 110, "SIGNAL", 80);

    signalLabel = UIWidgets::addText(screen, 100, 110, "SEARCHING", 125);

    rssiLabel = UIWidgets::addText(screen, 15, 140, "RSSI: --- dBm", 120);
    peakLabel = UIWidgets::addText(screen, 135, 140, "PEAK: ---", 95);

    signalBar = lv_bar_create(screen);
    lv_obj_set_pos(signalBar, 15, 170);
    lv_obj_set_size(signalBar, 210, 28);

    lv_bar_set_range(signalBar, 0, 100);
    lv_bar_set_value(signalBar, 0, LV_ANIM_OFF);

    lv_obj_set_style_bg_color(signalBar, lv_color_hex(0x080808), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(signalBar, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(signalBar, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(signalBar, lv_color_hex(0xFF1744), LV_PART_MAIN);
    lv_obj_set_style_radius(signalBar, 3, LV_PART_MAIN);

    lv_obj_set_style_bg_color(signalBar, lv_color_hex(0xFF8C00), LV_PART_INDICATOR);
    lv_obj_set_style_radius(signalBar, 2, LV_PART_INDICATOR);

    lastSeenLabel = UIWidgets::addText(screen, 15, 215, "LAST SEEN: searching...", 210);

    UIWidgets::addText( screen, 15, 250, "Go find the target\nRSSI 0 dBm == found!", 210 );

    huntStarted = false;

    /*
     * BLE is deliberately not started directly inside create().
     *
     * ScreenManager creates the new screen before deleting the old one.
     * The Explorer therefore still owns the BLE subsystem at this point.
     *
     * Starting from the timer allows the Explorer delete callback to
     * finish shutting down NimBLE before Fox Hunt initializes it again.
     */
    updateTimer = lv_timer_create(updateTimerCallback, 250, &screenContext);

    lv_obj_add_event_cb(screen, screenDeleted, LV_EVENT_DELETE, &bleFoxHunt);

    return screen;
}

/**
 * @brief Refreshes all Fox Hunt UI values.
 */
void ScreenBLEFoxHunt::render(BLEFoxHunt& bleFoxHunt)
{
    if (rssiLabel == nullptr ||
        peakLabel == nullptr ||
        signalLabel == nullptr ||
        lastSeenLabel == nullptr ||
        signalBar == nullptr)
        return;

    char buffer[48];

    int8_t rssi = bleFoxHunt.getRSSI();
    int8_t peak = bleFoxHunt.getPeakRSSI();

    if (rssi == -127)
        lv_label_set_text(rssiLabel, "RSSI: --- dBm");
    else
    {
        snprintf(buffer, sizeof(buffer), "RSSI: %d dBm", rssi);
        lv_label_set_text(rssiLabel, buffer);
    }

    if (peak == -127)
        lv_label_set_text(peakLabel, "PEAK: ---");
    else
    {
        snprintf(buffer, sizeof(buffer), "PEAK: %d", peak);
        lv_label_set_text(peakLabel, buffer);
    }

    lv_label_set_text(signalLabel, bleFoxHunt.getSignalText());

    lv_bar_set_value( signalBar, bleFoxHunt.getSignalPercent(), LV_ANIM_OFF );

    uint32_t lastSeen = bleFoxHunt.getLastSeen();

    if (lastSeen == 0)
    {
        lv_label_set_text(lastSeenLabel, "LAST SEEN: searching...");
        return;
    }

    uint32_t ageMs = millis() - lastSeen;

    if (ageMs < 1000)
        lv_label_set_text(lastSeenLabel, "LAST SEEN: now");
    else
    {
        snprintf( buffer, sizeof(buffer), "LAST SEEN: %lu.%lus ago", static_cast<unsigned long>(ageMs / 1000), static_cast<unsigned long>((ageMs % 1000) / 100) );
        lv_label_set_text(lastSeenLabel, buffer);
    }
}

/**
 * @brief Handles periodic Fox Hunt updates.
 *
 * @param timer Pointer to the LVGL timer.
 */
void ScreenBLEFoxHunt::updateTimerCallback(lv_timer_t* timer)
{
    ScreenContext* context =
        static_cast<ScreenContext*>(lv_timer_get_user_data(timer));

    if (context == nullptr || context->bleFoxHunt == nullptr)
        return;

    BLEFoxHunt& bleFoxHunt = *context->bleFoxHunt;

    if (!huntStarted)
    {
        huntStarted = true;

        if (!bleFoxHunt.start())
        {
            if (signalLabel != nullptr)
                lv_label_set_text(signalLabel, "SCAN FAILED");

            return;
        }
    }

    bleFoxHunt.update();
    render(bleFoxHunt);
}

/**
 * @brief Handles the Fox Hunt back button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenBLEFoxHunt::backClicked(lv_event_t* event)
{
    ScreenContext* context =
        static_cast<ScreenContext*>(lv_event_get_user_data(event));

    if (context == nullptr || context->screenManager == nullptr || context->bleFoxHunt == nullptr)
        return;

    context->bleFoxHunt->stop();
    context->screenManager->back();
}

/**
 * @brief Cleans up Fox Hunt resources when the screen is deleted.
 *
 * Shuts down the BLE subsystem, removes the periodic update timer and
 * clears all stored UI references.
 *
 * @param event Pointer to the LVGL delete event.
 */
void ScreenBLEFoxHunt::screenDeleted(lv_event_t* event)
{
    BLEFoxHunt* bleFoxHunt =
        static_cast<BLEFoxHunt*>(lv_event_get_user_data(event));

    if (bleFoxHunt != nullptr)
        bleFoxHunt->shutdown();

    if (updateTimer != nullptr)
    {
        lv_timer_delete(updateTimer);
        updateTimer = nullptr;
    }

    screenContext.screenManager = nullptr;
    screenContext.bleFoxHunt = nullptr;

    targetNameLabel = nullptr;
    targetAddressLabel = nullptr;
    rssiLabel = nullptr;
    peakLabel = nullptr;
    signalLabel = nullptr;
    lastSeenLabel = nullptr;
    signalBar = nullptr;

    huntStarted = false;
}