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
 * The screen automatically starts packet capture when an active Wi-Fi
 * Station connection exists.
 *
 * Packet records are displayed using a limited number of reusable LVGL
 * objects to keep memory usage predictable.
 *
 * Selecting a packet opens a detailed representation of the captured
 * metadata.
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
     * Limiting the number of visible rows prevents unnecessary LVGL memory
     * usage while additional packet records remain available in the capture
     * ring buffer.
     */
    static constexpr uint8_t MAX_VISIBLE_ROWS = 12;

    /**
     * @brief Context shared by Packet Viewer callbacks.
     */
    static ScreenContext screenContext;

    /**
     * @brief Scrollable container displaying retained packets.
     */
    static lv_obj_t* packetContainer;

    /**
     * @brief START or STOP capture button.
     */
    static lv_obj_t* captureButton;

    /**
     * @brief Button used to clear retained packet records.
     */
    static lv_obj_t* clearButton;

    /**
     * @brief Button used to cycle through packet capture filters.
     */
    static lv_obj_t* filterButton;

    /**
     * @brief Label displaying packet ring buffer usage.
     */
    static lv_obj_t* statusLabel;

    /**
     * @brief Label displayed when no packet records are available.
     */
    static lv_obj_t* emptyLabel;

    /**
     * @brief Timer used to periodically refresh the packet list.
     */
    static lv_timer_t* refreshTimer;

    /**
     * @brief Number of packet rows currently created.
     */
    static uint8_t renderedRows;

    /**
     * @brief Indicates whether packet detail is currently displayed.
     */
    static bool detailVisible;

    /**
     * @brief Updates the visible packet list.
     *
     * Existing packet rows are reused and only their labels are updated.
     *
     * @param packetViewer Reference to the Packet Viewer feature.
     */
    static void updatePackets(PacketViewer& packetViewer);

    /**
     * @brief Updates one packet row.
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
     * @brief Updates the packet ring buffer usage indicator.
     *
     * @param packetViewer Reference to the Packet Viewer feature.
     */
    static void updateStatus(PacketViewer& packetViewer);

    /**
     * @brief Updates the packet filter button label.
     *
     * @param packetViewer Reference to the Packet Viewer feature.
     */
    static void updateFilterButton(PacketViewer& packetViewer);

    /**
     * @brief Handles packet row selection.
     *
     * @param event Pointer to the LVGL event.
     */
    static void packetClicked(lv_event_t* event);

    /**
     * @brief Handles the START or STOP capture button.
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
     * @brief Handles the packet capture filter button.
     *
     * Cycles through the available capture filters.
     *
     * @param event Pointer to the LVGL event.
     */
    static void filterClicked(lv_event_t* event);

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
     * @brief Periodically refreshes packet information.
     *
     * @param timer Pointer to the LVGL timer.
     */
    static void refreshTimerCallback(lv_timer_t* timer);

    /**
     * @brief Cleans up Packet Viewer screen resources.
     *
     * Packet capture resources are released while the active Wi-Fi Station
     * connection remains unchanged.
     *
     * @param event Pointer to the LVGL delete event.
     */
    static void screenDeleted(lv_event_t* event);
};