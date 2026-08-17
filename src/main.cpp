#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <lvgl.h>

lv_obj_t* buttonLabel = nullptr;

static void buttonClicked(lv_event_t* event)
{
    lv_label_set_text(buttonLabel, "TOUCH WORKS!");
}

void setup()
{
    Serial.begin(115200);

    //
    // Initialize CYD display + touch + LVGL.
    //
    smartdisplay_init();

    //
    // Get the active LVGL screen.
    //
    lv_obj_t* screen = lv_screen_active();

    //
    // Black background.
    //
    lv_obj_set_style_bg_color(
        screen,
        lv_color_hex(0x000000),
        LV_PART_MAIN
    );

    //
    // Ch3rryB0mb title.
    //
    lv_obj_t* title = lv_label_create(screen);

    lv_label_set_text(
        title,
        "Ch3rryB0mb"
    );

    lv_obj_set_style_text_color(
        title,
        lv_color_hex(0xFF0000),
        LV_PART_MAIN
    );

    lv_obj_align(
        title,
        LV_ALIGN_CENTER,
        0,
        -60
    );

    //
    // Touch test button.
    //
    lv_obj_t* button = lv_button_create(screen);

    lv_obj_set_size(
        button,
        160,
        60
    );

    lv_obj_center(button);

    //
    // Button label.
    //
    buttonLabel = lv_label_create(button);

    lv_label_set_text(
        buttonLabel,
        "TOUCH ME"
    );

    lv_obj_center(buttonLabel);

    //
    // Register click event.
    //
    lv_obj_add_event_cb(
        button,
        buttonClicked,
        LV_EVENT_CLICKED,
        nullptr
    );
}

void loop()
{
    static uint32_t lastTick = millis();

    uint32_t now = millis();

    lv_tick_inc(now - lastTick);

    lastTick = now;

    lv_timer_handler();

    delay(5);
}