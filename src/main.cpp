#include <Arduino.h>

#include "app/Application.h"

/**
 * @file main.cpp
 * @brief Entry point for the Ch3rryB0mb firmware.
 *
 * This file contains the setup and loop functions required by the Arduino framework.
 * It initializes the application and continuously updates it in the main loop.
 *
 * @note The application lifecycle is managed by the Application class defined in src/app/Application.h.
 */
Application* app = nullptr;

/**
 * @brief Arduino setup function.
 *
 * This function is called once when the microcontroller starts.
 * It initializes the application by calling the start() method.
 */
void setup()
{
    // Open a serial connection to the host computer for logging and debugging.
    // A bootstrapper of sorts.
    Serial.begin(115200);

    app = new Application();

    if (app == nullptr)
    {
        Serial.println();
        Serial.println("========== FATAL ERROR ==========");
        Serial.println("Failed to allocate Application.");
        Serial.println("System halted.");
        Serial.println("=================================");

        while (true)
        {
            delay(1000);
            Serial.println("System halted. FATAL ERROR: Failed to allocate Application.");
        }
    }

    if (!app->start())
    {
        Serial.println();
        Serial.println("========== FATAL ERROR ==========");
        Serial.println("Application failed to start.");
        Serial.println("System halted.");
        Serial.println("=================================");

        // This is a fatal error, so we halt the system by entering an infinite loop.
        while (true)
        {
            delay(1000);
            Serial.println("System halted. FATAL ERROR: Application failed to start.");
        }
    }
    else
    {
        Serial.println("=========== ENHANCE ============");
        Serial.println("Application started successfully.");
        Serial.println("Welcome to Ch3rryB0mb!");
        Serial.println("=================================");
    }
}

/**
 * @brief Arduino loop function.
 *
 * This function is called repeatedly in an infinite loop by the Arduino framework.
 * It continuously updates the application by calling the update() method.
 */
void loop()
{
    if (app == nullptr) return;

    app->update();
}