/**
 * @file screennrfscanner.cpp
 * @brief Implementation of the NRF Scanner screen.
 *
 * This file creates the NRF Scanner interface, handles scanner
 * controls and visualizes live RF activity.
 */

#include "screennrfscanner.h"

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/nrfscanner/nrfscanner.h"

/**
 * @brief Dropdown used to select the scanner operating mode.
 */
lv_obj_t* ScreenNrfScanner::modeDropdown = nullptr;

/**
 * @brief Dropdown used to select the channel or frequency range.
 */
lv_obj_t* ScreenNrfScanner::channelDropdown = nullptr;

/**
 * @brief Container used for RF signal visualization.
 */
lv_obj_t* ScreenNrfScanner::signalContainer = nullptr;

/**
 * @brief Chart used to display RF activity.
 */
lv_obj_t* ScreenNrfScanner::signalChart = nullptr;

/**
 * @brief Chart series used to draw RF activity.
 */
lv_chart_series_t* ScreenNrfScanner::signalSeries = nullptr;

/**
 * @brief Chart series used to draw peak RF activity for peak holding.
 */
lv_chart_series_t* ScreenNrfScanner::peakSeries = nullptr;

/**
 * @brief Timer used to update scanner state and visualization.
 */
lv_timer_t* ScreenNrfScanner::updateTimer = nullptr;

/**
 * @brief Context used by the back navigation button.
 */
ScreenNrfScanner::BackContext ScreenNrfScanner::backContext = {nullptr, nullptr};

/**
 * @brief Buffer containing generated NRF channel options.
 */
char ScreenNrfScanner::nrfChannelOptions[1536] = {0};

/**
 * @brief Creates the NRF Scanner screen.
 *
 * Creates scanner configuration controls, live RF visualization
 * and explicit start and stop controls.
 *
 * @param screenManager Reference to the application ScreenManager.
 * @param nrfScanner Reference to the NRFScanner feature.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenNrfScanner::create(ScreenManager& screenManager, NRFScanner& nrfScanner)
{
    lv_obj_t* screen = UIWidgets::createScreen();

    UIWidgets::addHeader(screen, 0, 0, "NRF SC4NNER");

    backContext.screenManager = &screenManager;
    backContext.nrfScanner = &nrfScanner;

    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &backContext);

    UIWidgets::addText(screen, 15, 50, "MODE", 100);
    modeDropdown = UIWidgets::addDropdown( screen, 15, 70, "FULL\nNRF\nWIFI", 100 );

    UIWidgets::addText(screen, 125, 50, "CHANNEL", 100);
    channelDropdown = UIWidgets::addDropdown(screen, 125, 70, "0 - 125", 100);

    buildNrfChannelOptions();
    updateChannelDropdown(ScanMode::FullSpectrum);

    lv_obj_add_event_cb(modeDropdown, modeChanged, LV_EVENT_VALUE_CHANGED, &nrfScanner);
    lv_obj_add_event_cb(channelDropdown, channelChanged, LV_EVENT_VALUE_CHANGED, &nrfScanner);

    UIWidgets::addText(screen, 15, 115, "SIGNAL", 210);

    //Since I think we will never need this, this is not a uiwidet

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
    lv_chart_set_point_count(signalChart, NRFManager::NRF_CHANNEL_COUNT);
    lv_chart_set_range(signalChart, LV_CHART_AXIS_PRIMARY_Y, 0, 25);

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

    //peakhold series in grey
        peakSeries = lv_chart_add_series(
        signalChart,
        lv_color_hex(0x808080),
        LV_CHART_AXIS_PRIMARY_Y
    );

    lv_obj_t* startButton = UIWidgets::addButton(screen, 15, 265, "START", 100, 35);
    lv_obj_add_event_cb(startButton, startClicked, LV_EVENT_CLICKED, &nrfScanner);

    lv_obj_t* stopButton = UIWidgets::addButton(screen, 125, 265, "STOP", 100, 35);
    lv_obj_add_event_cb(stopButton, stopClicked, LV_EVENT_CLICKED, &nrfScanner);

    lv_obj_t* clearButton = UIWidgets::addButton(screen, 175, 120, "CLR", 50, 25);
    lv_obj_add_event_cb(clearButton, clearClicked, LV_EVENT_CLICKED, &nrfScanner);

    updateTimer = lv_timer_create(updateScanner, 20, &nrfScanner);

    return screen;
}

/**
 * @brief Handles changes to the scanner mode dropdown.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNrfScanner::modeChanged(lv_event_t* event)
{
    NRFScanner* nrfScanner = static_cast<NRFScanner*>(lv_event_get_user_data(event));

    if (nrfScanner == nullptr)
        return;

    lv_obj_t* dropdown = static_cast<lv_obj_t*>(lv_event_get_target(event));
    ScanMode mode = static_cast<ScanMode>(lv_dropdown_get_selected(dropdown));

    nrfScanner->setMode(static_cast<NRFScanner::ScanMode>(mode));
    updateChannelDropdown(mode);
}

/**
 * @brief Handles changes to the channel dropdown.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNrfScanner::channelChanged(lv_event_t* event)
{
    NRFScanner* nrfScanner = static_cast<NRFScanner*>(lv_event_get_user_data(event));

    if (nrfScanner == nullptr)
        return;

    uint16_t selected = lv_dropdown_get_selected(channelDropdown);

    switch (nrfScanner->getMode())
    {
        case NRFScanner::ScanMode::FullSpectrum:
            break;

        case NRFScanner::ScanMode::NrfChannel:
            nrfScanner->setNrfChannel(static_cast<uint8_t>(selected));
            break;

        case NRFScanner::ScanMode::WifiBand:
            nrfScanner->setWifiChannel(static_cast<uint8_t>(selected + 1));
            break;
    }
}

/**
 * @brief Handles the scanner start button event.
 *
 * Starts continuous RF scanning.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNrfScanner::startClicked(lv_event_t* event)
{
    NRFScanner* nrfScanner = static_cast<NRFScanner*>(lv_event_get_user_data(event));

    if (nrfScanner == nullptr)
        return;

    nrfScanner->start();
}

/**
 * @brief Handles the scanner stop button event.
 *
 * Stops continuous RF scanning while keeping the latest
 * scan results visible.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNrfScanner::stopClicked(lv_event_t* event)
{
    NRFScanner* nrfScanner = static_cast<NRFScanner*>(lv_event_get_user_data(event));

    if (nrfScanner == nullptr)
        return;

    nrfScanner->stop();
}

/**
 * @brief Periodically updates scanner state and visualization.
 *
 * Performs a small scanner update and renders the latest results.
 *
 * @param timer Pointer to the LVGL timer.
 */
void ScreenNrfScanner::updateScanner(lv_timer_t* timer)
{
    NRFScanner* nrfScanner = static_cast<NRFScanner*>(lv_timer_get_user_data(timer));

    if (nrfScanner == nullptr)
        return;

    nrfScanner->update();

    if (nrfScanner->isRunning())
        renderResults(*nrfScanner);
}

/**
 * @brief Updates the channel dropdown for the selected scan mode.
 *
 * @param mode Selected scanner operating mode.
 */
void ScreenNrfScanner::updateChannelDropdown(ScanMode mode)
{
    switch (mode)
    {
        case ScanMode::FullSpectrum:
            lv_dropdown_set_options(channelDropdown, "0-125");
            lv_dropdown_set_selected(channelDropdown, 0);
            lv_obj_add_state(channelDropdown, LV_STATE_DISABLED);
            break;

        case ScanMode::NrfChannel:
            lv_obj_clear_state(channelDropdown, LV_STATE_DISABLED);
            lv_dropdown_set_options(channelDropdown, nrfChannelOptions);
            lv_dropdown_set_selected(channelDropdown, 0);
            break;

        case ScanMode::WifiBand:
            lv_obj_clear_state(channelDropdown, LV_STATE_DISABLED);
            lv_dropdown_set_options(
                channelDropdown,
                "CH 1\n"
                "CH 2\n"
                "CH 3\n"
                "CH 4\n"
                "CH 5\n"
                "CH 6\n"
                "CH 7\n"
                "CH 8\n"
                "CH 9\n"
                "CH 10\n"
                "CH 11\n"
                "CH 12\n"
                "CH 13"
            );
            lv_dropdown_set_selected(channelDropdown, 0);
            break;
    }
}

/**
 * @brief Generates NRF channel dropdown options.
 *
 * Generates newline-separated channel labels for NRF24
 * channels 0 through 125.
 */
void ScreenNrfScanner::buildNrfChannelOptions()
{
    nrfChannelOptions[0] = '\0';

    size_t offset = 0;

    for (uint16_t channel = 0; channel < NRFManager::NRF_CHANNEL_COUNT; channel++)
    {
        int written = snprintf(
            nrfChannelOptions + offset,
            sizeof(nrfChannelOptions) - offset,
            channel < NRFManager::NRF_CHANNEL_COUNT - 1 ? "CH %u\n" : "CH %u",
            channel
        );

        if (written <= 0)
            break;

        offset += static_cast<size_t>(written);

        if (offset >= sizeof(nrfChannelOptions))
            break;
    }
}

/**
 * @brief Renders the latest scanner results.
 *
 * Full spectrum and Wi-Fi modes render fixed frequency ranges.
 * NRF channel mode renders a scrolling activity history.
 *
 * @param nrfScanner Reference to the NRFScanner feature.
 */
void ScreenNrfScanner::renderResults(NRFScanner& nrfScanner)
{
    if (signalChart == nullptr || signalSeries == nullptr)
        return;

    switch (nrfScanner.getMode())
    {
        case NRFScanner::ScanMode::FullSpectrum:
        {
            lv_chart_hide_series(signalChart, peakSeries, false);

            const uint8_t* results = nrfScanner.getSpectrumResults();
            const uint8_t* peaks = nrfScanner.getSpectrumPeaks();

            lv_chart_set_point_count(signalChart, NRFManager::NRF_CHANNEL_COUNT);

            for (uint16_t i = 0; i < NRFManager::NRF_CHANNEL_COUNT; i++)
            {
                lv_chart_set_next_value(signalChart, signalSeries, results[i]);
                lv_chart_set_next_value(signalChart, peakSeries, peaks[i]);
            }

            break;
        }

        case NRFScanner::ScanMode::NrfChannel:
        {
            lv_chart_hide_series(signalChart, peakSeries, true);

            lv_chart_set_point_count(signalChart, 64);

            lv_chart_set_next_value(
                signalChart,
                signalSeries,
                nrfScanner.getNrfChannelActivity()
            );

            break;
        }

        case NRFScanner::ScanMode::WifiBand:
        {
            lv_chart_hide_series(signalChart, peakSeries, true);

            const uint8_t* results = nrfScanner.getWifiResults();

            lv_chart_set_point_count(signalChart, NRFManager::WIFI_SCAN_WIDTH);

            for (uint8_t i = 0; i < NRFManager::WIFI_SCAN_WIDTH; i++)
                lv_chart_set_next_value(signalChart, signalSeries, results[i]);

            break;
        }
    }

    lv_chart_refresh(signalChart);
}

/**
 * @brief Handles the peak clear button event.
 *
 * Clears all stored spectrum peak values.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNrfScanner::clearClicked(lv_event_t* event)
{
    NRFScanner* nrfScanner = static_cast<NRFScanner*>(lv_event_get_user_data(event));

    if (nrfScanner == nullptr)
        return;

    nrfScanner->clearPeaks();
}

/**
 * @brief Handles the NRF Scanner back button event.
 *
 * Stops active scanning, removes the update timer and navigates
 * back to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNrfScanner::backClicked(lv_event_t* event)
{
    BackContext* context = static_cast<BackContext*>(lv_event_get_user_data(event));

    if (context == nullptr || context->screenManager == nullptr || context->nrfScanner == nullptr)
        return;

    context->nrfScanner->stop();

    if (updateTimer != nullptr)
    {
        lv_timer_delete(updateTimer);
        updateTimer = nullptr;
    }

    context->screenManager->back();
}