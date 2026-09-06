/**
 * @file screennrfsend.cpp
 * @brief Implementation of the NRF24 message transmission screen.
 */

#include "screennrfsend.h"

#include <Arduino.h>
#include <cstdlib>

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/nrfsend/nrfsend.h"

ScreenNRFSend::Context ScreenNRFSend::context;

lv_obj_t* ScreenNRFSend::channelInput = nullptr;
lv_obj_t* ScreenNRFSend::messageInput = nullptr;
lv_obj_t* ScreenNRFSend::statusLabel = nullptr;
lv_obj_t* ScreenNRFSend::keyboard = nullptr;

/**
 * @brief Creates the NRF Send screen.
 *
 * @param screenManager Reference to the application ScreenManager.
 * @param nrfSend Reference to the NRFSend feature.
 *
 * @return Pointer to the created LVGL screen.
 */
lv_obj_t* ScreenNRFSend::create(ScreenManager& screenManager, NRFSend& nrfSend)
{
    lv_obj_t* screen = UIWidgets::createScreen();

    context.screenManager = &screenManager;
    context.nrfSend = &nrfSend;

    UIWidgets::addHeader(screen, 0, 0, "NRF SEND");

    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &context);

    lv_obj_t* content = UIWidgets::createScrollContainer(screen, 0, 40, 240, 280);

    UIWidgets::addText(content, 15, 15, "CHANNEL (2-80)", 210);
    channelInput = UIWidgets::addInput(content, 15, 40, String(nrfSend.getChannel()).c_str(), 210);
    lv_textarea_set_accepted_chars(channelInput, "0123456789");
    lv_textarea_set_max_length(channelInput, 2);

    UIWidgets::addText(content, 15, 95, "MESSAGE", 210);
    messageInput = UIWidgets::addInput(content, 15, 120, "", 210);
    lv_textarea_set_max_length(messageInput, NRFSend::MAX_MESSAGE_LENGTH);
    lv_textarea_set_placeholder_text(messageInput, "HELLO NODE");

    UIWidgets::addText(content, 15, 175, "1MBPS | ACK | C3N0", 210);

    lv_obj_t* sendButton = UIWidgets::addButton(content, 15, 205, "SEND", 210, 45);
    lv_obj_add_event_cb(sendButton, sendClicked, LV_EVENT_CLICKED, &context);

    UIWidgets::addText(content, 15, 270, "STATUS", 210);
    statusLabel = UIWidgets::addText(content, 15, 295, "", 210);

    UIWidgets::addSpacer(content, 0, 380, 1, 150);

    keyboard = UIWidgets::addKeyboard(screen);
    lv_obj_add_event_cb(keyboard, keyboardFinished, LV_EVENT_READY, nullptr);
    lv_obj_add_event_cb(keyboard, keyboardFinished, LV_EVENT_CANCEL, nullptr);

    lv_obj_add_event_cb(channelInput, inputFocused, LV_EVENT_FOCUSED, nullptr);
    lv_obj_add_event_cb(messageInput, inputFocused, LV_EVENT_FOCUSED, nullptr);

    lv_obj_add_event_cb(screen, screenDeleted, LV_EVENT_DELETE, nullptr);

    String ready = "READY\nCH " + String(nrfSend.getChannel()) + " | " + String(2400 + nrfSend.getChannel()) + " MHz";
    showStatus(ready.c_str());

    return screen;
}

/**
 * @brief Handles focus events for screen input fields.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNRFSend::inputFocused(lv_event_t* event)
{
    if (keyboard == nullptr) return;

    lv_obj_t* input = static_cast<lv_obj_t*>(lv_event_get_target(event));
    if (input == nullptr) return;

    if (input == channelInput)
        lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_NUMBER);
    else
        lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);

    lv_keyboard_set_textarea(keyboard, input);
    lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_scroll_to_view(input, LV_ANIM_ON);
}

/**
 * @brief Handles keyboard READY and CANCEL events.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNRFSend::keyboardFinished(lv_event_t* event)
{
    if (keyboard == nullptr) return;

    lv_keyboard_set_textarea(keyboard, nullptr);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief Handles the SEND button event.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNRFSend::sendClicked(lv_event_t* event)
{
    Context* callbackContext = static_cast<Context*>(lv_event_get_user_data(event));
    if (callbackContext == nullptr || callbackContext->nrfSend == nullptr || channelInput == nullptr || messageInput == nullptr) return;

    int channel = atoi(lv_textarea_get_text(channelInput));
    if (channel < MIN_CHANNEL || channel > MAX_CHANNEL) {
        showStatus("INVALID CHANNEL\nUSE CHANNEL 2-80");
        return;
    }

    if (!callbackContext->nrfSend->setChannel(static_cast<uint8_t>(channel))) {
        showStatus("INVALID CHANNEL");
        return;
    }

    String message = lv_textarea_get_text(messageInput);
    NRFSend::SendResult result = callbackContext->nrfSend->sendMessage(message);

    switch (result) {
        case NRFSend::SendResult::Success: {
            String status = "SENT - ACK RECEIVED\nCH " + String(channel) + " | " + String(2400 + channel) + " MHz\n" + String(message.length()) + " BYTES";
            showStatus(status.c_str());
            break;
        }

        case NRFSend::SendResult::NRFUnavailable:
            showStatus("NRF24 UNAVAILABLE");
            break;

        case NRFSend::SendResult::InvalidChannel:
            showStatus("INVALID CHANNEL");
            break;

        case NRFSend::SendResult::EmptyMessage:
            showStatus("MESSAGE IS EMPTY");
            break;

        case NRFSend::SendResult::MessageTooLong:
            showStatus("MESSAGE TOO LONG\nMAX 31 BYTES");
            break;

        case NRFSend::SendResult::ConfigurationFailed:
            showStatus("NRF CONFIG FAILED");
            break;

        case NRFSend::SendResult::NoAcknowledgement:
            showStatus("SEND FAILED - NO ACK\nCHECK NODE / CHANNEL");
            break;
    }
}

/**
 * @brief Handles the BACK button event.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNRFSend::backClicked(lv_event_t* event)
{
    Context* callbackContext = static_cast<Context*>(lv_event_get_user_data(event));
    if (callbackContext == nullptr || callbackContext->screenManager == nullptr) return;

    callbackContext->screenManager->back();
}

/**
 * @brief Updates the status display.
 *
 * @param text Text to display.
 */
void ScreenNRFSend::showStatus(const char* text)
{
    if (statusLabel == nullptr) return;

    lv_label_set_text(statusLabel, text);
}

/**
 * @brief Clears static screen references when the screen is deleted.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNRFSend::screenDeleted(lv_event_t* event)
{
    channelInput = nullptr;
    messageInput = nullptr;
    statusLabel = nullptr;
    keyboard = nullptr;

    context.screenManager = nullptr;
    context.nrfSend = nullptr;
}