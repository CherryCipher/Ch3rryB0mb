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
lv_obj_t* ScreenNodeConfig::parameterLabel = nullptr;

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

    const BLEDeviceInfo& node = configureNode.getSelectedNode();
    String nodeName = node.name.length() > 0 ? node.name : "C3N0";

    UIWidgets::addText(screen, 15, 50, nodeName.c_str(), 210);

    String nodeInfo = String(node.rssi) + " dBm | " + node.address;
    UIWidgets::addText(screen, 15, 70, nodeInfo.c_str(), 210);

    UIWidgets::addText(screen, 15, 100, "RADIO", 90);
    radioButton = UIWidgets::addButton(screen, 110, 92, "", 115, 35);
    lv_obj_add_event_cb(radioButton, radioClicked, LV_EVENT_CLICKED, &context);

    UIWidgets::addText(screen, 15, 145, "MODE", 90);
    modeButton = UIWidgets::addButton(screen, 110, 137, "", 115, 35);
    lv_obj_add_event_cb(modeButton, modeClicked, LV_EVENT_CLICKED, &context);

    parameterLabel = UIWidgets::addText(screen, 15, 190, "", 210);

    lv_obj_t* startButton = UIWidgets::addButton(screen, 65, 250, "START", 110, 45);
    lv_obj_add_event_cb(startButton, startClicked, LV_EVENT_CLICKED, &context);

    lv_obj_add_event_cb(screen, screenDeleted, LV_EVENT_DELETE, nullptr);

    updateControls();

    return screen;
}

/**
 * @brief Updates the configuration controls.
 */
void ScreenNodeConfig::updateControls()
{
    if (context.configureNode == nullptr) return;

    NodeConfig& config = context.configureNode->getConfig();

    if (radioButton != nullptr) {
        lv_obj_t* label = lv_obj_get_child(radioButton, 0);
        if (label != nullptr) lv_label_set_text(label, ConfigureNode::getRadioName(config.radio));
    }

    if (modeButton != nullptr) {
        lv_obj_t* label = lv_obj_get_child(modeButton, 0);
        if (label != nullptr) lv_label_set_text(label, ConfigureNode::getModeName(config.mode));
    }

    if (parameterLabel == nullptr) return;

    String parameter;

    switch (config.radio) {
        case NodeRadio::BLE:
            parameter = "BLE configuration follows";
            break;

        case NodeRadio::NRF24:
            parameter = "CHANNEL: " + String(config.channel);
            break;

        case NodeRadio::CC1101:
            parameter = "FREQ: " + String(config.frequency, 3) + " MHz";
            break;
    }

    lv_label_set_text(parameterLabel, parameter.c_str());
}

/**
 * @brief Replaces the configuration controls with a success message.
 *
 * Shows the configuration that was sent to the node and informs the user
 * that the node must be reset before it can be configured again.
 */
void ScreenNodeConfig::showSuccess()
{
    if (context.screen == nullptr || context.configureNode == nullptr) return;

    lv_obj_clean(context.screen);

    UIWidgets::addHeader(context.screen, 0, 0, "N0DE CONFIGURED");

    const BLEDeviceInfo& node = context.configureNode->getSelectedNode();
    const NodeConfig& config = context.configureNode->getConfig();

    String nodeName = node.name.length() > 0 ? node.name : "C3N0";
    UIWidgets::addText(context.screen, 15, 55, nodeName.c_str(), 210);

    UIWidgets::addText(context.screen, 15, 95, "NODE CONFIGURED", 210);

    String radio = "RADIO: " + String(ConfigureNode::getRadioName(config.radio));
    UIWidgets::addText(context.screen, 15, 130, radio.c_str(), 210);

    String mode = "MODE: " + String(ConfigureNode::getModeName(config.mode));
    UIWidgets::addText(context.screen, 15, 155, mode.c_str(), 210);

    String parameter;

    switch (config.radio) {
        case NodeRadio::BLE:
            parameter = "BLE";
            break;

        case NodeRadio::NRF24:
            parameter = "CHANNEL: " + String(config.channel);
            break;

        case NodeRadio::CC1101:
            parameter = "FREQ: " + String(config.frequency, 3) + " MHz";
            break;
    }

    UIWidgets::addText(context.screen, 15, 180, parameter.c_str(), 210);
    UIWidgets::addText(context.screen, 15, 215, "RESET NODE TO\nCONFIGURE AGAIN", 210);

    lv_obj_t* closeButton = UIWidgets::addButton(context.screen, 65, 270, "CLOSE", 110, 40);
    lv_obj_add_event_cb(closeButton, closeClicked, LV_EVENT_CLICKED, &context);

    radioButton = nullptr;
    modeButton = nullptr;
    parameterLabel = nullptr;
}

/**
 * @brief Handles the radio selection button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::radioClicked(lv_event_t* event)
{
    if (context.configureNode == nullptr) return;

    NodeConfig& config = context.configureNode->getConfig();

    switch (config.radio) {
        case NodeRadio::BLE: config.radio = NodeRadio::NRF24; break;
        case NodeRadio::NRF24: config.radio = NodeRadio::CC1101; break;
        case NodeRadio::CC1101: config.radio = NodeRadio::BLE; break;
    }

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

    NodeConfig& config = context.configureNode->getConfig();
    config.mode = config.mode == NodeMode::Beacon ? NodeMode::Listen : NodeMode::Beacon;

    updateControls();
}

/**
 * @brief Handles the start button.
 *
 * Sends the current node configuration followed by the START command.
 * When both operations succeed, the screen changes to a confirmation
 * state while BLE remains available until the user closes the screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::startClicked(lv_event_t* event)
{
    if (context.configureNode == nullptr) return;

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
 * @brief Handles the close button after successful configuration.
 *
 * Fully shuts down BLE before returning directly to the main menu. This
 * ensures Wi-Fi can safely use the shared ESP32 radio afterwards.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::closeClicked(lv_event_t* event)
{
    if (context.configureNode != nullptr) context.configureNode->stop();
    if (context.screenManager != nullptr) context.screenManager->home();
}

/**
 * @brief Handles the back button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::backClicked(lv_event_t* event)
{
    if (context.screenManager == nullptr) return;

    context.screenManager->back();
}

/**
 * @brief Cleans up static screen references.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeConfig::screenDeleted(lv_event_t* event)
{
    radioButton = nullptr;
    modeButton = nullptr;
    parameterLabel = nullptr;

    context.screen = nullptr;
    context.screenManager = nullptr;
    context.configureNode = nullptr;
}