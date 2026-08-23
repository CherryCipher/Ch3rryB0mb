/**
 * @file screenpacketviewer.h
 * @brief Wi-Fi Packet Viewer screen interface.
 */

#pragma once

#include <lvgl.h>

class ScreenManager;
class PacketViewer;

/**
 * @class ScreenPacketViewer
 * @brief Displays passively captured Wi-Fi packet metadata.
 *
 * The screen automatically starts capture when an active Wi-Fi Station
 * connection exists.
 *
 * A fixed number of recent packet rows are displayed and reused while
 * capturing, avoiding unbounded LVGL object creation.
 *
 * Selecting a packet opens a detailed view using a copy of the selected
 * packet metadata.
 */
class ScreenPacketViewer
{
public:
    /**
     * @brief Creates the Wi-Fi Packet Viewer screen.
     *
     * @param screenManager Reference to the application ScreenManager.
     * @param packetViewer Reference to the Packet Viewer feature.
     *
     * @return Pointer to the created LVGL screen object.
     */
    static lv_obj_t* create(ScreenManager& screenManager, PacketViewer& packetViewer);

private:
    /**
     * @struct ScreenContext
     * @brief Context shared by Packet Viewer callbacks.
     */
    struct ScreenContext
    {
        ScreenManager* screenManager = nullptr;
        PacketViewer* packetViewer = nullptr;
    };

    /**
     * @brief Maximum number of packet rows displayed simultaneously.
     *
     * The feature retains more packets than are displayed so the UI can
     * remain compact without creating excessive LVGL objects.
     */
    static constexpr uint8_t MAX_VISIBLE_ROWS = 12;

    static ScreenContext screenContext;
    static lv_obj_t* packetContainer;
    static lv_obj_t* captureButton;
    static lv_obj_t* clearButton;
    static lv_obj_t* statusLabel;
    static lv_obj_t* emptyLabel;
    static lv_timer_t* refreshTimer;
    static uint8_t renderedRows;
    static bool detailVisible;

    /**
     * @brief Updates the visible packet list.
     *
     * Existing packet rows are reused and only their labels are changed.
     *
     * @param packetViewer Reference to the Packet Viewer feature.
     */
    static void updatePackets(PacketViewer& packetViewer);

    /**
     * @brief Updates one existing packet row.
     *
     * @param row Packet row button.
     * @param packetViewer Reference to the Packet Viewer feature.
     * @param index Packet index represented by the row.
     */
    static void updatePacketRow(lv_obj_t* row, PacketViewer& packetViewer, uint8_t index);

    /**
     * @brief Displays detailed information about the selected packet.
     *
     * @param packetViewer Reference to the Packet Viewer feature.
     */
    static void showPacketDetail(PacketViewer& packetViewer);

    /**
     * @brief Updates capture status text.
     *
     * @param packetViewer Reference to the Packet Viewer feature.
     */
    static void updateStatus(PacketViewer& packetViewer);

    /**
     * @brief Handles packet row selection.
     *
     * @param event Pointer to the LVGL event.
     */
    static void packetClicked(lv_event_t* event);

    /**
     * @brief Handles the START/STOP capture button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void captureClicked(lv_event_t* event);

    /**
     * @brief Handles the CLEAR button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void clearClicked(lv_event_t* event);

    /**
     * @brief Returns from packet detail to the packet list.
     *
     * @param event Pointer to the LVGL event.
     */
    static void detailBackClicked(lv_event_t* event);

    /**
     * @brief Opens WiFi Lab when packet capture cannot be used.
     *
     * @param event Pointer to the LVGL event.
     */
    static void wifiLabClicked(lv_event_t* event);

    /**
     * @brief Handles the Packet Viewer back button.
     *
     * @param event Pointer to the LVGL event.
     */
    static void backClicked(lv_event_t* event);

    /**
     * @brief Periodically refreshes the packet list and status.
     *
     * @param timer Pointer to the LVGL timer.
     */
    static void refreshTimerCallback(lv_timer_t* timer);

    /**
     * @brief Cleans up Packet Viewer screen resources.
     *
     * Packet capture is stopped but the Wi-Fi Station connection remains active.
     *
     * @param event Pointer to the LVGL delete event.
     */
    static void screenDeleted(lv_event_t* event);
};