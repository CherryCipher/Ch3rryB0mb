/**
 * @file screencc1101scanner.cpp
 * @brief Implementation of the CC1101 Sub-GHz Scanner screen.
 *
 * This file creates the Sub-GHz Scanner interface and handles
 * basic scanner controls.
 */

#include "screencc1101scanner.h"

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/cc1101scanner/cc1101scanner.h"

/**
 * @brief Context used by the back navigation button.
 */
ScreenCC1101Scanner::BackContext ScreenCC1101Scanner::backContext = {nullptr, nullptr};

/**
 * @brief Creates the Sub-GHz Scanner screen.
 *
 * Creates the basic scanner interface with explicit start and stop controls.
 *
 * @param screenManager Reference to the application ScreenManager.
 * @param cc1101Scanner Reference to the CC1101Scanner feature.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenCC1101Scanner::create(ScreenManager& screenManager, CC1101Scanner& cc1101Scanner)
{
    lv_obj_t* screen = UIWidgets::createScreen();

    UIWidgets::addHeader(screen, 0, 0, "SUB-GHZ SC4N");

    backContext.screenManager = &screenManager;
    backContext.cc1101Scanner = &cc1101Scanner;

    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &backContext);

    UIWidgets::addText(screen, 15, 55, "CC1101 SUB-GHZ RADIO", 210);
    UIWidgets::addText(screen, 15, 85, "Scanner ready.", 210);

    lv_obj_t* startButton = UIWidgets::addButton(screen, 15, 265, "START", 100, 35);
    lv_obj_add_event_cb(startButton, startClicked, LV_EVENT_CLICKED, &cc1101Scanner);

    lv_obj_t* stopButton = UIWidgets::addButton(screen, 125, 265, "STOP", 100, 35);
    lv_obj_add_event_cb(stopButton, stopClicked, LV_EVENT_CLICKED, &cc1101Scanner);

    return screen;
}

/**
 * @brief Handles the scanner start button event.
 *
 * Starts the CC1101 scanner feature.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenCC1101Scanner::startClicked(lv_event_t* event)
{
    CC1101Scanner* cc1101Scanner =
        static_cast<CC1101Scanner*>(lv_event_get_user_data(event));

    if (cc1101Scanner == nullptr) return;

    cc1101Scanner->start();
}

/**
 * @brief Handles the scanner stop button event.
 *
 * Stops the CC1101 scanner feature.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenCC1101Scanner::stopClicked(lv_event_t* event)
{
    CC1101Scanner* cc1101Scanner =
        static_cast<CC1101Scanner*>(lv_event_get_user_data(event));

    if (cc1101Scanner == nullptr) return;

    cc1101Scanner->stop();
}

/**
 * @brief Handles the Sub-GHz Scanner back button event.
 *
 * Stops the scanner before navigating back to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenCC1101Scanner::backClicked(lv_event_t* event)
{
    BackContext* context =
        static_cast<BackContext*>(lv_event_get_user_data(event));

    if (context == nullptr ||
        context->screenManager == nullptr ||
        context->cc1101Scanner == nullptr)
        return;

    context->cc1101Scanner->stop();
    context->screenManager->back();
}