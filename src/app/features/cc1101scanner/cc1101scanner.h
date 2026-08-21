/**
 * @file cc1101scanner.h
 * @brief Declaration of the CC1101 Sub-GHz scanner feature.
 *
 * The CC1101Scanner provides the feature layer used by the Sub-GHz
 * scanner screen and uses the application's services to access the
 * CC1101 radio and logger.
 */

#pragma once

#include "services/services.h"

/**
 * @class CC1101Scanner
 * @brief Feature for scanning Sub-GHz radio activity using the CC1101.
 *
 * The scanner provides the application feature layer between the UI
 * and the CC1101Manager.
 */
class CC1101Scanner
{
public:
    /**
     * @brief Constructs a new CC1101Scanner.
     *
     * @param services Reference to the application's Services container.
     */
    explicit CC1101Scanner(Services& services);

    /**
     * @brief Starts the CC1101 scanner feature.
     *
     * The feature can only start when the CC1101Manager is available.
     *
     * @return true if the scanner started successfully.
     * @return false if the CC1101 radio is unavailable.
     */
    bool start();

    /**
     * @brief Stops the CC1101 scanner feature.
     *
     * @return true if the scanner stopped successfully.
     */
    bool stop();

    /**
     * @brief Returns whether the CC1101 scanner feature is available.
     *
     * @return true if the CC1101Manager is running.
     * @return false otherwise.
     */
    bool isAvailable() const;

    /**
     * @brief Returns whether the scanner feature is currently running.
     *
     * @return true if the scanner is running.
     * @return false otherwise.
     */
    bool isRunning() const;

private:
    /** @brief Application services used by the scanner feature. */
    Services& services;

    /** @brief Indicates whether the scanner feature is currently running. */
    bool running = false;
};