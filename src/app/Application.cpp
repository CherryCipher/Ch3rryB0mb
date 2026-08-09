#include "Application.h"

    /**
     * @brief Constructs the application controller.
     *
     * Initializes all shared services and prepares the application
     * for startup.
     */
    Application::Application()
    {
    }

    /**
     * @brief Starts the application.
     *
     * Initializes all services and prepares the application for
     * operation.
     *
     * @return true if the application started successfully.
     * @return false otherwise.
     */
    bool Application::start()
    {
        if (!services.start())
        {
            return false;
        }

        //
        // Temporary startup behavior.
        // Later this will become the BootManager/UI.
        //
        // First create a default WiFiAPConfig object with the default values. Then start the AP with the default config.
        // Initialy I would love to check if any external modules are connected to the ESP32
        // if no modules are connected, automatically start in AP mode and serve the captive portal 
        WiFiAPConfig config;
        services.wifi.startAP(config);

        return true;
    }

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
    void Application::update()
    {
        // Update all services in the application.
        services.update();
    }

    /**
     * @brief Stops the application.
     *
     * Shuts down all services and prepares the application for
     * termination.
     */
    void Application::stop()
    {
        services.stop();
    }