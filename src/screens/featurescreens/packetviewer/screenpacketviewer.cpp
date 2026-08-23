/**
 * @file screenpacketviewer.cpp
 * @brief Implementation of the Wi-Fi Packet Viewer screen.
 */

#include "screenpacketviewer.h"
#include "screens/screenmanager.h"
#include "../../uiwidgets/uiwidget.h"
#include "app/features/packetviewer/packetviewer.h"

ScreenPacketViewer::ScreenContext ScreenPacketViewer::screenContext;

lv_obj_t* ScreenPacketViewer::packetContainer = nullptr;
lv_obj_t* ScreenPacketViewer::captureButton = nullptr;
lv_obj_t* ScreenPacketViewer::clearButton = nullptr;
lv_obj_t* ScreenPacketViewer::statusLabel = nullptr;
lv_obj_t* ScreenPacketViewer::emptyLabel = nullptr;

lv_timer_t* ScreenPacketViewer::refreshTimer = nullptr;

uint8_t ScreenPacketViewer::renderedRows = 0;
bool ScreenPacketViewer::detailVisible = false;

/**
 * @brief Creates the Wi-Fi Packet Viewer screen.
 *
 * @param screenManager Reference to the application ScreenManager.
 * @param packetViewer Reference to the Packet Viewer feature.
 *
 * @return Pointer to the created LVGL screen object.
 */
lv_obj_t* ScreenPacketViewer::create(ScreenManager& screenManager, PacketViewer& packetViewer)
{
    lv_obj_t* screen = UIWidgets::createScreen();

    UIWidgets::addHeader(screen, 0, 0, "P4CKET VIEWER");

    lv_obj_t* backButton = UIWidgets::addButton(screen, 150, 5, "< BACK", 80, 30);
    lv_obj_add_event_cb(backButton, backClicked, LV_EVENT_CLICKED, &screenManager);

    screenContext.screenManager = &screenManager;
    screenContext.packetViewer = &packetViewer;

    renderedRows = 0;
    detailVisible = false;

    if (!packetViewer.isConnected())
    {
        UIWidgets::addText(screen, 15, 75, "Please connect first through the WiFi Lab.", 210);

        lv_obj_t* wifiButton = UIWidgets::addButton(screen, 15, 145, "> WIFI LAB", 210, 40);
        lv_obj_add_event_cb(wifiButton, wifiLabClicked, LV_EVENT_CLICKED, &screenManager);

        lv_obj_add_event_cb(screen, screenDeleted, LV_EVENT_DELETE, &packetViewer);

        return screen;
    }

    captureButton = UIWidgets::addButton(screen, 15, 50, "STOP", 90, 35);
    lv_obj_add_event_cb(captureButton, captureClicked, LV_EVENT_CLICKED, &packetViewer);

    clearButton = UIWidgets::addButton(screen, 115, 50, "CLEAR", 60, 35);
    lv_obj_add_event_cb(clearButton, clearClicked, LV_EVENT_CLICKED, &packetViewer);

    statusLabel = UIWidgets::addText(screen, 180, 58, "0", 50);

    packetContainer = UIWidgets::createScrollContainer(screen, 10, 95, 220, 210);
    emptyLabel = UIWidgets::addText(packetContainer, 5, 5, "Listening for Wi-Fi traffic...", 195);

    packetViewer.clear();

    if (!packetViewer.start())
    {
        lv_label_set_text(emptyLabel, "Packet capture failed.");

        lv_obj_t* label = lv_obj_get_child(captureButton, 0);
        if (label != nullptr) lv_label_set_text(label, "START");
    }

    refreshTimer = lv_timer_create(refreshTimerCallback, 500, &packetViewer);

    lv_obj_add_event_cb(screen, screenDeleted, LV_EVENT_DELETE, &packetViewer);

    updateStatus(packetViewer);

    return screen;
}

/**
 * @brief Updates the visible packet list.
 *
 * Creates only the number of packet rows required and reuses those rows
 * afterwards by updating their label text.
 *
 * @param packetViewer Reference to the Packet Viewer feature.
 */
void ScreenPacketViewer::updatePackets(PacketViewer& packetViewer)
{
    if (packetContainer == nullptr || detailVisible)
        return;

    uint8_t packetCount = packetViewer.getPacketCount();
    uint8_t visibleCount = min(packetCount, MAX_VISIBLE_ROWS);

    if (visibleCount == 0)
    {
        if (emptyLabel == nullptr)
            emptyLabel = UIWidgets::addText(packetContainer, 5, 5, packetViewer.isRunning() ? "Listening for Wi-Fi traffic..." : "Capture stopped.", 195);

        return;
    }

    if (emptyLabel != nullptr)
    {
        lv_obj_delete(emptyLabel);
        emptyLabel = nullptr;
    }

    for (uint8_t i = 0; i < visibleCount; i++)
    {
        lv_obj_t* row = nullptr;

        if (i < renderedRows)
            row = lv_obj_get_child(packetContainer, i);
        else
        {
            row = UIWidgets::addButton(packetContainer, 5, 5 + (i * 58), "", 195, 52);
            lv_obj_add_event_cb(row, packetClicked, LV_EVENT_CLICKED, reinterpret_cast<void*>(static_cast<uintptr_t>(i)));
        }

        if (row != nullptr)
            updatePacketRow(row, packetViewer, i);
    }

    renderedRows = visibleCount;
}

/**
 * @brief Updates one packet row.
 *
 * @param row Packet row button.
 * @param packetViewer Reference to the Packet Viewer feature.
 * @param index Packet index represented by the row.
 */
void ScreenPacketViewer::updatePacketRow(lv_obj_t* row, PacketViewer& packetViewer, uint8_t index)
{
    WiFiPacketInfo packet = packetViewer.getPacket(index);

    String protocol = PacketViewer::getProtocolName(packet.protocol);

    if (packet.protocol == WiFiPacketProtocol::None)
        protocol = PacketViewer::getFrameSubtypeName(packet.frameType, packet.frameSubtype);

    String text =
        "#" + String(packet.id) + "  " +
        protocol + "  " +
        String(packet.length) + " B\n" +
        String(packet.rssi) + " dBm | CH " +
        String(packet.channel) + " | " +
        PacketViewer::getFrameTypeName(packet.frameType);

    lv_obj_t* label = lv_obj_get_child(row, 0);

    if (label != nullptr)
        lv_label_set_text(label, text.c_str());
}

/**
 * @brief Displays detailed information about the selected packet.
 *
 * @param packetViewer Reference to the Packet Viewer feature.
 */
void ScreenPacketViewer::showPacketDetail(PacketViewer& packetViewer)
{
    if (packetContainer == nullptr || !packetViewer.hasSelectedPacket())
        return;

    detailVisible = true;

    lv_obj_clean(packetContainer);

    const WiFiPacketInfo& packet = packetViewer.getSelectedPacket();

    String text =
        "PACKET #" + String(packet.id) + "\n\n" +
        "TYPE: " + PacketViewer::getFrameTypeName(packet.frameType) + "\n" +
        "SUB: " + PacketViewer::getFrameSubtypeName(packet.frameType, packet.frameSubtype) + "\n" +
        "PROTOCOL: " + PacketViewer::getProtocolName(packet.protocol) + "\n" +
        "RSSI: " + String(packet.rssi) + " dBm\n" +
        "CHANNEL: " + String(packet.channel) + "\n" +
        "LENGTH: " + String(packet.length) + " B\n\n" +
        "RX: " + PacketViewer::formatMAC(packet.receiver) + "\n" +
        "TX: " + PacketViewer::formatMAC(packet.transmitter);

    if (packet.sourceIP != 0 || packet.destinationIP != 0)
    {
        text +=
            "\n\nSRC IP: " + PacketViewer::formatIP(packet.sourceIP) +
            "\nDST IP: " + PacketViewer::formatIP(packet.destinationIP);
    }

    if (packet.sourcePort != 0 || packet.destinationPort != 0)
    {
        text +=
            "\nSRC PORT: " + String(packet.sourcePort) +
            "\nDST PORT: " + String(packet.destinationPort);
    }

    if (packet.protectedFrame)
        text += "\n\nPAYLOAD: PROTECTED";

    UIWidgets::addText(packetContainer, 5, 5, text.c_str(), 195);

    lv_obj_t* listButton = UIWidgets::addButton(packetContainer, 5, 245, "< PACKETS", 195, 40);
    lv_obj_add_event_cb(listButton, detailBackClicked, LV_EVENT_CLICKED, &packetViewer);
}

/**
 * @brief Updates Packet Viewer status text.
 *
 * @param packetViewer Reference to the Packet Viewer feature.
 */
void ScreenPacketViewer::updateStatus(PacketViewer& packetViewer)
{
    if (statusLabel == nullptr)
        return;

    String status = String(packetViewer.getPacketCount()) + "/16";
    lv_label_set_text(statusLabel, status.c_str());
}

/**
 * @brief Handles packet row selection.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenPacketViewer::packetClicked(lv_event_t* event)
{
    if (screenContext.packetViewer == nullptr)
        return;

    uint8_t index = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(lv_event_get_user_data(event)));

    if (!screenContext.packetViewer->selectPacket(index))
        return;

    showPacketDetail(*screenContext.packetViewer);
}

/**
 * @brief Handles the START/STOP capture button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenPacketViewer::captureClicked(lv_event_t* event)
{
    PacketViewer* packetViewer = static_cast<PacketViewer*>(lv_event_get_user_data(event));

    if (packetViewer == nullptr)
        return;

    if (packetViewer->isRunning())
        packetViewer->stop();
    else
        packetViewer->start();

    lv_obj_t* label = lv_obj_get_child(captureButton, 0);

    if (label != nullptr)
        lv_label_set_text(label, packetViewer->isRunning() ? "STOP" : "START");

    updateStatus(*packetViewer);
}

/**
 * @brief Handles the CLEAR button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenPacketViewer::clearClicked(lv_event_t* event)
{
    PacketViewer* packetViewer = static_cast<PacketViewer*>(lv_event_get_user_data(event));

    if (packetViewer == nullptr || packetContainer == nullptr)
        return;

    packetViewer->clear();

    lv_obj_clean(packetContainer);

    renderedRows = 0;
    detailVisible = false;

    emptyLabel = UIWidgets::addText(packetContainer, 5, 5, packetViewer->isRunning() ? "Listening for Wi-Fi traffic..." : "Capture stopped.", 195);

    updateStatus(*packetViewer);
}

/**
 * @brief Returns from packet detail to the packet list.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenPacketViewer::detailBackClicked(lv_event_t* event)
{
    PacketViewer* packetViewer = static_cast<PacketViewer*>(lv_event_get_user_data(event));

    if (packetViewer == nullptr || packetContainer == nullptr)
        return;

    lv_obj_clean(packetContainer);

    renderedRows = 0;
    emptyLabel = nullptr;
    detailVisible = false;

    updatePackets(*packetViewer);
}

/**
 * @brief Opens WiFi Lab.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenPacketViewer::wifiLabClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    screenManager->show(Screen::WifiLab);
}

/**
 * @brief Handles the Packet Viewer back button.
 *
 * @param event Pointer to the LVGL event.
 */
void ScreenPacketViewer::backClicked(lv_event_t* event)
{
    ScreenManager* screenManager = static_cast<ScreenManager*>(lv_event_get_user_data(event));

    if (screenManager == nullptr)
        return;

    screenManager->back();
}

/**
 * @brief Periodically refreshes the visible packet list.
 *
 * @param timer Pointer to the LVGL timer.
 */
void ScreenPacketViewer::refreshTimerCallback(lv_timer_t* timer)
{
    PacketViewer* packetViewer = static_cast<PacketViewer*>(lv_timer_get_user_data(timer));

    if (packetViewer == nullptr)
        return;

    if (!packetViewer->isConnected())
    {
        packetViewer->stop();
        return;
    }

    updatePackets(*packetViewer);
    updateStatus(*packetViewer);
}

/**
 * @brief Cleans up Packet Viewer screen resources.
 *
 * Stops packet capture and releases the dynamically allocated packet
 * buffer while preserving the active Wi-Fi Station connection.
 *
 * @param event Pointer to the LVGL delete event.
 */
void ScreenPacketViewer::screenDeleted(lv_event_t* event)
{
    PacketViewer* packetViewer = static_cast<PacketViewer*>(lv_event_get_user_data(event));

    if (packetViewer != nullptr)
        packetViewer->shutdown();

    if (refreshTimer != nullptr)
    {
        lv_timer_delete(refreshTimer);
        refreshTimer = nullptr;
    }

    screenContext.screenManager = nullptr;
    screenContext.packetViewer = nullptr;

    packetContainer = nullptr;
    captureButton = nullptr;
    clearButton = nullptr;
    statusLabel = nullptr;
    emptyLabel = nullptr;

    renderedRows = 0;
    detailVisible = false;
}