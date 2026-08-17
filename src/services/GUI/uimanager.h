#pragma once

#include <Arduino.h>
#include "../logger/logger.h"

/**
 * @class UIManager
 * @brief Manages the graphical user interface.
 *
 * The UIManager initializes the display and touch interface through
 * esp32-smartdisplay and keeps LVGL updated during runtime.
 *
 * Screen creation and navigation can be added on top of this manager.
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
     * Initializes esp32-smartdisplay, including the display,
     * touch controller and LVGL.
     *
     * @return true if the UI was started successfully.
     */
    bool start();

    /**
     * @brief Returns true if the UIManager is running
     * 
     * @return true if manager is running.
     * @return false if otherwise.
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
    * @brief Stops the UImanager
    *
    * This function should be called when the application is shutting down
    *
    * @return true if stopped succesfull
    * @return false otherwise.
    */
    bool stop();

private:
    /**
     * @brief Variable to check if UIManager is running
     *
     * Used to check if the UIManager is runnign to handle updating in services
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
    *
    * Used for logging messages related to web server operations.
    */
    Logger& logger;
};