/**
 * @file screencc1101scanner.cpp
 * @brief Implementation of the CC1101 Sub-GHz Scanner screen.
 *
 * This file creates the Sub-GHz Scanner interface, handles scanner
 * controls and visualizes continuous RSSI spectrum measurements.
 */

#include "screencc1101scanner.h"

#include <cstdlib>

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/cc1101scanner/cc1101scanner.h"
#include "services/cc1101/cc1101manager.h"

/**
 * @brief Input field used to configure the center frequency.
 */
lv_obj_t* ScreenCC1101Scanner::frequencyInput = nullptr;

/**
 * @brief Dropdown used to select the frequency scan range.
 */
lv_obj_t* ScreenCC1101Scanner::rangeDropdown = nullptr;

/**
 * @brief On-screen keyboard used for frequency input.
 */
lv_obj_t* ScreenCC1101Scanner::keyboard = nullptr;

/**
 * @brief Container used for Sub-GHz signal visualization.
 */
lv_obj_t* ScreenCC1101Scanner::signalContainer = nullptr;

/**
 * @brief Chart used to display RSSI measurements.
 */
lv_obj_t* ScreenCC1101Scanner::signalChart = nullptr;

/**
 * @brief Chart series used to draw RSSI measurements.
 */
lv_chart_series_t* ScreenCC1101Scanner::signalSeries = nullptr;

/**
 * @brief Timer used to schedule individual spectrum measurements.
 */
lv_timer_t* ScreenCC1101Scanner::updateTimer = nullptr;

/**
 * @brief Context used by the back navigation button.
 */
ScreenCC1101Scanner::BackContext ScreenCC1101Scanner::backContext = {nullptr, nullptr};

/**
 * @brief Creates the Sub-GHz Scanner screen.
 *
 * Creates center frequency and scan range controls, RSSI visualization,
 * scanner controls and the timer used for incremental RF measurements.
 *
 * @param screenManager Reference to the application ScreenManager.
 * @param cc1101Scanner Reference to the CC1101Scanner feature.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenCC1101Scanner::create(ScreenManager& screenManager, CC1101Scanner& cc1101Scanner)
{
    lv_obj_t* screen = UIWidgets::createScreen();
    UIWidgets::addHeader(screen, 0, 0, "SUB-GHZ SC4N");

    backContext.screenManager = &screenManager;
    backContext.cc1101Scanner = &cc1101Scanner;

    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &backContext);

    UIWidgets::addText(screen, 15, 50, "CENTER MHZ", 100);
    frequencyInput = UIWidgets::addInput(screen, 15, 70, "433.92", 100);
    lv_obj_add_event_cb(frequencyInput, inputFocused, LV_EVENT_FOCUSED, nullptr);

    UIWidgets::addText(screen, 125, 50, "RANGE", 100);
    rangeDropdown = UIWidgets::addDropdown(
        screen,
        125,
        70,
        "+/- 0.1\n"
        "+/- 0.5\n"
        "+/- 1.0\n"
        "+/- 2.0\n"
        "+/- 5.0",
        100
    );
    lv_dropdown_set_selected(rangeDropdown, 1);

    UIWidgets::addText(screen, 15, 115, "SIGNAL", 210);

    // The signal visualization is specific to this screen and is created
    // directly with LVGL instead of using a generic UIWidget.
    signalContainer = lv_obj_create(screen);
    lv_obj_set_pos(signalContainer, 15, 135);
    lv_obj_set_size(signalContainer, 210, 115);
    lv_obj_set_style_bg_color(signalContainer, lv_color_hex(0x080808), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(signalContainer, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(signalContainer, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(signalContainer, lv_color_hex(0xFF1744), LV_PART_MAIN);
    lv_obj_set_style_radius(signalContainer, 3, LV_PART_MAIN);
    lv_obj_set_style_pad_all(signalContainer, 5, LV_PART_MAIN);
    lv_obj_clear_flag(signalContainer, LV_OBJ_FLAG_SCROLLABLE);

    signalChart = lv_chart_create(signalContainer);
    lv_obj_set_size(signalChart, 198, 103);
    lv_obj_center(signalChart);

    lv_chart_set_type(signalChart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(signalChart, CC1101Manager::SCAN_POINT_COUNT);
    lv_chart_set_range(signalChart, LV_CHART_AXIS_PRIMARY_Y, -115, -60);

    lv_obj_set_style_line_color(signalChart, lv_color_hex(0x404040), LV_PART_MAIN);
    lv_obj_set_style_bg_color(signalChart, lv_color_hex(0x080808), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(signalChart, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(signalChart, 0, LV_PART_MAIN);
    lv_obj_set_style_line_width(signalChart, 1, LV_PART_ITEMS);

    signalSeries = lv_chart_add_series(
        signalChart,
        lv_color_hex(0xFF8C00),
        LV_CHART_AXIS_PRIMARY_Y
    );

    lv_obj_t* startButton = UIWidgets::addButton(screen, 15, 265, "START", 100, 35);
    lv_obj_add_event_cb(startButton, startClicked, LV_EVENT_CLICKED, &cc1101Scanner);

    lv_obj_t* stopButton = UIWidgets::addButton(screen, 125, 265, "STOP", 100, 35);
    lv_obj_add_event_cb(stopButton, stopClicked, LV_EVENT_CLICKED, &cc1101Scanner);

    // Create the on-screen keyboard used by the center frequency input.
    keyboard = UIWidgets::addKeyboard(screen);
    lv_obj_add_event_cb(keyboard, keyboardFinished, LV_EVENT_READY, nullptr);
    lv_obj_add_event_cb(keyboard, keyboardFinished, LV_EVENT_CANCEL, nullptr);

    // Each timer callback measures only one frequency. A short interval
    // keeps scanning fast while returning control to LVGL between points.
    updateTimer = lv_timer_create(updateTimerCallback, 5, &cc1101Scanner);
    lv_timer_pause(updateTimer);

    return screen;
}

/**
 * @brief Handles the scanner start button event.
 *
 * Reads the center frequency and selected scan range, configures the
 * scanner and starts incremental spectrum scanning.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenCC1101Scanner::startClicked(lv_event_t* event)
{
    CC1101Scanner* cc1101Scanner =
        static_cast<CC1101Scanner*>(lv_event_get_user_data(event));

    if (cc1101Scanner == nullptr) return;

    const float centerMHz = atof(lv_textarea_get_text(frequencyInput));
    if (centerMHz <= 0.0f) return;

    // Restart scanning when START is pressed so changed frequency or
    // range settings are applied immediately.
    if (cc1101Scanner->isRunning())
        cc1101Scanner->stop();

    cc1101Scanner->setCenterFrequency(centerMHz);
    cc1101Scanner->setRange(
        getSelectedRange(lv_dropdown_get_selected(rangeDropdown))
    );

    if (!cc1101Scanner->start()) return;

    if (updateTimer != nullptr)
    {
        lv_timer_reset(updateTimer);
        lv_timer_resume(updateTimer);
    }
}

/**
 * @brief Handles the scanner stop button event.
 *
 * Stops spectrum scanning and pauses the incremental update timer.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenCC1101Scanner::stopClicked(lv_event_t* event)
{
    CC1101Scanner* cc1101Scanner =
        static_cast<CC1101Scanner*>(lv_event_get_user_data(event));

    if (cc1101Scanner == nullptr) return;

    cc1101Scanner->stop();

    if (updateTimer != nullptr)
        lv_timer_pause(updateTimer);
}

/**
 * @brief Handles periodic spectrum scan updates.
 *
 * Performs exactly one frequency measurement per callback. When all
 * scan points have been measured, the completed sweep is rendered.
 *
 * Returning after every individual RF measurement prevents a complete
 * spectrum sweep from blocking LVGL and keeps touch controls responsive.
 *
 * @param timer Pointer to the LVGL timer.
 */
void ScreenCC1101Scanner::updateTimerCallback(lv_timer_t* timer)
{
    CC1101Scanner* cc1101Scanner =
        static_cast<CC1101Scanner*>(lv_timer_get_user_data(timer));

    if (cc1101Scanner == nullptr || !cc1101Scanner->isRunning()) return;

    if (cc1101Scanner->update())
        renderResults(*cc1101Scanner);
}

/**
 * @brief Renders the latest complete CC1101 spectrum sweep.
 *
 * Copies all RSSI measurements into their corresponding chart points
 * and refreshes the chart.
 *
 * @param cc1101Scanner Reference to the CC1101Scanner feature.
 */
void ScreenCC1101Scanner::renderResults(CC1101Scanner& cc1101Scanner)
{
    if (signalChart == nullptr || signalSeries == nullptr) return;

    const int16_t* results = cc1101Scanner.getResults();
    const uint8_t resultCount = cc1101Scanner.getResultCount();

    for (uint8_t i = 0; i < resultCount; i++)
        lv_chart_set_value_by_id(signalChart, signalSeries, i, results[i]);

    lv_chart_refresh(signalChart);
}

/**
 * @brief Converts the selected range option into MHz.
 *
 * @param selected Selected range dropdown index.
 *
 * @return Frequency range on either side of the center frequency.
 */
float ScreenCC1101Scanner::getSelectedRange(uint16_t selected)
{
    switch (selected)
    {
        case 0: return 0.1f;
        case 1: return 0.5f;
        case 2: return 1.0f;
        case 3: return 2.0f;
        case 4: return 5.0f;
        default: return 0.5f;
    }
}

/**
 * @brief Handles focus events for the center frequency input.
 *
 * Connects the on-screen keyboard to the focused frequency input
 * and makes the keyboard visible.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenCC1101Scanner::inputFocused(lv_event_t* event)
{
    if (keyboard == nullptr) return;

    lv_obj_t* input =
        static_cast<lv_obj_t*>(lv_event_get_target(event));

    if (input == nullptr) return;

    lv_keyboard_set_textarea(keyboard, input);
    lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief Handles completion or cancellation of keyboard input.
 *
 * Disconnects the keyboard from the active input field and hides it.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenCC1101Scanner::keyboardFinished(lv_event_t* event)
{
    if (keyboard == nullptr) return;

    lv_keyboard_set_textarea(keyboard, nullptr);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief Handles the Sub-GHz Scanner back button event.
 *
 * Stops active scanning, pauses periodic updates and navigates back
 * to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenCC1101Scanner::backClicked(lv_event_t* event)
{
    BackContext* context =
        static_cast<BackContext*>(lv_event_get_user_data(event));

    if (context == nullptr ||
        context->screenManager == nullptr ||
        context->cc1101Scanner == nullptr)
        return;

    context->cc1101Scanner->stop();

    if (updateTimer != nullptr)
        lv_timer_pause(updateTimer);

    context->screenManager->back();
}