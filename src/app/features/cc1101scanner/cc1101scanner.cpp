/**
 * @file cc1101scanner.cpp
 * @brief Implementation of the CC1101 Sub-GHz scanner feature.
 */

#include "cc1101scanner.h"

/**
 * @brief Constructs a new CC1101Scanner.
 *
 * Stores a reference to the application's Services container.
 *
 * @param services Reference to the application's Services container.
 */
CC1101Scanner::CC1101Scanner(Services& services)
    : services(services)
{
}

/**
 * @brief Starts the CC1101 scanner feature.
 *
 * Verifies that the CC1101 radio is available before marking the
 * scanner as running.
 *
 * @return true if the scanner started successfully.
 * @return false if the CC1101 radio is unavailable.
 */
bool CC1101Scanner::start()
{
    if (running) return true;

    if (!isAvailable())
    {
        services.logger.error("CC1101Scanner unavailable: CC1101Manager is not running.");
        return false;
    }

    services.logger.info("Starting CC1101Scanner.");

    running = true;

    services.logger.info("CC1101Scanner started.");

    return true;
}

/**
 * @brief Stops the CC1101 scanner feature.
 *
 * Marks the scanner feature as inactive.
 *
 * @return true if the scanner stopped successfully.
 */
bool CC1101Scanner::stop()
{
    if (!running) return true;

    running = false;

    services.logger.info("CC1101Scanner stopped.");

    return true;
}

/**
 * @brief Returns whether the CC1101 scanner feature is available.
 *
 * The scanner is available when the CC1101Manager has successfully
 * initialized the CC1101 radio.
 *
 * @return true if the CC1101 radio is available.
 * @return false otherwise.
 */
bool CC1101Scanner::isAvailable() const
{
    return services.cc1101.isRunning();
}

/**
 * @brief Returns whether the scanner feature is currently running.
 *
 * @return true if the scanner is running.
 * @return false otherwise.
 */
bool CC1101Scanner::isRunning() const
{
    return running;
}