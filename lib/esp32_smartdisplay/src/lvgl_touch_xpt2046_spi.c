/**
 * @file lvgl_touch_xpt2046_spi.c
 * @brief LVGL touch driver initialization for XPT2046 over SPI.
 *
 * Provides XPT2046 touch controller initialization and LVGL input
 * device integration for ESP32 Smart Display boards.
 */

#include <esp_log.h>
#include <esp_err.h>

#include <driver/spi_master.h>

#include <esp_lcd_panel_io.h>
#include <esp_touch_xpt2046.h>

#include "esp32_smartdisplay.h"

static const char* TAG = "XPT2046";

/**
 * @brief Reads touch data from the XPT2046 controller for LVGL.
 *
 * Retrieves the latest touch coordinates and updates the LVGL input
 * device state accordingly.
 *
 * @param indev LVGL input device.
 * @param data LVGL input data structure to update.
 */
static void xpt2046_lvgl_touch_cb(lv_indev_t* indev, lv_indev_data_t* data)
{
    esp_lcd_touch_handle_t touch_handle = lv_indev_get_user_data(indev);

    if (touch_handle == NULL) {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    uint16_t touch_x[1] = {0};
    uint16_t touch_y[1] = {0};
    uint16_t touch_strength[1] = {0};
    uint8_t touch_count = 0;

    esp_lcd_touch_read_data(touch_handle);

    bool touched = esp_lcd_touch_get_coordinates(
        touch_handle,
        touch_x,
        touch_y,
        touch_strength,
        &touch_count,
        1
    );

    if (!touched || touch_count == 0) {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    data->point.x = touch_x[0];
    data->point.y = touch_y[0];
    data->state = LV_INDEV_STATE_PRESSED;
}

/**
 * @brief Initializes the XPT2046 touch controller.
 *
 * Initializes the configured SPI bus if required, creates the LCD panel
 * IO interface used by the XPT2046 touch controller and registers the
 * controller as an LVGL pointer input device.
 *
 * ESP_ERR_INVALID_STATE from spi_bus_initialize() is accepted because it
 * indicates that the selected SPI host has already been initialized.
 */
void lvgl_touch_init(void)
{
    ESP_LOGD(TAG, "lvgl_touch_init");

    const spi_bus_config_t spi_bus_config = {
        .mosi_io_num = XPT2046_SPI_BUS_MOSI,
        .miso_io_num = XPT2046_SPI_BUS_MISO,
        .sclk_io_num = XPT2046_SPI_BUS_SCLK,
        .quadwp_io_num = XPT2046_SPI_BUS_QUADWP,
        .quadhd_io_num = XPT2046_SPI_BUS_QUADHD,
        .max_transfer_sz = 0,
    };

    esp_err_t bus_result = spi_bus_initialize(
        XPT2046_SPI_HOST,
        &spi_bus_config,
        XPT2046_SPI_DMA_CHANNEL
    );

    if (bus_result != ESP_OK && bus_result != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Failed to initialize XPT2046 SPI bus: %s", esp_err_to_name(bus_result));
        ESP_ERROR_CHECK(bus_result);
        return;
    }

    const esp_lcd_panel_io_spi_config_t io_spi_config = {
        .cs_gpio_num = XPT2046_SPI_CONFIG_CS,
        .dc_gpio_num = XPT2046_SPI_CONFIG_DC,
        .spi_mode = XPT2046_SPI_CONFIG_SPI_MODE,
        .pclk_hz = XPT2046_SPI_CONFIG_PCLK_HZ,
        .trans_queue_depth = XPT2046_SPI_CONFIG_TRANS_QUEUE_DEPTH,
        .lcd_cmd_bits = XPT2046_SPI_CONFIG_LCD_CMD_BITS,
        .lcd_param_bits = XPT2046_SPI_CONFIG_LCD_PARAM_BITS,
        .flags = {
            .dc_low_on_data = XPT2046_SPI_CONFIG_FLAGS_DC_LOW_ON_DATA,
            .octal_mode = XPT2046_SPI_CONFIG_FLAGS_OCTAL_MODE,
            .lsb_first = XPT2046_SPI_CONFIG_FLAGS_LSB_FIRST,
        },
    };

    esp_lcd_panel_io_handle_t io_handle = NULL;

    ESP_ERROR_CHECK(
        esp_lcd_new_panel_io_spi(
            (esp_lcd_spi_bus_handle_t)XPT2046_SPI_HOST,
            &io_spi_config,
            &io_handle
        )
    );

    const esp_lcd_touch_config_t touch_config = {
        .x_max = XPT2046_TOUCH_CONFIG_X_MAX,
        .y_max = XPT2046_TOUCH_CONFIG_Y_MAX,
        .rst_gpio_num = XPT2046_TOUCH_CONFIG_RST,
        .int_gpio_num = XPT2046_TOUCH_CONFIG_INT,
        .levels = {
            .reset = XPT2046_TOUCH_CONFIG_LEVELS_RESET,
            .interrupt = XPT2046_TOUCH_CONFIG_LEVELS_INTERRUPT,
        },
        .flags = {
            .swap_xy = TOUCH_SWAP_XY,
            .mirror_x = TOUCH_MIRROR_X,
            .mirror_y = TOUCH_MIRROR_Y,
        },
    };

    esp_lcd_touch_handle_t touch_handle = NULL;

    ESP_ERROR_CHECK(
        esp_lcd_touch_new_spi_xpt2046(
            io_handle,
            &touch_config,
            &touch_handle
        )
    );

    lv_indev_t* indev = lv_indev_create();

    if (indev == NULL) {
        ESP_LOGE(TAG, "Failed to create LVGL touch input device.");
        return;
    }

    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, xpt2046_lvgl_touch_cb);
    lv_indev_set_user_data(indev, touch_handle);

    ESP_LOGD(TAG, "XPT2046 touch initialized.");
}