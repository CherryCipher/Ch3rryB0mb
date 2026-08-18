#include "screenapmode.h"
#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/apmode/apmode.h"

/**
 * @brief Status label used by the AP Mode screen.
 */
lv_obj_t* ScreenAPMode::statusText = nullptr;

/**
 * @brief Creates the AP Mode screen.
 *
 * Creates the AP Mode interface with START, STOP and BACK controls.
 *
 * @param screenManager Reference to the application ScreenManager.
 * @param apMode Reference to the AP Mode feature.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenAPMode::create(ScreenManager& screenManager, APMode& apMode)
{
    // Screen background.
    lv_obj_t* screen = UIWidgets::createScreen();

    // Header.
    UIWidgets::addHeader(screen, 0, 0, "AP MODE" );

    // Back button.
    lv_obj_t* backButton = UIWidgets::addButton( screen, 150, 5, "< BACK", 80, 30 );

    lv_obj_add_event_cb(
        backButton,
        backClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    // Start button.
    lv_obj_t* startButton = UIWidgets::addButton(screen, 15, 50, "START", 100, 40);

    lv_obj_add_event_cb(
        startButton,
        startClicked,
        LV_EVENT_CLICKED,
        &apMode
    );

    // Stop button.
    lv_obj_t* stopButton = UIWidgets::addButton(screen, 125, 50, "STOP", 100, 40);

    lv_obj_add_event_cb(
        stopButton,
        stopClicked,
        LV_EVENT_CLICKED,
        &apMode
    );

    // AP status information stored in statusText. Empty for now filled dynamically
    statusText = UIWidgets::addText( screen, 15, 115, "initializing...", 210);

    // Stop button.
    lv_obj_t* configButton = UIWidgets::addButton(screen, 15, 220, "Config AP", 100, 40);

    lv_obj_add_event_cb(
        configButton,
        configClicked,
        LV_EVENT_CLICKED,
        &apMode
    );


    //update the status
    updateStatus(apMode);

    return screen;
}

/**
 * @brief Updates the AP status information shown on the screen.
 *
 * When AP Mode is running, the SSID, password and connection address
 * are displayed. When AP Mode is stopped, an instruction is shown
 * explaining how to start the access point.
 *
 * @param apMode Reference to the AP Mode feature.
 */
void ScreenAPMode::updateStatus(APMode& apMode)
{
    if (statusText == nullptr)
    {
        return;
    }

    if (apMode.isRunning())
    {
        const WiFiAPConfig& config = apMode.getConfig();

        String status =
            "AP Mode running...\n"
            "SSID: " + config.ssid + "\n"
            "PW: " + config.password + "\n"
            "Connect and navigate to:\n"
            "http://" + apMode.getIP();

        lv_label_set_text(
            statusText,
            status.c_str()
        );

        return;
    }

    lv_label_set_text(
        statusText,
        "AP Mode stopped.\n"
        "\n"
        "Press START to enable the\n"
        "Ch3rryB0mb access point."
    );
}

/**
 * @brief Handles the AP Mode start button event.
 *
 * Called when the user activates the START control.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPMode::startClicked(lv_event_t* event)
{
    auto* apMode =
        static_cast<APMode*>(lv_event_get_user_data(event));

    if (apMode == nullptr)
    {
        return;
    }

    apMode->start();

    //update the status text
    updateStatus(*apMode);
}

/**
 * @brief Handles the AP Mode stop button event.
 *
 * Called when the user activates the STOP control.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPMode::stopClicked(lv_event_t* event)
{
    auto* apMode =
        static_cast<APMode*>(lv_event_get_user_data(event));

    if (apMode == nullptr)
    {
        return;
    }

    apMode->stop();

    //update the status text
    updateStatus(*apMode);
}

/**
 * @brief Handles the Config button event.
 *
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPMode::configClicked(lv_event_t* event)
{
    ScreenManager* screenManager =
        static_cast<ScreenManager*>(
            lv_event_get_user_data(event)
        );

    if (screenManager == nullptr)
    {
        return;
    }

    //implementation to set config
}

/**
 * @brief Handles the AP Mode back button event.
 *
 * Retrieves the ScreenManager from the LVGL event user data
 * and requests navigation to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenAPMode::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager =
        static_cast<ScreenManager*>(
            lv_event_get_user_data(event)
        );

    if (screenManager == nullptr)
    {
        return;
    }

    screenManager->back();
}