/**
 * @file screennodeconfig.cpp
 * @brief Implementation of the Ch3rryN0de configuration screen.
 */

#include "screennodeconfig.h"

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"

ScreenNodeConfig::Context ScreenNodeConfig::context;

lv_obj_t* ScreenNodeConfig::radioButton = nullptr;
lv_obj_t* ScreenNodeConfig::modeButton = nullptr;

lv_obj_t* ScreenNodeConfig::channelLabel = nullptr;
lv_obj_t* ScreenNodeConfig::channelInput = nullptr;

lv_obj_t* ScreenNodeConfig::frequencyLabel = nullptr;
lv_obj_t* ScreenNodeConfig::frequencyValueLabel = nullptr;
lv_obj_t* ScreenNodeConfig::frequencyInput = nullptr;

lv_obj_t* ScreenNodeConfig::intervalLabel = nullptr;
lv_obj_t* ScreenNodeConfig::intervalInput = nullptr;

lv_obj_t* ScreenNodeConfig::startButton = nullptr;
lv_obj_t* ScreenNodeConfig::keyboard = nullptr;

/**
 * @brief Creates the node configuration screen.
 *
 * @param screenManager Reference to the application ScreenManager.
 * @param configureNode Reference to the ConfigureNode feature.
 *
 * @return Pointer to the created LVGL screen.
 */
lv_obj_t* ScreenNodeConfig::create(ScreenManager& screenManager, ConfigureNode& configureNode)
{
    lv_obj_t* screen = UIWidgets::createScreen();

    context.screenManager = &screenManager;
    context.configureNode = &configureNode;
    context.screen = screen;

    UIWidgets::addHeader(screen, 0, 0, "N0DE CONFIG");

    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &context);

    if (!configureNode.hasSelectedNode()) {
        UIWidgets::addText(screen, 15, 60, "NO NODE SELECTED", 210);
        return screen;
    }

    NodeConfig& config = configureNode.getConfig();

    // BLE remains reserved in the protocol but is currently not exposed
    // as a configurable node session.
    if (config.radio == NodeRadio::BLE) config.radio = NodeRadio::NRF24;

    lv_obj_t* content = UIWidgets::createScrollContainer(screen, 0, 40, 240, 280);

    const BLEDeviceInfo& node = configureNode.getSelectedNode();

    String nodeName = node.name.length() > 0 ? node.name : "C3N0";
    UIWidgets::addText(content, 15, 10, nodeName.c_str(), 210);

    String nodeInfo = String(node.rssi) + " dBm | " + node.address;
    UIWidgets::addText(content, 15, 30, nodeInfo.c_str(), 210);

    UIWidgets::addText(content, 15, 65, "RADIO", 90);
    radioButton = UIWidgets::addButton(content, 110, 57, "", 115, 35);
    lv_obj_add_event_cb(radioButton, radioClicked, LV_EVENT_CLICKED, &context);

    UIWidgets::addText(content, 15, 110, "MODE", 90);
    modeButton = UIWidgets::addButton(content, 110, 102, "", 115, 35);
    lv_obj_add_event_cb(modeButton, modeClicked, LV_EVENT_CLICKED, &context);

    channelLabel = UIWidgets::addText(content, 15, 155, "CHANNEL (2-80)", 210);

    channelInput = UIWidgets::addInput(content, 15, 177, String(config.channel).c_str(), 210);
    lv_textarea_set_accepted_chars(channelInput, "0123456789");
    lv_textarea_set_max_length(channelInput, 2);
    lv_obj_add_event_cb(channelInput, inputFocused, LV_EVENT_FOCUSED, nullptr);
    lv_obj_add_event_cb(channelInput, channelChanged, LV_EVENT_VALUE_CHANGED, nullptr);

    frequencyLabel = UIWidgets::addText(content, 15, 225, "FREQUENCY", 210);

    String nrfFrequency = String(2400 + config.channel) + " MHz";
    frequencyValueLabel = UIWidgets::addText(content, 15, 247, nrfFrequency.c_str(), 210);

    frequencyInput = UIWidgets::addInput(content, 15, 247, String(config.frequency, 3).c_str(), 210);
    lv_textarea_set_accepted_chars(frequencyInput, "0123456789.");
    lv_textarea_set_max_length(frequencyInput, 7);
    lv_obj_add_event_cb(frequencyInput, inputFocused, LV_EVENT_FOCUSED, nullptr);

    intervalLabel = UIWidgets::addText(content, 15, 295, "INTERVAL (MS)", 210);

    intervalInput = UIWidgets::addInput(content, 15, 317, String(config.interval).c_str(), 210);
    lv_textarea_set_accepted_chars(intervalInput, "0123456789");
    lv_textarea_set_max_length(intervalInput, 5);
    lv_obj_add_event_cb(intervalInput, inputFocused, LV_EVENT_FOCUSED, nullptr);

    startButton = UIWidgets::addButton(content, 65, 385, "START", 110, 45);
    lv_obj_add_event_cb(startButton, startClicked, LV_EVENT_CLICKED, &context);

    UIWidgets::addSpacer(content, 0, 450, 1, 160);

    keyboard = UIWidgets::addKeyboard(screen);
    lv_obj_add_event_cb(keyboard, keyboardFinished, LV_EVENT_READY, nullptr);
    lv_obj_add_event_cb(keyboard, keyboardFinished, LV_EVENT_CANCEL, nullptr);

    lv_obj_add_event_cb(screen, screenDeleted, LV_EVENT_DELETE, nullptr);

    updateControls();

    return screen;
}

/**
 * @brief Updates the visible configuration controls.
 */
void ScreenNodeConfig::updateControls()
{
    if (context.configureNode == nullptr) return;

    NodeConfig& config = context.configureNode->getConfig();

    if (config.radio == NodeRadio::BLE) config.radio = NodeRadio::NRF24;

    if (radioButton != nullptr) {
        lv_obj_t* label = lv_obj_get_child(radioButton, 0);
        if (label != nullptr) lv_label_set_text(label, ConfigureNode::getRadioName(config.radio));
    }

    if (modeButton != nullptr) {
        lv_obj_t* label = lv_obj_get_child(modeButton, 0);
        if (label != nullptr) lv_label_set_text(label, ConfigureNode::getModeName(config.mode));
    }

    if (channelLabel == nullptr || channelInput == nullptr ||
        frequencyLabel == nullptr || frequencyValueLabel == nullptr ||
        frequencyInput == nullptr || intervalLabel == nullptr ||
        intervalInput == nullptr || startButton == nullptr) return;

    lv_obj_add_flag(channelLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(channelInput, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(frequencyLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(frequencyValueLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(frequencyInput, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(intervalLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(intervalInput, LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text(channelLabel, "CHANNEL (2-80)");
    lv_label_set_text(frequencyLabel, "FREQUENCY");
    lv_label_set_text(intervalLabel, "INTERVAL (MS) 100-60000");

    int y = 155;

    if (config.radio == NodeRadio::NRF24) {
        lv_obj_set_y(channelLabel, y);
        lv_obj_set_y(channelInput, y + 22);
        lv_obj_clear_flag(channelLabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(channelInput, LV_OBJ_FLAG_HIDDEN);
        y += 70;

        lv_obj_set_y(frequencyLabel, y);
        lv_obj_set_y(frequencyValueLabel, y + 22);
        lv_obj_clear_flag(frequencyLabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(frequencyValueLabel, LV_OBJ_FLAG_HIDDEN);
        updateNRFFrequency();
        y += 60;
    } else if (config.radio == NodeRadio::CC1101) {
        lv_obj_set_y(frequencyLabel, y);
        lv_obj_set_y(frequencyInput, y + 22);
        lv_obj_clear_flag(frequencyLabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(frequencyInput, LV_OBJ_FLAG_HIDDEN);
        y += 70;
    }

    if (config.mode == NodeMode::Beacon) {
        lv_obj_set_y(intervalLabel, y);
        lv_obj_set_y(intervalInput, y + 22);
        lv_obj_clear_flag(intervalLabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(intervalInput, LV_OBJ_FLAG_HIDDEN);
        y += 70;
    }

    lv_obj_set_y(startButton, y + 10);
}

/**
 * @brief Updates the calculated NRF24 frequency display.
 */
void ScreenNodeConfig::updateNRFFrequency()
{
    if (channelInput == nullptr || frequencyValueLabel == nullptr) return;

    String value = lv_textarea_get_text(channelInput);
    int channel = value.toInt();

    if (value.length() == 0 || channel < MIN_NRF_CHANNEL || channel > MAX_NRF_CHANNEL) {
        lv_label_set_text(frequencyValueLabel, "-- MHz");
        return;
    }

    String frequency = String(2400 + channel) + " MHz";
    lv_label_set_text(frequencyValueLabel, frequency.c_str());
}

/**
 * @brief Stores all configuration fields relevant to the active session.
 *
 * @return true when all relevant values are valid.
 * @return false otherwise.
 */
bool ScreenNodeConfig::storeInputs()
{
    if (context.configureNode == nullptr) return false;

    NodeConfig& config = context.configureNode->getConfig();

    if (config.radio == NodeRadio::NRF24 && !storeChannel()) return false;
    if (config.radio == NodeRadio::CC1101 && !storeFrequency()) return false;
    if (config.mode == NodeMode::Beacon && !storeInterval()) return false;

    return true;
}

/**
 * @brief Stores the selected NRF24 channel.
 *
 * @return true when the channel is valid.
 */
bool ScreenNodeConfig::storeChannel()
{
    if (context.configureNode == nullptr || channelInput == nullptr) return false;

    String value = lv_textarea_get_text(channelInput);
    int channel = value.toInt();

    if (value.length() == 0 || channel < MIN_NRF_CHANNEL || channel > MAX_NRF_CHANNEL) {
        if (channelLabel != nullptr) lv_label_set_text(channelLabel, "INVALID CHANNEL (2-80)");
        return false;
    }

    context.configureNode->getConfig().channel = static_cast<uint8_t>(channel);

    lv_label_set_text(channelLabel, "CHANNEL (2-80)");
    updateNRFFrequency();

    return true;
}

/**
 * @brief Stores the selected CC1101 frequency.
 *
 * @return true when the frequency is valid.
 */
bool ScreenNodeConfig::storeFrequency()
{
    if (context.configureNode == nullptr || frequencyInput == nullptr) return false;

    String value = lv_textarea_get_text(frequencyInput);
    float frequency = value.toFloat();

    if (value.length() == 0 || !isValidCC1101Frequency(frequency)) {
        if (frequencyLabel != nullptr) lv_label_set_text(frequencyLabel, "INVALID CC1101 FREQUENCY");
        return false;
    }

    context.configureNode->getConfig().frequency = frequency;

    lv_label_set_text(frequencyLabel, "FREQUENCY");

    return true;
}

/**
 * @brief Stores the selected beacon interval.
 *
 * @return true when the interval is valid.
 */
bool ScreenNodeConfig::storeInterval()
{
    if (context.configureNode == nullptr || intervalInput == nullptr) return false;

    String value = lv_textarea_get_text(intervalInput);
    long interval = value.toInt();

    if (value.length() == 0 || interval < MIN_INTERVAL || interval > MAX_INTERVAL) {
        if (intervalLabel != nullptr) lv_label_set_text(intervalLabel, "INVALID INTERVAL 100-60000");
        return false;
    }

    context.configureNode->getConfig().interval = static_cast<uint16_t>(interval);

    lv_label_set_text(intervalLabel, "INTERVAL (MS) 100-60000");

    return true;
}

/**
 * @brief Returns whether a frequency is supported by the CC1101.
 *
 * @param frequency Frequency in MHz.
 *
 * @return true when the frequency is inside a supported range.
 */
bool ScreenNodeConfig::isValidCC1101Frequency(float frequency)
{
    return (frequency >= 300.0f && frequency <= 348.0f) ||
           (frequency >= 387.0f && frequency <= 464.0f) ||
           (frequency >= 779.0f && frequency <= 928.0f);
}

/**
 * @brief Handles focus on a configuration input.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::inputFocused(lv_event_t* event)
{
    if (keyboard == nullptr) return;

    lv_obj_t* input = static_cast<lv_obj_t*>(lv_event_get_target(event));
    if (input == nullptr) return;

    lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_NUMBER);
    lv_keyboard_set_textarea(keyboard, input);
    lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_scroll_to_view(input, LV_ANIM_ON);
}

/**
 * @brief Handles completion or cancellation of keyboard input.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::keyboardFinished(lv_event_t* event)
{
    if (keyboard == nullptr) return;

    lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_READY) {
        lv_obj_t* input = lv_keyboard_get_textarea(keyboard);

        if (input == channelInput) storeChannel();
        else if (input == frequencyInput) storeFrequency();
        else if (input == intervalInput) storeInterval();
    }

    lv_keyboard_set_textarea(keyboard, nullptr);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief Updates the NRF24 frequency while the channel is edited.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::channelChanged(lv_event_t* event)
{
    updateNRFFrequency();
}

/**
 * @brief Replaces the configuration controls with a success message.
 */
void ScreenNodeConfig::showSuccess()
{
    if (context.screen == nullptr || context.configureNode == nullptr) return;

    lv_obj_clean(context.screen);

    radioButton = nullptr;
    modeButton = nullptr;
    channelLabel = nullptr;
    channelInput = nullptr;
    frequencyLabel = nullptr;
    frequencyValueLabel = nullptr;
    frequencyInput = nullptr;
    intervalLabel = nullptr;
    intervalInput = nullptr;
    startButton = nullptr;
    keyboard = nullptr;

    UIWidgets::addHeader(context.screen, 0, 0, "N0DE CONFIGURED");

    const BLEDeviceInfo& node = context.configureNode->getSelectedNode();
    const NodeConfig& config = context.configureNode->getConfig();

    String nodeName = node.name.length() > 0 ? node.name : "C3N0";
    UIWidgets::addText(context.screen, 15, 55, nodeName.c_str(), 210);
    UIWidgets::addText(context.screen, 15, 90, "NODE CONFIGURED", 210);

    String radio = "RADIO: " + String(ConfigureNode::getRadioName(config.radio));
    UIWidgets::addText(context.screen, 15, 120, radio.c_str(), 210);

    String mode = "MODE: " + String(ConfigureNode::getModeName(config.mode));
    UIWidgets::addText(context.screen, 15, 145, mode.c_str(), 210);

    String parameter;

    if (config.radio == NodeRadio::NRF24)
        parameter = "CH: " + String(config.channel) + " | " + String(2400 + config.channel) + " MHz";
    else if (config.radio == NodeRadio::CC1101)
        parameter = "FREQ: " + String(config.frequency, 3) + " MHz";

    if (config.mode == NodeMode::Beacon)
        parameter += "\nINTERVAL: " + String(config.interval) + " ms";

    UIWidgets::addText(context.screen, 15, 175, parameter.c_str(), 210);
    UIWidgets::addText(context.screen, 15, 225, "RESET NODE TO\nCONFIGURE AGAIN", 210);

    lv_obj_t* closeButton = UIWidgets::addButton(context.screen, 65, 270, "CLOSE", 110, 40);
    lv_obj_add_event_cb(closeButton, closeClicked, LV_EVENT_CLICKED, &context);
}

/**
 * @brief Handles the radio selection button.
 *
 * Cycles only between NRF24 and CC1101. BLE remains reserved for
 * future node functionality and is not exposed in this screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::radioClicked(lv_event_t* event)
{
    if (context.configureNode == nullptr) return;

    if (keyboard != nullptr) {
        lv_keyboard_set_textarea(keyboard, nullptr);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }

    NodeConfig& config = context.configureNode->getConfig();
    config.radio = config.radio == NodeRadio::NRF24 ? NodeRadio::CC1101 : NodeRadio::NRF24;

    updateControls();
}

/**
 * @brief Handles the mode selection button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::modeClicked(lv_event_t* event)
{
    if (context.configureNode == nullptr) return;

    if (keyboard != nullptr) {
        lv_keyboard_set_textarea(keyboard, nullptr);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }

    NodeConfig& config = context.configureNode->getConfig();
    config.mode = config.mode == NodeMode::Beacon ? NodeMode::Listen : NodeMode::Beacon;

    updateControls();
}

/**
 * @brief Handles the START button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::startClicked(lv_event_t* event)
{
    if (context.configureNode == nullptr) return;
    if (!storeInputs()) return;

    if (keyboard != nullptr) {
        lv_keyboard_set_textarea(keyboard, nullptr);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }

    Serial.println("[NODE CONFIG] START clicked.");

    if (!context.configureNode->sendConfig()) {
        Serial.println("[NODE CONFIG] sendConfig FAILED.");
        return;
    }

    Serial.println("[NODE CONFIG] Config sent.");

    if (!context.configureNode->startSession()) {
        Serial.println("[NODE CONFIG] startSession FAILED.");
        return;
    }

    Serial.println("[NODE CONFIG] START sent.");

    showSuccess();
}

/**
 * @brief Handles the CLOSE button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::closeClicked(lv_event_t* event)
{
    if (context.configureNode != nullptr) context.configureNode->stop();
    if (context.screenManager != nullptr) context.screenManager->home();
}

/**
 * @brief Handles the BACK button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::backClicked(lv_event_t* event)
{
    if (context.screenManager == nullptr) return;

    context.screenManager->back();
}

/**
 * @brief Clears static screen references when the screen is deleted.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::screenDeleted(lv_event_t* event)
{
    radioButton = nullptr;
    modeButton = nullptr;

    channelLabel = nullptr;
    channelInput = nullptr;

    frequencyLabel = nullptr;
    frequencyValueLabel = nullptr;
    frequencyInput = nullptr;

    intervalLabel = nullptr;
    intervalInput = nullptr;

    startButton = nullptr;
    keyboard = nullptr;

    context.screen = nullptr;
    context.screenManager = nullptr;
    context.configureNode = nullptr;
}