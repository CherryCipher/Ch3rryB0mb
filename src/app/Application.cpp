#include "Application.h"

    /**
     * @brief Constructs the application controller.
     *
     * Initializes all shared services and prepares the application
     * for startup.
     */
    Application::Application()    
        :   services(),
            features(services),
            screens(services.logger, features)
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

        if (!screens.start())
        {
            Serial.println("FATAL ERROR: Can't start ScreenManager");
            return false;
        }
        
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
        screens.stop();
    }