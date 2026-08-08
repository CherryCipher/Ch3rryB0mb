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
Application app;

  /**
   * @brief Arduino setup function.
   *
   * This function is called once when the microcontroller starts.
   * It initializes the application by calling the start() method.
   */
  void setup()
  {
      app.start();
  }

  /**
   * @brief Arduino loop function.
   *
   * This function is called repeatedly in an infinite loop by the Arduino framework.
   * It continuously updates the application by calling the update() method.
   */
  void loop()
  {
      app.update();
  }