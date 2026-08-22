#pragma once

#include "apmode/apmode.h"
#include "wifilab/wifilab.h"
#include "nrfscanner/nrfscanner.h"
#include "cc1101scanner/cc1101scanner.h"

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
     * @brief NRF Scanner feature.
     */
    NRFScanner nrfScanner;

    /**
     * @brief CC1101 Scanner feature.
     */
    CC1101Scanner cc1101Scanner;
};