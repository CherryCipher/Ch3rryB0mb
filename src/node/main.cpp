#include <Arduino.h>

#include "nodeservices/nodeservices.h"

NodeServices services;

/**
 * @brief Initializes the Ch3rryN0de firmware.
 */
void setup()
{
    Serial.begin(115200);
    delay(500);

    services.start();
}

/**
 * @brief Main Ch3rryN0de firmware loop.
 */
void loop()
{
    delay(10);
}