#include "screenapmode.h"
#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"

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

    // Start button.
    lv_obj_t* startButton = UIWidgets::addButton(screen, 15, 75, "START", 100, 45);

    lv_obj_add_event_cb(
        startButton,
        startClicked,
        LV_EVENT_CLICKED,
        &apMode
    );

    // Stop button.
    lv_obj_t* stopButton = UIWidgets::addButton(screen, 125, 75, "STOP", 100, 45);

    lv_obj_add_event_cb(
        stopButton,
        stopClicked,
        LV_EVENT_CLICKED,
        &apMode
    );

    // Back button.
    lv_obj_t* backButton = UIWidgets::addButton( screen, 10, 270, "< BACK", 100, 40 );

    lv_obj_add_event_cb(
        backButton,
        backClicked,
        LV_EVENT_CLICKED,
        &screenManager
    );

    return screen;
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