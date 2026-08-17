#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>
#include <XPT2046_Touchscreen.h>

#include "../logger/logger.h"

/**
 * @class TouchManager
 * @brief Manages the touchscreen hardware and LVGL input interface.
 *
 * The TouchManager is responsible for initializing the physical
 * touchscreen controller and connecting it to LVGL as a pointer
 * input device.
 *
 * Higher-level user interface components should use LVGL events
 * instead of communicating directly with the touchscreen hardware.
 *
 * Responsibilities
 * ----------------
 * - Initialize the touchscreen controller.
 * - Register the touchscreen as an LVGL input device.
 * - Read raw touchscreen coordinates.
 * - Convert touchscreen coordinates to display coordinates.
 * - Provide touch input to LVGL.
 *
 * Example
 * -------
 * @code
 * TouchManager touch(logger);
 *
 * if (!touch.start())
 * {
 *     logger.error("Failed to start TouchManager.");
 * }
 * @endcode
 */
class TouchManager
{
public:
    /**
     * @brief Constructs a new TouchManager.
     *
     * @param logger Reference to the application Logger.
     */
    explicit TouchManager(Logger& logger);

    /**
     * @brief Initializes the touchscreen and LVGL input device.
     *
     * Initializes the physical touch controller and registers
     * the touchscreen with LVGL.
     *
     * LVGL must already be initialized by the DisplayManager
     * before this method is called.
     *
     * Example
     * -------
     * @code
     * if (!touch.start())
     * {
     *     logger.error("Failed to initialize touchscreen.");
     * }
     * @endcode
     *
     * @return true if the touchscreen initialized successfully.
     * @return false otherwise.
     */
    bool start();

    /**
     * @brief Returns whether the TouchManager is running.
     *
     * Example
     * -------
     * @code
     * if (touch.isRunning())
     * {
     *     logger.info("Touchscreen is ready.");
     * }
     * @endcode
     *
     * @return true if the touchscreen is initialized.
     * @return false otherwise.
     */
    bool isRunning() const;

private:
    /**
     * @brief Reads touchscreen input for LVGL.
     *
     * This callback is called by LVGL whenever the current
     * pointer state is required.
     *
     * @param indev LVGL input device requesting the input.
     * @param data LVGL input data structure to populate.
     */
    static void readTouch(
        lv_indev_t* indev,
        lv_indev_data_t* data
    );

    /**
     * @brief Indicates whether the touchscreen is initialized.
     */
    bool running = false;

    /**
     * @brief Reference to the application Logger.
     */
    Logger& logger;

    /**
     * @brief SPI interface used by the touchscreen controller.
     *
     * The CYD touchscreen uses a separate SPI configuration
     * from the TFT display.
     */
    SPIClass touchSPI;

    /**
     * @brief Physical XPT2046 touchscreen controller.
     */
    XPT2046_Touchscreen touchscreen;

    /**
     * @brief LVGL touchscreen input device.
     */
    lv_indev_t* inputDevice = nullptr;
};