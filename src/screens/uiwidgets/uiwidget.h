/**
 * @file uiwidget.h
 * @brief Reusable styled LVGL UI widgets for Ch3rryB0mb.
 *
 * This file defines helper functions for creating consistently styled
 * LVGL widgets used throughout the Ch3rryB0mb user interface.
 *
 * Centralizing widget creation keeps screen implementations small and
 * prevents duplicated LVGL styling code.
 */

#pragma once

#include <lvgl.h>

/**
 * @class UIWidgets
 * @brief Provides reusable styled LVGL widgets.
 *
 * UIWidgets acts as a small abstraction layer on top of LVGL.
 *
 * Screens can use these helpers to create common UI elements without
 * having to define colors, borders, radius and other styling properties
 * every time a widget is created.
 */
class UIWidgets
{
public:
    /**
     * @brief Creates a styled header label.
     *
     * The header uses the default Ch3rryB0mb header styling.
     *
     * @param parent Parent LVGL object.
     * @param x Horizontal position in pixels.
     * @param y Vertical position in pixels.
     * @param text Text displayed in the header.
     *
     * @return Pointer to the created LVGL label object.
     */
    static lv_obj_t* addHeader(
        lv_obj_t* parent,
        int x,
        int y,
        const char* text
    );

    /**
     * @brief Creates a styled button.
     *
     * The button uses the default Ch3rryB0mb button styling and contains
     * a centered text label.
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
    static lv_obj_t* addButton(
        lv_obj_t* parent,
        int x,
        int y,
        const char* text,
        int width = 100,
        int height = 40
    );

    /**
     * @brief Creates a styled text label.
     *
     * Creates an LVGL label using the default Ch3rryB0mb text styling.
     * The returned label can later be updated using lv_label_set_text().
     *
     * @param parent Parent LVGL object.
     * @param x Horizontal position in pixels.
     * @param y Vertical position in pixels.
     * @param text Initial text displayed by the label.
     * @param width Maximum width of the text area in pixels.
     *
     * @return Pointer to the created LVGL label object.
     */
    static lv_obj_t* addText(
        lv_obj_t* parent,
        int x,
        int y,
        const char* text,
        int width = 220
    );

    /**
     * @brief Creates a styled text input field.
     *
     * Creates an LVGL textarea using the default Ch3rryB0mb input styling.
     * The field is configured as a single-line input and can optionally
     * hide entered characters for password input.
     *
     * @param parent Parent LVGL object.
     * @param x Horizontal position in pixels.
     * @param y Vertical position in pixels.
     * @param text Initial text displayed in the input field.
     * @param width Input field width in pixels.
     * @param passwordMode true to hide entered characters, otherwise false.
     *
     * @return Pointer to the created LVGL textarea object.
     */
    static lv_obj_t* addInput(
        lv_obj_t* parent,
        int x,
        int y,
        const char* text,
        int width = 210,
        bool passwordMode = false
    );

    /**
     * @brief Creates a default Ch3rryB0mb screen.
     *
     * Creates a new LVGL screen object using the default Ch3rryB0mb
     * background styling.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* createScreen();

    /**
     * @brief Creates a scrollable content container.
     *
     * Creates an LVGL container intended for vertically scrollable screen
     * content. Widgets added to this container move together while fixed
     * screen elements such as headers remain stationary.
     *
     * @param parent Parent LVGL object.
     * @param x Horizontal position in pixels.
     * @param y Vertical position in pixels.
     * @param width Container width in pixels.
     * @param height Container height in pixels.
     *
     * @return Pointer to the created scrollable LVGL container.
     */
    static lv_obj_t* createScrollContainer(
        lv_obj_t* parent,
        int x,
        int y,
        int width,
        int height
    );
};