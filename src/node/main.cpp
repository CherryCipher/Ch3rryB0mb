#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("C3N0");
    Serial.println("Node firmware booted!");
}

void loop()
{
    delay(1000);
}