#pragma once

#include <Arduino.h>
#include <lvgl.h>

#include "../logger/logger.h"
#include "softtouch.h"

/**
 * @class UIManager
 * @brief Manages the graphical user interface.
 *
 * The UIManager initializes the display and LVGL through
 * esp32-smartdisplay.
 *
 * Touch input is handled separately through the SoftTouch
 * software SPI driver to prevent conflicts with the SD card.
 */
class UIManager
{
public:
    /**
     * @brief Constructs a new UIManager.
     *
     * @param logger Reference to the application's Logger.
     */
    explicit UIManager(Logger& logger);

    /**
     * @brief Starts the user interface.
     *
     * Initializes esp32-smartdisplay, the software SPI touch
     * controller and registers the touch device with LVGL.
     *
     * @return true if the UI was started successfully.
     */
    bool start();

    /**
     * @brief Returns true if the UIManager is running.
     *
     * @return true if manager is running.
     * @return false otherwise.
     */
    bool isRunning() const;

    /**
     * @brief Updates the user interface.
     *
     * Updates the LVGL system tick and processes pending
     * LVGL timers, rendering and input events.
     *
     * This method should be called continuously from the
     * application update loop.
     */
    void update();

    /**
     * @brief Stops the UIManager.
     *
     * @return true if stopped successfully.
     */
    bool stop();

private:
    /**
     * @brief LVGL callback used to read touch input.
     *
     * Reads raw touch data from the SoftTouch driver,
     * converts it to display coordinates and provides
     * the result to LVGL.
     *
     * @param indev Pointer to the LVGL input device.
     * @param data Pointer to the LVGL input data structure.
     */
    static void readTouch( lv_indev_t* indev, lv_indev_data_t* data );

    /**
     * @brief Variable to check if UIManager is running.
     */
    bool running = false;

    /**
     * @brief Timestamp of the previous LVGL tick update.
     *
     * Used to calculate the elapsed time between update calls.
     */
    uint32_t lastTick = 0;

    /**
     * @brief Reference to the application's Logger instance.
     */
    Logger& logger;

    /**
     * @brief Software SPI XPT2046 touch controller.
     */
    SoftTouch touch;

    /**
     * @brief LVGL touch input device.
     */
    lv_indev_t* touchInput = nullptr;
};