#include "features.h"
#include "../../services/services.h"

/**
 * @brief Constructs all application features.
 *
 * @param services Reference to the shared application services.
 */
Features::Features(Services& services)
    : services(services)
    , apMode(services)
    , wifiLab(services)
    , packetViewer(services)
    , bleExplorer(services)
    , bleFoxHunt(services)
    , nrfScanner(services)
    , cc1101Scanner(services)
{
}

/**
 * @brief Returns whether Ch3rryB0mb is currently connected to Wi-Fi.
 *
 * Retrieves the current Station connection state from the shared WiFiManager.
 *
 * @return true when the Wi-Fi Station interface is connected.
 * @return false when no Wi-Fi connection is active.
 */
bool Features::isWiFiConnected() const
{
    return services.wifi.isConnected();
}