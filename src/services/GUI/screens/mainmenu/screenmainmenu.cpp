#include "screenmainmenu.h"
#include "Services/GUI/screenmanager.h"

/**
 * @brief Creates the main menu screen.
 *
 * Creates the main Ch3rryB0mb menu and adds the currently
 * available application navigation options.
 *
 * @param screenManager Reference to the application ScreenManager.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenMainMenu::create(ScreenManager& screenManager)
{
    lv_obj_t* screen = lv_obj_create(nullptr);

    //
    // Screen background.
    //
    lv_obj_set_style_bg_color(
        screen,
        lv_color_hex(0x080808),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        screen,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        screen,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        screen,
        0,
        LV_PART_MAIN
    );

    //
    // Header.
    //
    lv_obj_t* header = lv_obj_create(screen);

    lv_obj_set_size(
        header,
        LV_PCT(100),
        40
    );

    lv_obj_align(
        header,
        LV_ALIGN_TOP_MID,
        0,
        0
    );

    lv_obj_set_style_bg_color(
        header,
        lv_color_hex(0x8A0000),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        header,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        header,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        header,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        header,
        0,
        LV_PART_MAIN
    );

    //
    // Header title.
    //
    lv_obj_t* title = lv_label_create(header);

    lv_label_set_text(
        title,
        "CH3RRYB0MB"
    );

    lv_obj_set_style_text_color(
        title,
        lv_color_hex(0xFFFFFF),
        LV_PART_MAIN
    );

    lv_obj_align(
        title,
        LV_ALIGN_LEFT_MID,
        10,
        0
    );

    //
    // AP Mode menu button.
    //
    lv_obj_t* apModeButton = lv_button_create(screen);

    lv_obj_set_size(
        apModeButton,
        220,
        45
    );

    lv_obj_align(
        apModeButton,
        LV_ALIGN_TOP_MID,
        0,
        60
    );

    //
    // Remove the default LVGL button appearance.
    //
    lv_obj_set_style_radius(
        apModeButton,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_color(
        apModeButton,
        lv_color_hex(0x181818),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        apModeButton,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        apModeButton,
        1,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        apModeButton,
        lv_color_hex(0x8A0000),
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_width(
        apModeButton,
        0,
        LV_PART_MAIN
    );

    //
    // AP Mode button label.
    //
    lv_obj_t* apModeLabel = lv_label_create(apModeButton);

    lv_label_set_text(
        apModeLabel,
        "> AP MODE"
    );

    lv_obj_set_style_text_color(
        apModeLabel,
        lv_color_hex(0xFFFFFF),
        LV_PART_MAIN
    );

    lv_obj_align(
        apModeLabel,
        LV_ALIGN_LEFT_MID,
        5,
        0
    );

    //
    // AP Mode navigation event.
    //
    lv_obj_add_event_cb(
        apModeButton,
        apModeClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    return screen;
}

/**
 * @brief Handles the AP Mode menu button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the AP Mode screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenMainMenu::apModeClicked(lv_event_t* event)
{
    Serial.println("AP MODE CLICKED");

    ScreenManager* screenManager =
        static_cast<ScreenManager*>(
            lv_event_get_user_data(event)
        );

    if (screenManager == nullptr)
    {
        Serial.println("ScreenManager is null");
        return;
    }

    Serial.println("Showing AP MODE screen");
    screenManager->show(Screen::APMode);
}