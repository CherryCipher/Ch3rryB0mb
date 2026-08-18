#include "uimanager.h"

#include <esp32_smartdisplay.h>

/**
 * @brief Raw XPT2046 calibration limits.
 *
 * These values are temporary starting points and should be
 * calibrated for the specific touchscreen.
 */
static constexpr uint16_t TOUCH_RAW_X_MIN = 200;
static constexpr uint16_t TOUCH_RAW_X_MAX = 3900;

static constexpr uint16_t TOUCH_RAW_Y_MIN = 200;
static constexpr uint16_t TOUCH_RAW_Y_MAX = 3900;

/**
 * @brief Constructs a new UIManager.
 *
 * Stores a reference to the application's Logger and configures
 * the software SPI touch controller using the CYD touch pins.
 *
 * @param logger Reference to the application's Logger.
 */
UIManager::UIManager(Logger& logger)
    : logger(logger),
      touch(
          32, // MOSI
          39, // MISO
          25, // CLK
          33, // CS
          36  // IRQ
      )
{
}

/**
 * @brief Starts the user interface.
 *
 * Initializes esp32-smartdisplay for the display and LVGL,
 * starts the software SPI touch controller and registers
 * the touch input device with LVGL.
 *
 * @return true when initialization has completed.
 */
bool UIManager::start()
{
    if (running)
        return true;

    // Initialize display and LVGL.
    //
    // The esp32-smartdisplay hardware touch initialization
    // must be disabled separately to prevent SPI conflicts.
    smartdisplay_init();

    // Initialize software SPI touch controller.
    if (!touch.start())
    {
        logger.error("Failed to start software touch.");
        return false;
    }

    // Register the software touch controller as an
    // LVGL pointer input device.
    touchInput = lv_indev_create();

    if (touchInput == nullptr)
    {
        logger.error("Failed to create LVGL touch input device.");
        return false;
    }

    lv_indev_set_type( touchInput, LV_INDEV_TYPE_POINTER );

    lv_indev_set_read_cb( touchInput, readTouch );

    lv_indev_set_user_data( touchInput, this );

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
    if (!running)
        return;

    uint32_t currentTick = millis();

    lv_tick_inc(currentTick - lastTick);

    lastTick = currentTick;

    lv_timer_handler();
}

/**
 * @brief Check to see if UIManager is running.
 *
 * @return true if the manager is running.
 * @return false otherwise.
 */
bool UIManager::isRunning() const
{
    return running;
}

/**
 * @brief Reads touch input for LVGL.
 *
 * Reads raw XPT2046 coordinates using the software SPI
 * touch driver and maps them to display coordinates.
 *
 * @param indev Pointer to the LVGL input device.
 * @param data Pointer to the LVGL input data structure.
 */
void UIManager::readTouch( lv_indev_t* indev, lv_indev_data_t* data )
{
    auto* ui = static_cast<UIManager*>( lv_indev_get_user_data(indev) );

    if (ui == nullptr)
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    uint16_t rawX;
    uint16_t rawY;

    if (!ui->touch.read(rawX, rawY))
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    // Convert raw XPT2046 values to display coordinates.
    //
    // The current board configuration uses:
    //
    // TOUCH_SWAP_XY   = false
    // TOUCH_MIRROR_X  = true
    // TOUCH_MIRROR_Y  = false
    //
    // so X is mirrored here as well.
    int32_t x = map( rawX, TOUCH_RAW_X_MIN, TOUCH_RAW_X_MAX, 239, 0 );

    int32_t y = map( rawY, TOUCH_RAW_Y_MIN, TOUCH_RAW_Y_MAX, 0, 319 );

    // Keep coordinates inside the display bounds.
    x = constrain(x, 0, 239);
    y = constrain(y, 0, 319);

    data->point.x = x;
    data->point.y = y;

    data->state = LV_INDEV_STATE_PRESSED;
}