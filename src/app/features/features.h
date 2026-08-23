#pragma once

#include "apmode/apmode.h"
#include "wifilab/wifilab.h"
#include "nrfscanner/nrfscanner.h"
#include "cc1101scanner/cc1101scanner.h"
#include "bleexplorer/bleexplorer.h"
#include "blefoxhunt/blefoxhunt.h"
#include "packetviewer/packetviewer.h"

class Services;

/**
 * @class Features
 * @brief Container for all Ch3rryB0mb application features.
 *
 * Features combine one or more low-level services into
 * user-facing functionality.
 *
 * Each feature receives access to the shared Services container.
 */
class Features
{
public:
    /**
     * @brief Constructs all application features.
     *
     * @param services Reference to the shared application services.
     */
    explicit Features(Services& services);

    /**
     * @brief Access Point Mode feature.
     */
    APMode apMode;

    /**
     * @brief Wi-Fi Lab feature.
     */
    WiFiLab wifiLab;

    /**
     * @brief Returns whether Ch3rryB0mb is currently connected to Wi-Fi.
     *
     * Provides the UI layer with the current Wi-Fi connection state without
     * exposing the underlying Services or WiFiManager implementation.
     *
     * @return true when the Wi-Fi Station interface is connected.
     * @return false when no Wi-Fi connection is active.
     */
    bool isWiFiConnected() const;

    /**
     * @brief Wi-Fi Packet Viewer feature.
     */
    PacketViewer packetViewer;

    /**
     * @brief NRF Scanner feature.
     */
    NRFScanner nrfScanner;

    /**
     * @brief CC1101 Scanner feature.
     */
    CC1101Scanner cc1101Scanner;

    /**
     * @brief BLE Explorer feature.
     */
    BLEExplorer bleExplorer;

    /**
     * @brief BLE Fox Hunt feature.
     */
    BLEFoxHunt bleFoxHunt;

    
private:
    /**
     * @brief Reference to the shared application services.
     *
     * Used by the feature container to expose application-level state
     * without requiring UI components to access Services directly.
     */
    Services& services;
};