#include "uimanager.h"

#include <esp32_smartdisplay.h>
#include <lvgl.h>

/**
 * @brief Constructs a new UIManager.
 *
 * Stores a reference to the application's Logger.
 *
 * @param logger Reference to the application's Logger.
 */
UIManager::UIManager(Logger& logger)
    : logger(logger)
{
}

/**
 * @brief Starts the user interface.
 *
 * Initializes esp32-smartdisplay. This configures the display,
 * touch controller and LVGL using the active PlatformIO
 * board definition.
 *
 * @return true when initialization has completed.
 */
bool UIManager::start()
{
    if(running)
        return true;

    smartdisplay_init();

    lastTick = millis();

    running = true;

    logger.info("UIManager started.");

    return true;
}

/**
 * @brief Stops the UIManager.
 */
bool UIManager::stop()
{
    running = false;

    return true;
}

/**
 * @brief Updates the user interface.
 *
 * Calculates the elapsed time since the previous update and
 * forwards it to LVGL. LVGL then processes its timers,
 * rendering tasks and input events.
 */
void UIManager::update()
{
    uint32_t currentTick = millis();

    lv_tick_inc(currentTick - lastTick);

    lastTick = currentTick;

    lv_timer_handler();
}

/**
 * @brief Check to see if UIManager is running
 *
 * @return true if the manager is running
 * @return false otherwise.
 */
bool UIManager::isRunning() const
{
    return running;
}