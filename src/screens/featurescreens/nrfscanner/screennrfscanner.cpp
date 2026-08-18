/**
 * @file screennrfscanner.cpp
 * @brief Implementation of the NRF Scanner screen.
 *
 * This file creates the NRF Scanner interface, connects user controls
 * to the NRFScanner feature and visualizes live RF activity.
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
 * @brief Timer used for continuous scanner updates.
 */
lv_timer_t* ScreenNrfScanner::updateTimer = nullptr;

/**
 * @brief Buffer containing generated NRF channel options.
 */
char ScreenNrfScanner::nrfChannelOptions[1536] = {0};

/**
 * @brief Creates the NRF Scanner screen.
 *
 * Creates scanner controls, signal visualization and navigation
 * controls and connects the screen to the NRFScanner feature.
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

    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &screenManager);

    UIWidgets::addText(screen, 15, 50, "MODE", 100);
    modeDropdown = UIWidgets::addDropdown(
        screen,
        15,
        70,
        "FULL SPECTRUM\nNRF CHANNEL\nWIFI BAND",
        100
    );

    UIWidgets::addText(screen, 125, 50, "CHANNEL", 100);
    channelDropdown = UIWidgets::addDropdown(screen, 125, 70, "0 - 125", 100);

    buildNrfChannelOptions();
    updateChannelDropdown(ScanMode::FullSpectrum);

    lv_obj_add_event_cb(modeDropdown, modeChanged, LV_EVENT_VALUE_CHANGED, &nrfScanner);
    lv_obj_add_event_cb(channelDropdown, channelChanged, LV_EVENT_VALUE_CHANGED, &nrfScanner);

    UIWidgets::addText(screen, 15, 115, "SIGNAL", 210);

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
    lv_chart_set_range(signalChart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    lv_obj_set_style_bg_color(signalChart, lv_color_hex(0x080808), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(signalChart, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(signalChart, 0, LV_PART_MAIN);
    lv_obj_set_style_line_width(signalChart, 1, LV_PART_ITEMS);

    signalSeries = lv_chart_add_series(
        signalChart,
        lv_color_hex(0xFF1744),
        LV_CHART_AXIS_PRIMARY_Y
    );

    lv_obj_t* scanButton = UIWidgets::addButton(screen, 65, 265, "SCAN", 110, 35);
    lv_obj_add_event_cb(scanButton, scanClicked, LV_EVENT_CLICKED, &nrfScanner);

    updateTimer = lv_timer_create(updateScanner, 50, &nrfScanner);

    return screen;
}

/**
 * @brief Handles changes to the scanner mode dropdown.
 *
 * Updates the NRFScanner feature and rebuilds the channel selector.
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
 * Updates the selected NRF24 or Wi-Fi channel depending on the
 * currently selected scanner mode.
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
 * @brief Handles the scan button event.
 *
 * Starts or stops continuous NRF scanning.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNrfScanner::scanClicked(lv_event_t* event)
{
    NRFScanner* nrfScanner = static_cast<NRFScanner*>(lv_event_get_user_data(event));

    if (nrfScanner == nullptr)
        return;

    if (nrfScanner->isRunning())
        nrfScanner->stop();
    else
        nrfScanner->start();
}

/**
 * @brief Periodically updates scanning and visualization.
 *
 * Calls the NRFScanner feature update method and redraws the chart
 * using the latest available scan results.
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
            lv_dropdown_set_options(channelDropdown, "0 - 125");
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
                "WIFI CH 1\n"
                "WIFI CH 2\n"
                "WIFI CH 3\n"
                "WIFI CH 4\n"
                "WIFI CH 5\n"
                "WIFI CH 6\n"
                "WIFI CH 7\n"
                "WIFI CH 8\n"
                "WIFI CH 9\n"
                "WIFI CH 10\n"
                "WIFI CH 11\n"
                "WIFI CH 12\n"
                "WIFI CH 13"
            );
            lv_dropdown_set_selected(channelDropdown, 0);
            break;
    }
}

/**
 * @brief Generates NRF channel dropdown options.
 *
 * Generates newline-separated channel names for all NRF24
 * channels ranging from 0 through 125.
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
            channel < NRFManager::NRF_CHANNEL_COUNT - 1 ? "NRF CH %u\n" : "NRF CH %u",
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
 * Updates the chart using data returned by the NRFScanner feature.
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
            const uint8_t* results = nrfScanner.getSpectrumResults();

            lv_chart_set_point_count(signalChart, NRFManager::NRF_CHANNEL_COUNT);

            for (uint16_t i = 0; i < NRFManager::NRF_CHANNEL_COUNT; i++)
                lv_chart_set_next_value(signalChart, signalSeries, results[i]);

            break;
        }

        case NRFScanner::ScanMode::NrfChannel:
        {
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
 * @brief Handles the NRF Scanner back button event.
 *
 * Stops scanning, removes the update timer and requests navigation
 * to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNrfScanner::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    if (updateTimer != nullptr)
    {
        lv_timer_delete(updateTimer);
        updateTimer = nullptr;
    }

    screenManager->back();
}