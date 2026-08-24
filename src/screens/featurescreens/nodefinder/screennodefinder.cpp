/**
 * @file screennodefinder.cpp
 * @brief Implementation of the Ch3rryN0de finder screen.
 */

#include "screennodefinder.h"

#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"

ScreenNodeFinder::Context ScreenNodeFinder::context;
ScreenNodeFinder::NodeButtonContext ScreenNodeFinder::nodeButtonContexts[ConfigureNode::MAX_NODES];

lv_obj_t* ScreenNodeFinder::nodeContainer = nullptr;
lv_obj_t* ScreenNodeFinder::statusLabel = nullptr;
lv_timer_t* ScreenNodeFinder::refreshTimer = nullptr;

uint8_t ScreenNodeFinder::renderedNodeCount = 0;

/**
 * @brief Creates the Ch3rryN0de finder screen.
 *
 * @param screenManager Reference to the application ScreenManager.
 * @param configureNode Reference to the ConfigureNode feature.
 *
 * @return Pointer to the created LVGL screen.
 */
lv_obj_t* ScreenNodeFinder::create(ScreenManager& screenManager, ConfigureNode& configureNode)
{
    lv_obj_t* screen = UIWidgets::createScreen();

    context.screenManager = &screenManager;
    context.configureNode = &configureNode;

    UIWidgets::addHeader(screen, 0, 0, "FIND N0DE");

    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &context);

    lv_obj_t* scanButton = UIWidgets::addButton(screen, 15, 50, "SCAN", 90, 35);
    lv_obj_add_event_cb(scanButton, scanClicked, LV_EVENT_CLICKED, &context);

    statusLabel = UIWidgets::addText(screen, 115, 58, "IDLE | 0", 115);
    nodeContainer = UIWidgets::createScrollContainer(screen, 10, 95, 220, 210);

    renderedNodeCount = 0;

    UIWidgets::addText(nodeContainer, 5, 5, "Searching for Ch3rryN0des...", 195);

    refreshTimer = lv_timer_create(refreshTimerCallback, 500, &context);
    lv_obj_add_event_cb(screen, screenDeleted, LV_EVENT_DELETE, &context);

    startScan(configureNode);

    return screen;
}

/**
 * @brief Starts a new node scan.
 *
 * @param configureNode Reference to the ConfigureNode feature.
 */
void ScreenNodeFinder::startScan(ConfigureNode& configureNode)
{
    if (nodeContainer != nullptr) {
        lv_obj_clean(nodeContainer);
        UIWidgets::addText(nodeContainer, 5, 5, "Searching for Ch3rryN0des...", 195);
    }

    renderedNodeCount = 0;

    if (!configureNode.startScan() && nodeContainer != nullptr) {
        lv_obj_clean(nodeContainer);
        UIWidgets::addText(nodeContainer, 5, 5, "Node scan failed.", 195);
    }

    updateStatus(configureNode);
}

/**
 * @brief Updates the displayed node list.
 *
 * @param configureNode Reference to the ConfigureNode feature.
 */
void ScreenNodeFinder::updateNodes(ConfigureNode& configureNode)
{
    configureNode.update();

    uint8_t nodeCount = configureNode.getNodeCount();

    if (nodeCount == 0) return;

    if (renderedNodeCount == 0 && nodeContainer != nullptr) lv_obj_clean(nodeContainer);

    for (uint8_t i = renderedNodeCount; i < nodeCount; i++) {
        const BLEDeviceInfo& node = configureNode.getNode(i);

        String name = node.name.length() > 0 ? node.name : "C3N0";
        if (name.length() > 18) name = name.substring(0, 15) + "...";

        String text = name + "\n" + String(node.rssi) + " dBm";

        nodeButtonContexts[i].index = i;

        lv_obj_t* button = UIWidgets::addButton(nodeContainer, 5, 5 + (i * 58), text.c_str(), 195, 52);
        lv_obj_add_event_cb(button, nodeClicked, LV_EVENT_CLICKED, &nodeButtonContexts[i]);
    }

    renderedNodeCount = nodeCount;
}

/**
 * @brief Updates the scan status label.
 *
 * @param configureNode Reference to the ConfigureNode feature.
 */
void ScreenNodeFinder::updateStatus(ConfigureNode& configureNode)
{
    if (statusLabel == nullptr) return;

    String status = configureNode.isScanning() ? "SCAN" : "IDLE";
    status += " | ";
    status += configureNode.getNodeCount();

    lv_label_set_text(statusLabel, status.c_str());
}

/**
 * @brief Handles a discovered node button event.
 *
 * Selects the requested node, stops discovery and opens the node
 * configuration screen while keeping BLE available for communication.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeFinder::nodeClicked(lv_event_t* event)
{
    NodeButtonContext* buttonContext = static_cast<NodeButtonContext*>(lv_event_get_user_data(event));

    if (buttonContext == nullptr) return;
    if (context.configureNode == nullptr || context.screenManager == nullptr) return;
    if (!context.configureNode->selectNode(buttonContext->index)) return;

    context.configureNode->stopScan();
    context.screenManager->show(Screen::NodeConfig);
}

/**
 * @brief Handles the scan button event.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeFinder::scanClicked(lv_event_t* event)
{
    Context* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    if (ctx == nullptr || ctx->configureNode == nullptr) return;

    startScan(*ctx->configureNode);
}

/**
 * @brief Handles the back button event.
 *
 * Stops the node discovery feature, releases BLE resources and returns
 * to the previous screen.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeFinder::backClicked(lv_event_t* event)
{
    Context* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    if (ctx == nullptr || ctx->screenManager == nullptr) return;

    if (ctx->configureNode != nullptr) ctx->configureNode->stop();

    ctx->screenManager->back();
}

/**
 * @brief Periodically refreshes discovered nodes.
 *
 * @param timer Pointer to the LVGL timer.
 */
void ScreenNodeFinder::refreshTimerCallback(lv_timer_t* timer)
{
    Context* ctx = static_cast<Context*>(lv_timer_get_user_data(timer));
    if (ctx == nullptr || ctx->configureNode == nullptr) return;

    updateNodes(*ctx->configureNode);
    updateStatus(*ctx->configureNode);
}

/**
 * @brief Cleans up screen resources when the screen is deleted.
 *
 * Stops an active discovery scan but deliberately keeps BLE available
 * because the next screen may continue communicating with the selected
 * node.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenNodeFinder::screenDeleted(lv_event_t* event)
{
    Context* ctx = static_cast<Context*>(lv_event_get_user_data(event));

    if (ctx != nullptr && ctx->configureNode != nullptr) ctx->configureNode->stopScan();

    if (refreshTimer != nullptr) {
        lv_timer_delete(refreshTimer);
        refreshTimer = nullptr;
    }

    nodeContainer = nullptr;
    statusLabel = nullptr;
    renderedNodeCount = 0;

    context.screenManager = nullptr;
    context.configureNode = nullptr;
}