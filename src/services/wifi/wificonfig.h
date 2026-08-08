#pragma once

#include <Arduino.h>

//v1
//Struct to hold the configuration for the WiFi access point


/**
 * @brief WIFIAPConfig struct holds the configuration for the WiFi access point.
 *
 * Hardcoded default values are provided for the SSID, password, channel, hidden status, and maximum number of clients.
 * Later this can be set by the user through the interface, but for now it is hardcoded.
 *
 */
struct WiFiAPConfig
{
    String ssid = "CherryLab";
    String password = "ch3rryb0mb";

    uint8_t channel = 6;

    bool hidden = false;

    uint8_t maxClients = 4;
};