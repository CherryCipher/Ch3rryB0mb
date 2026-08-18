/**
 * @file uiwidget.cpp
 * @brief Implementation of reusable styled LVGL UI widgets.
 *
 * This file contains the implementation of the UIWidgets helper class.
 *
 * The styling for common Ch3rryB0mb UI elements is defined here so that
 * individual screens only need to describe their layout and behavior.
 */

#include "uiwidget.h"

/**
 * @brief Creates a styled Ch3rryB0mb header.
 *
 * Creates a full-width header bar with the default Ch3rryB0mb styling
 * and places the supplied title text inside the header.
 *
 * @param parent Parent LVGL object.
 * @param x Horizontal position in pixels.
 * @param y Vertical position in pixels.
 * @param text Text displayed in the header.
 *
 * @return Pointer to the created LVGL header container.
 */
lv_obj_t* UIWidgets::addHeader(
    lv_obj_t* parent,
    int x,
    int y,
    const char* text
)
{
    //
    // Header container.
    //
    lv_obj_t* header = lv_obj_create(parent);

    lv_obj_set_pos(header, x, y);
    lv_obj_set_size(header, 240, 40);

    lv_obj_set_style_bg_color(
        header,
        lv_color_hex(0x7A0019),
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

    lv_obj_clear_flag(
        header,
        LV_OBJ_FLAG_SCROLLABLE
    );

    //
    // Header title.
    //
    lv_obj_t* label = lv_label_create(header);

    lv_label_set_text(label, text);

    lv_obj_set_style_text_color(
        label,
        lv_color_hex(0xFFFFFF),
        LV_PART_MAIN
    );

    lv_obj_align(
        label,
        LV_ALIGN_LEFT_MID,
        10,
        0
    );

    return header;
}

/**
 * @brief Creates a styled button.
 *
 * Creates an LVGL button using the default Ch3rryB0mb button styling.
 * A centered label containing the supplied text is automatically added
 * to the button.
 *
 * @param parent Parent LVGL object.
 * @param x Horizontal position in pixels.
 * @param y Vertical position in pixels.
 * @param text Text displayed on the button.
 * @param width Button width in pixels.
 * @param height Button height in pixels.
 *
 * @return Pointer to the created LVGL button object.
 */
lv_obj_t* UIWidgets::addButton(
    lv_obj_t* parent,
    int x,
    int y,
    const char* text,
    int width,
    int height
)
{
    // Create button
    lv_obj_t* button = lv_btn_create(parent);

    lv_obj_set_pos(button, x, y);
    lv_obj_set_size(button, width, height);

    // Button styling
    lv_obj_set_style_bg_color(
        button,
        lv_color_hex(0x080808),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        button,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_set_style_radius(
        button,
        3,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        button,
        1,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_color(
        button,
        lv_color_hex(0xFF1744),
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_width(
        button,
        0,
        LV_PART_MAIN
    );

    //
    // Pressed state styling
    //
    lv_obj_set_style_bg_color(
        button,
        lv_color_hex(0xA00024),
        LV_PART_MAIN | LV_STATE_PRESSED
    );

    //
    // Button label
    //
    lv_obj_t* label = lv_label_create(button);

    lv_label_set_text(label, text);

    lv_obj_set_style_text_color(
        label,
        lv_color_hex(0xFFFFFF),
        LV_PART_MAIN
    );

    lv_obj_center(label);

    return button;
}

/**
 * @brief Creates a default Ch3rryB0mb screen.
 *
 * Creates a new LVGL screen object and applies the default
 * Ch3rryB0mb background styling.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* UIWidgets::createScreen()
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

    lv_obj_clear_flag(
        screen,
        LV_OBJ_FLAG_SCROLLABLE
    );

    return screen;
}