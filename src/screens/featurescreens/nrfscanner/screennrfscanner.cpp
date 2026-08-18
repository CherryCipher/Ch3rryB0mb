/**
 * @file screennrfscanner.cpp
 * @brief Implementation of the NRF Scanner screen.
 *
 * This file creates the NRF Scanner interface and handles
 * screen configuration and navigation events.
 */

#include "screennrfscanner.h"

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"

/**
 * @brief Dropdown used to select the scanner operating mode.
 */
lv_obj_t* ScreenNrfScanner::modeDropdown = nullptr;

/**
 * @brief Dropdown used to select the channel or frequency range.
 */
lv_obj_t* ScreenNrfScanner::channelDropdown = nullptr;

/**
 * @brief Container used for live RF signal visualization.
 */
lv_obj_t* ScreenNrfScanner::signalContainer = nullptr;

/**
 * @brief Chart used to display live RF activity.
 */
lv_obj_t* ScreenNrfScanner::signalChart = nullptr;

/**
 * @brief Buffer containing generated NRF channel options.
 */
char ScreenNrfScanner::nrfChannelOptions[1536] = {0};

/**
 * @brief Creates the NRF Scanner screen.
 *
 * Creates the scanner mode controls, channel selector and signal
 * visualization area.
 *
 * @param screenManager Reference to the application ScreenManager.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenNrfScanner::create(ScreenManager& screenManager)
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

    lv_obj_add_event_cb(modeDropdown, modeChanged, LV_EVENT_VALUE_CHANGED, nullptr);

    buildNrfChannelOptions();
    updateChannelDropdown(ScanMode::FullSpectrum);

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
    lv_chart_set_point_count(signalChart, 126);
    lv_chart_set_range(signalChart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    lv_obj_set_style_bg_color(signalChart, lv_color_hex(0x080808), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(signalChart, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(signalChart, 0, LV_PART_MAIN);
    lv_obj_set_style_line_width(signalChart, 1, LV_PART_ITEMS);

    return screen;
}

/**
 * @brief Handles changes to the scanner mode dropdown.
 *
 * Reads the selected mode and updates the channel dropdown
 * with the appropriate channel options.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNrfScanner::modeChanged(lv_event_t* event)
{
    lv_obj_t* dropdown = static_cast<lv_obj_t*>(lv_event_get_target(event));

    ScanMode mode = static_cast<ScanMode>(lv_dropdown_get_selected(dropdown));

    updateChannelDropdown(mode);
}

/**
 * @brief Updates the channel dropdown for the selected scan mode.
 *
 * Full spectrum mode scans all NRF channels and therefore disables
 * manual channel selection. NRF and Wi-Fi modes provide selectable
 * channel lists.
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
 * @brief Generates the NRF channel dropdown options.
 *
 * Generates newline-separated channel names for all 126 NRF24
 * radio channels ranging from channel 0 through channel 125.
 */
void ScreenNrfScanner::buildNrfChannelOptions()
{
    nrfChannelOptions[0] = '\0';

    size_t offset = 0;

    for (uint16_t channel = 0; channel <= 125; channel++)
    {
        int written = snprintf(
            nrfChannelOptions + offset,
            sizeof(nrfChannelOptions) - offset,
            channel < 125 ? "NRF CH %u\n" : "NRF CH %u",
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
 * @brief Handles the NRF Scanner back button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNrfScanner::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    screenManager->back();
}