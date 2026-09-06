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
 * An optional status value can be displayed on the right side of the
 * header for connection or system status indicators.
 *
 * @param parent Parent LVGL object.
 * @param x Horizontal position in pixels.
 * @param y Vertical position in pixels.
 * @param text Text displayed in the header.
 * @param status Optional status text displayed on the right side.
 *
 * @return Pointer to the created LVGL header container.
 */
lv_obj_t* UIWidgets::addHeader(lv_obj_t* parent, int x, int y, const char* text, const char* status)
{
    lv_obj_t* header = lv_obj_create(parent);
    lv_obj_set_pos(header, x, y);
    lv_obj_set_size(header, 240, 40);

    lv_obj_set_style_bg_color(header, lv_color_hex(0x7A0019), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(header, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(header, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(header, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(header, 0, LV_PART_MAIN);

    lv_obj_clear_flag(header, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* label = lv_label_create(header);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 10, 0);

    if (status != nullptr)
    {
        lv_obj_t* statusLabel = lv_label_create(header);
        lv_label_set_text(statusLabel, status);
        lv_obj_set_style_text_color(statusLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_align(statusLabel, LV_ALIGN_RIGHT_MID, -10, 0);
    }

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
lv_obj_t* UIWidgets::addButton(lv_obj_t* parent, int x, int y, const char* text, int width, int height)
{
    // Create button.
    lv_obj_t* button = lv_btn_create(parent);

    lv_obj_set_pos(button, x, y);
    lv_obj_set_size(button, width, height);

    // Button styling.
    lv_obj_set_style_bg_color(button, lv_color_hex(0x080808), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(button, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(button, 3, LV_PART_MAIN);
    lv_obj_set_style_border_width(button, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(button, lv_color_hex(0xFF1744), LV_PART_MAIN);
    lv_obj_set_style_shadow_width(button, 0, LV_PART_MAIN);

    // Pressed state styling.
    lv_obj_set_style_bg_color(button, lv_color_hex(0xA00024), static_cast<lv_style_selector_t>(LV_PART_MAIN) | LV_STATE_PRESSED);

    // Disabled state styling.
    lv_obj_set_style_bg_color(button, lv_color_hex(0x101010), static_cast<lv_style_selector_t>(LV_PART_MAIN) | LV_STATE_DISABLED);
    lv_obj_set_style_border_color(button, lv_color_hex(0x444444), static_cast<lv_style_selector_t>(LV_PART_MAIN) | LV_STATE_DISABLED);
    lv_obj_set_style_text_color(button, lv_color_hex(0x666666), static_cast<lv_style_selector_t>(LV_PART_MAIN) | LV_STATE_DISABLED);

    // Button label.
    lv_obj_t* label = lv_label_create(button);

    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_center(label);

    return button;
}

/**
 * @brief Creates a styled text label.
 *
 * Creates an LVGL label using the default Ch3rryB0mb text styling.
 * Long text automatically wraps within the configured width.
 *
 * @param parent Parent LVGL object.
 * @param x Horizontal position in pixels.
 * @param y Vertical position in pixels.
 * @param text Initial text displayed by the label.
 * @param width Maximum width of the text area in pixels.
 *
 * @return Pointer to the created LVGL label object.
 */
lv_obj_t* UIWidgets::addText(lv_obj_t* parent, int x, int y, const char* text, int width)
{
    lv_obj_t* label = lv_label_create(parent);

    lv_obj_set_pos(label, x, y);
    lv_obj_set_width(label, width);
    lv_label_set_text(label, text);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);

    // Text styling.
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    return label;
}

/**
 * @brief Creates a styled text input field.
 *
 * Creates an LVGL textarea configured as a single-line input and applies
 * the default Ch3rryB0mb styling. Password mode can optionally be enabled
 * to obscure entered characters.
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
lv_obj_t* UIWidgets::addInput(lv_obj_t* parent, int x, int y, const char* text, int width, bool passwordMode)
{
    lv_obj_t* input = lv_textarea_create(parent);

    lv_obj_set_pos(input, x, y);
    lv_obj_set_size(input, width, 40);

    // Input behavior.
    lv_textarea_set_one_line(input, true);
    lv_textarea_set_text(input, text);
    lv_textarea_set_password_mode(input, passwordMode);

    // Input styling.
    lv_obj_set_style_bg_color(input, lv_color_hex(0x080808), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(input, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(input, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(input, lv_color_hex(0xFF7A00), LV_PART_MAIN);
    lv_obj_set_style_radius(input, 3, LV_PART_MAIN);
    lv_obj_set_style_text_color(input, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_pad_left(input, 8, LV_PART_MAIN);
    lv_obj_set_style_pad_right(input, 8, LV_PART_MAIN);
    lv_obj_set_style_pad_top(input, 8, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(input, 8, LV_PART_MAIN);

    return input;
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

    // Screen background.
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x080808), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(screen, 0, LV_PART_MAIN);

    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    return screen;
}

/**
 * @brief Creates a scrollable content container.
 *
 * Creates a transparent LVGL container with vertical scrolling enabled.
 * Child widgets can extend beyond the visible height and are accessible
 * by scrolling the container.
 *
 * @param parent Parent LVGL object.
 * @param x Horizontal position in pixels.
 * @param y Vertical position in pixels.
 * @param width Container width in pixels.
 * @param height Container height in pixels.
 *
 * @return Pointer to the created scrollable LVGL container.
 */
lv_obj_t* UIWidgets::createScrollContainer(lv_obj_t* parent, int x, int y, int width, int height)
{
    lv_obj_t* container = lv_obj_create(parent);

    lv_obj_set_pos(container, x, y);
    lv_obj_set_size(container, width, height);

    // Container styling.
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(container, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(container, 0, LV_PART_MAIN);

    // Enable vertical scrolling only.
    lv_obj_set_scroll_dir(container, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_AUTO);

    return container;
}

/**
 * @brief Creates a styled on-screen keyboard.
 *
 * Creates an LVGL keyboard aligned to the bottom of its parent.
 * The keyboard uses the default Ch3rryB0mb styling and is hidden
 * when initially created.
 *
 * The keyboard can be connected to a textarea using
 * lv_keyboard_set_textarea().
 *
 * @param parent Parent LVGL object.
 * @param width Keyboard width in pixels.
 * @param height Keyboard height in pixels.
 *
 * @return Pointer to the created LVGL keyboard object.
 */
lv_obj_t* UIWidgets::addKeyboard(lv_obj_t* parent, int width, int height)
{
    lv_obj_t* keyboard = lv_keyboard_create(parent);

    lv_obj_set_size(keyboard, width, height);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);

    // Keyboard styling.
    lv_obj_set_style_bg_color(keyboard, lv_color_hex(0x080808), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(keyboard, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(keyboard, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(keyboard, lv_color_hex(0xFF1744), LV_PART_MAIN);
    lv_obj_set_style_radius(keyboard, 0, LV_PART_MAIN);

    // Keyboard key styling.
    lv_obj_set_style_bg_color(keyboard, lv_color_hex(0x080808), LV_PART_ITEMS);
    lv_obj_set_style_border_width(keyboard, 1, LV_PART_ITEMS);
    lv_obj_set_style_border_color(keyboard, lv_color_hex(0x7A0019), LV_PART_ITEMS);
    lv_obj_set_style_text_color(keyboard, lv_color_hex(0xFFFFFF), LV_PART_ITEMS);

    // Pressed key styling.
    lv_obj_set_style_bg_color(keyboard, lv_color_hex(0x7A0019), static_cast<lv_style_selector_t>(LV_PART_ITEMS) | LV_STATE_PRESSED);

    // Keyboard starts hidden.
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);

    return keyboard;
}

/**
 * @brief Creates a styled toggle switch.
 *
 * Creates an LVGL switch and applies the default Ch3rryB0mb styling.
 * The checked state represents the enabled state of the toggle.
 *
 * @param parent Parent LVGL object.
 * @param x Horizontal position in pixels.
 * @param y Vertical position in pixels.
 * @param checked true to create the toggle in the enabled state, otherwise false.
 *
 * @return Pointer to the created LVGL switch object.
 */
lv_obj_t* UIWidgets::addToggle(lv_obj_t* parent, int x, int y, bool checked)
{
    lv_obj_t* toggle = lv_switch_create(parent);

    lv_obj_set_pos(toggle, x, y);
    lv_obj_set_size(toggle, 50, 26);

    // Main switch styling.
    lv_obj_set_style_bg_color(toggle, lv_color_hex(0x080808), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(toggle, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(toggle, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(toggle, lv_color_hex(0xFF7A00), LV_PART_MAIN);

    // Indicator styling.
    lv_obj_set_style_bg_color(toggle, lv_color_hex(0x7A0019), static_cast<lv_style_selector_t>(LV_PART_INDICATOR) | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(toggle, LV_OPA_COVER, static_cast<lv_style_selector_t>(LV_PART_INDICATOR) | LV_STATE_CHECKED);

    // Knob styling.
    lv_obj_set_style_bg_color(toggle, lv_color_hex(0xFFFFFF), LV_PART_KNOB);
    lv_obj_set_style_bg_opa(toggle, LV_OPA_COVER, LV_PART_KNOB);

    if (checked)
        lv_obj_add_state(toggle, LV_STATE_CHECKED);

    return toggle;
}

/**
 * @brief Creates an invisible spacer object.
 *
 * Creates a transparent, non-interactive LVGL object that occupies space
 * inside its parent. The spacer can be used to extend the content size of
 * scrollable containers without displaying a visible UI element.
 *
 * @param parent Parent LVGL object.
 * @param x Horizontal position in pixels.
 * @param y Vertical position in pixels.
 * @param width Spacer width in pixels.
 * @param height Spacer height in pixels.
 *
 * @return Pointer to the created LVGL spacer object.
 */
lv_obj_t* UIWidgets::addSpacer(lv_obj_t* parent, int x, int y, int width, int height)
{
    lv_obj_t* spacer = lv_obj_create(parent);

    lv_obj_set_pos(spacer, x, y);
    lv_obj_set_size(spacer, width, height);

    lv_obj_set_style_bg_opa(spacer, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(spacer, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(spacer, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(spacer, 0, LV_PART_MAIN);

    lv_obj_clear_flag(spacer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(spacer, LV_OBJ_FLAG_CLICKABLE);

    return spacer;
}

/**
 * @brief Creates a centered status box.
 *
 * Creates a temporary centered container using the Ch3rryB0mb
 * header styling and displays the supplied status text inside it.
 *
 * @param parent Parent LVGL object.
 * @param text Text displayed inside the status box.
 * @param width Width of the status box in pixels.
 * @param height Height of the status box in pixels.
 *
 * @return Pointer to the created LVGL status box object.
 */
lv_obj_t* UIWidgets::addStatusBox(lv_obj_t* parent, const char* text, int width, int height)
{
    // Status container.
    lv_obj_t* statusBox = lv_obj_create(parent);

    lv_obj_set_size(statusBox, width, height);
    lv_obj_align(statusBox, LV_ALIGN_CENTER, 0, 0);

    // Status box styling.
    lv_obj_set_style_bg_color(statusBox, lv_color_hex(0x7A0019), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(statusBox, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(statusBox, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(statusBox, lv_color_hex(0xFF1744), LV_PART_MAIN);
    lv_obj_set_style_radius(statusBox, 3, LV_PART_MAIN);
    lv_obj_set_style_pad_all(statusBox, 0, LV_PART_MAIN);

    lv_obj_clear_flag(statusBox, LV_OBJ_FLAG_SCROLLABLE);

    // Status text.
    lv_obj_t* label = lv_label_create(statusBox);

    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_center(label);

    // Keep the status box above other screen content.
    lv_obj_move_foreground(statusBox);

    return statusBox;
}

/**
 * @brief Creates a styled dropdown selector.
 *
 * Creates an LVGL dropdown and applies the default Ch3rryB0mb
 * input styling.
 *
 * @param parent Parent LVGL object.
 * @param x Horizontal position in pixels.
 * @param y Vertical position in pixels.
 * @param options Newline-separated dropdown options.
 * @param width Dropdown width in pixels.
 *
 * @return Pointer to the created LVGL dropdown object.
 */
lv_obj_t* UIWidgets::addDropdown(lv_obj_t* parent, int x, int y, const char* options, int width)
{
    lv_obj_t* dropdown = lv_dropdown_create(parent);

    lv_obj_set_pos(dropdown, x, y);
    lv_obj_set_width(dropdown, width);
    lv_dropdown_set_options(dropdown, options);

    // Dropdown styling.
    lv_obj_set_style_bg_color(dropdown, lv_color_hex(0x080808), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(dropdown, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_text_color(dropdown, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_border_width(dropdown, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(dropdown, lv_color_hex(0xFF1744), LV_PART_MAIN);
    lv_obj_set_style_radius(dropdown, 3, LV_PART_MAIN);

    // Dropdown list styling.
    lv_obj_t* list = lv_dropdown_get_list(dropdown);

    if (list != nullptr)
    {
        lv_obj_set_style_bg_color(list, lv_color_hex(0x080808), LV_PART_MAIN);
        lv_obj_set_style_text_color(list, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_border_color(list, lv_color_hex(0xFF1744), LV_PART_MAIN);
        lv_obj_set_style_border_width(list, 1, LV_PART_MAIN);
    }

    return dropdown;
}