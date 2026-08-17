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
        Serial.println("Application::Start - We are staring the app");

        if (!services.start())
        {
            Serial.println("FATAL ERROR CAN'T START SERVICES");
            return false;
        }

        //
        // Temporary startup behavior.
        // Later this will become the BootManager/UI.
        //
        // First create a default WiFiAPConfig object with the default values. Then start the AP with the default config.
        // Initialy I would love to check if any external modules are connected to the ESP32
        // if no modules are connected, automatically start in AP mode and serve the captive portal 
        if (!services.startAPMode())
        {
            Serial.println("========== FATAL ERROR ==========");
            Serial.println("FATAL ERROR CAN'T AP MODE");
            return false;
        }
        else
            Serial.println("========== AP MODE ACTIVATED ==========");
        

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