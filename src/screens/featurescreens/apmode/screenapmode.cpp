#include "screenapmode.h"
#include "screens/screenmanager.h"

/**
 * @brief Creates the AP Mode screen.
 *
 * Creates the AP Mode interface with START, STOP and BACK controls.
 *
 * @param screenManager Reference to the application ScreenManager.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenAPMode::create(ScreenManager& screenManager, Features& feature)
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
        "AP MODE"
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
    // Start button.
    //
    lv_obj_t* startButton = lv_button_create(screen);

    lv_obj_set_size(
        startButton,
        100,
        45
    );

    lv_obj_align(
        startButton,
        LV_ALIGN_CENTER,
        -55,
        -10
    );

    lv_obj_set_style_radius(
        startButton,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_color(
        startButton,
        lv_color_hex(0x181818),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        startButton,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        startButton,
        1,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        startButton,
        lv_color_hex(0x8A0000),
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_width(
        startButton,
        0,
        LV_PART_MAIN
    );

    lv_obj_t* startLabel = lv_label_create(startButton);

    lv_label_set_text(
        startLabel,
        "START"
    );

    lv_obj_set_style_text_color(
        startLabel,
        lv_color_hex(0xFFFFFF),
        LV_PART_MAIN
    );

    lv_obj_center(startLabel);

    lv_obj_add_event_cb(
        startButton,
        startClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    //
    // Stop button.
    //
    lv_obj_t* stopButton = lv_button_create(screen);

    lv_obj_set_size(
        stopButton,
        100,
        45
    );

    lv_obj_align(
        stopButton,
        LV_ALIGN_CENTER,
        55,
        -10
    );

    lv_obj_set_style_radius(
        stopButton,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_color(
        stopButton,
        lv_color_hex(0x181818),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        stopButton,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        stopButton,
        1,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        stopButton,
        lv_color_hex(0x8A0000),
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_width(
        stopButton,
        0,
        LV_PART_MAIN
    );

    lv_obj_t* stopLabel = lv_label_create(stopButton);

    lv_label_set_text(
        stopLabel,
        "STOP"
    );

    lv_obj_set_style_text_color(
        stopLabel,
        lv_color_hex(0xFFFFFF),
        LV_PART_MAIN
    );

    lv_obj_center(stopLabel);

    lv_obj_add_event_cb(
        stopButton,
        stopClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    //
    // Back button.
    //
    lv_obj_t* backButton = lv_button_create(screen);

    lv_obj_set_size(
        backButton,
        100,
        40
    );

    lv_obj_align(
        backButton,
        LV_ALIGN_BOTTOM_LEFT,
        10,
        -10
    );

    lv_obj_set_style_radius(
        backButton,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_color(
        backButton,
        lv_color_hex(0x181818),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        backButton,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        backButton,
        1,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        backButton,
        lv_color_hex(0x8A0000),
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_width(
        backButton,
        0,
        LV_PART_MAIN
    );

    lv_obj_t* backLabel = lv_label_create(backButton);

    lv_label_set_text(
        backLabel,
        "< BACK"
    );

    lv_obj_set_style_text_color(
        backLabel,
        lv_color_hex(0xFFFFFF),
        LV_PART_MAIN
    );

    lv_obj_center(backLabel);

    lv_obj_add_event_cb(
        backButton,
        backClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    return screen;
}

/**
 * @brief Handles the AP Mode start button event.
 *
 * Called when the user activates the START control.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPMode::startClicked(lv_event_t* event)
{
    //
    // AP Mode start logic will be connected here later.
    //
}

/**
 * @brief Handles the AP Mode stop button event.
 *
 * Called when the user activates the STOP control.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPMode::stopClicked(lv_event_t* event)
{
    //
    // AP Mode stop logic will be connected here later.
    //
}

/**
 * @brief Handles the AP Mode back button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPMode::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager =
        static_cast<ScreenManager*>(
            lv_event_get_user_data(event)
        );

    if (screenManager == nullptr)
    {
        return;
    }

    screenManager->back();
}