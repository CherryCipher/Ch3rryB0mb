#include "touchmanager.h"

/**
 * @brief Constructs a new TouchManager.
 *
 * Stores the Logger reference and configures the touchscreen
 * controller with the configured chip select pin.
 */
TouchManager::TouchManager(Logger& logger)
    :   logger(logger),
        touchSPI(VSPI),
        touchscreen(TOUCH_CS)
{
}

/**
 * @brief Initializes the touchscreen and LVGL input device.
 *
 * Initializes the SPI interface used by the touchscreen,
 * starts the XPT2046 controller and registers the touchscreen
 * as an LVGL pointer input device.
 *
 * Workflow
 * --------
 * @code
 * Start TouchManager
 *        │
 *        ▼
 * Check running state
 *        │
 *        ├── Already running ──► Return true
 *        │
 *        ▼
 * Initialize touch SPI
 *        │
 *        ▼
 * Initialize XPT2046
 *        │
 *        ▼
 * Create LVGL input device
 *        │
 *        ▼
 * Register touch callback
 *        │
 *        ▼
 * Store TouchManager as user data
 *        │
 *        ▼
 * TouchManager ready
 * @endcode
 *
 * @return true if the touchscreen initialized successfully.
 * @return false otherwise.
 */
bool TouchManager::start()
{
    if (running)
    {
        logger.info("TouchManager already running.");
        return true;
    }

    logger.info("Starting TouchManager.");

    touchSPI.begin(
        TOUCH_SCLK,
        TOUCH_MISO,
        TOUCH_MOSI,
        TOUCH_CS
    );

    //
    // Initialize the XPT2046 touchscreen controller.
    //
    if (!touchscreen.begin(touchSPI))
    {
        logger.error("Failed to initialize touchscreen.");
        return false;
    }

    touchscreen.setRotation(3);

    //
    // Create LVGL pointer input device.
    //
    inputDevice = lv_indev_create();

    if (inputDevice == nullptr)
    {
        logger.error("Failed to create LVGL touch input device.");
        return false;
    }

    lv_indev_set_type(
        inputDevice,
        LV_INDEV_TYPE_POINTER
    );

    //
    // Store this TouchManager so the static callback can
    // access the physical touchscreen instance.
    //
    lv_indev_set_user_data(
        inputDevice,
        this
    );

    //
    // Register LVGL touchscreen callback.
    //
    lv_indev_set_read_cb(
        inputDevice,
        readTouch
    );

    running = true;

    logger.info("TouchManager started.");

    return true;
}

/**
 * @brief Reads touchscreen input for LVGL.
 *
 * Reads the XPT2046 controller and converts the raw touch
 * coordinates to display coordinates.
 *
 * The calibration values used here are initial values and may
 * need to be adjusted for the physical touchscreen.
 *
 * Workflow
 * --------
 * @code
 * LVGL requests pointer state
 *        │
 *        ▼
 * Retrieve TouchManager
 *        │
 *        ▼
 * Touch detected?
 *        │
 *        ├── No ──► Report RELEASED
 *        │
 *        ▼
 * Read raw touch coordinates
 *        │
 *        ▼
 * Map raw coordinates
 *        │
 *        ▼
 * Report PRESSED + position
 * @endcode
 *
 * @param indev LVGL input device requesting the input.
 * @param data LVGL input data structure to populate.
 */
void TouchManager::readTouch(
    lv_indev_t* indev,
    lv_indev_data_t* data
)
{
    TouchManager* manager =
        static_cast<TouchManager*>(
            lv_indev_get_user_data(indev)
        );

    if (manager == nullptr)
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    if (!manager->touchscreen.touched())
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    TS_Point point = manager->touchscreen.getPoint();

    Serial.print("TOUCH RAW X: ");
    Serial.print(point.x);
    Serial.print(" Y: ");
    Serial.print(point.y);
    Serial.print(" Z: ");
    Serial.println(point.z);

    data->state = LV_INDEV_STATE_RELEASED;
}

/**
 * @brief Returns whether the TouchManager is running.
 *
 * @return true if the touchscreen is initialized.
 * @return false otherwise.
 */
bool TouchManager::isRunning() const
{
    return running;
}