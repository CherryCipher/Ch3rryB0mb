#include "features.h"
#include "../../services/services.h"

/**
 * @brief Constructs all application features.
 *
 * @param services Reference to the shared application services.
 */
Features::Features(Services& services)
    : apMode(services)
    , wifiLab(services)
    , nrfScanner(services)
{
}