#pragma once

#include "../services/services.h"
#include "features/features.h"
#include "../screens/screenmanager.h"

/**
 * @class Application
 * @brief Main application controller.
 *
 * Coordinates the firmware lifecycle.
 */
class Application
{
public:

    /**
     * @brief Constructs the application controller.
     *
     * Initializes all shared services and prepares the application
     * for startup.
     */
    Application();

    /**
     * @brief Starts the application.
     *
     * Initializes all services and prepares the application for
     * operation.
     *
     * @return true if the application started successfully.
     * @return false otherwise.
    */
    bool start();

    /**
     * @brief Updates the application.
     *
     * This function should be called continuously from the application's
     * main loop.
     *
     * Example
     * -------
     * @code
     * void loop()
     * {
     *     app.update();
     * }
     * @endcode
     */
    void update();

    /**
     * @brief Stops the application.
     *
     * Shuts down all services and prepares the application for
     * termination.
     */
    void stop();

private:
    /**
     * @brief Shared application services.
     *
     * Provides access to all shared services used by the application.
     * found in src/services/services.h.
     */
    Services services;

    /**
     * @brief Shared Features.
     */
    Features features;

    /**
     * @brief Shared ScreenManager.
     */
    ScreenManager screens;
};