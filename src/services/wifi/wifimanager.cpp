#include "WiFiManager.h"

//V1
//Wifimanager class implementation. This class is responsible for managing the WiFi access point and modes.
//The WifiManger manages wifi for the entire Ch3rryB0mb system.
//It is responsible for starting and stopping the access point, as well as managing the WiFi modes (AP, STA, etc.).

    /**
     * @brief Constructs a new WiFiManager.
     *
     * Stores a reference to the application's Logger.
     *
     * @param logger Reference to the application's Logger.
     */
    WiFiManager::WiFiManager(Logger& logger)
        : logger(logger)
    {
    }

    /**
     * @brief Initializes the WiFiManager.
     *
     * Wifi mode is initially set to WIFI_OFF to ensure a known state. Applications should explicitly start the desired mode (AP, STA, etc.) after initialization.
     * start() is called during the initialization of the services.wifi object in the Services class constructor.
     */
    bool WiFiManager::start()
    {
        //Initialy we turn wifi off, and set the apRunning flag to false. This is to ensure that we start in a known state.
        WiFi.mode(WIFI_OFF);
        logger.info("WiFiManager started.");
        logger.info("WiFi mode set to WIFI_OFF. Wi-Fi radio disabled.");

        return true;
    }

    /**
     * @brief Returns if the C3B0 is in Access Point mode.
     *
     * Uses the ESP32 WiFi library to check if the device is currently in Access Point mode.
     * WIFI_AP is the mode for Access Point only, while WIFI_AP_STA is the mode for both Access Point and Station.
     *
     */
    bool WiFiManager::isAPRunning() const
    {
        return (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA);
    }

    /**
     * @brief Starts the ESP32 in Access Point (SoftAP) mode.
     *
     * Configures the Wi-Fi radio as an Access Point using the supplied
     * configuration and begins broadcasting the configured SSID.
     * 
     * SoftAP() is a native function from the ESP32 Wi-Fi library that handles the underlying implementation of starting an AP.
     * 
     * If you want the AP to serve a web page, you will need to set up a web server separately. 
     * This function only starts the AP and does not handle any web server functionality.
     * Webserver is started though webservermanager, which is a separate service that can be started after the AP is running.
     *
     * @param config The Access Point configuration.
     * @return true if the Access Point started successfully.
     * @return false if the SoftAP failed to start.
     */
    bool WiFiManager::startAP(const WiFiAPConfig& config)
    {
        // Configure the ESP32 Wi-Fi radio for Access Point mode.
        // This sets the Wi-Fi mode to WIFI_AP, which enables the device to act as an Access Point.
        // We need to do this because in begin() we set the mode to WIFI_OFF.
        // so we need to set it to WIFI_AP before starting the SoftAP.
        WiFi.mode(WIFI_AP);
        logger.info("WiFi mode set to WIFI_AP. Starting SoftAP");

        // Start the SoftAP using the supplied configuration.
        // This config comes from the WiFiAPConfig struct, which is passed in as a parameter.
        bool success = WiFi.softAP(
            config.ssid.c_str(),
            config.password.c_str(),
            config.channel,
            config.hidden,
            config.maxClients
        );

        logger.info("SoftAP started with SSID: " + config.ssid + ", Channel: " + String(config.channel) + ", Hidden: " + String(config.hidden) + ", Max Clients: " + String(config.maxClients));

        // True if the SoftAP started successfully, false otherwise.
        return success;
    }

    /**
     * @brief Stops the Access Point and disables the Wi-Fi.
     *
     * Disconnects all connected clients, stops the SoftAP and powers down
     * the Wi-Fi radio to return the ESP32 to an idle state.
     *
     */
    void WiFiManager::stopAP()
    {
        // Disconnect all connected clients and stop the Access Point.
        WiFi.softAPdisconnect(true);
        logger.info("SoftAP stopped. All clients disconnected.");

        // Disable the ESP32 Wi-Fi radio.
        // Since the AP stops we also set the mode to WIFI_OFF to ensure we are in a known state.
        WiFi.mode(WIFI_OFF);
        logger.info("Wifi mode set to WIFI_OFF. Wi-Fi radio disabled.");
    }

    /**
     * @brief Returns the IP address of the Access Point.
     *
     * Calls the ESP32 Wi-Fi library to retrieve the IP address assigned to the Access Point interface.
     *
     */
    IPAddress WiFiManager::getAPIP() const
    {
        return WiFi.softAPIP();
    }

    /**
     * @brief Stops the WiFiManager and disables all Wi-Fi functionality.
     *
     * This function stops the Access Point if it is running and then disables
     * the Wi-Fi radio to return the ESP32 to an idle state.
     *
     */
    bool WiFiManager::stop()
    {
        // Stop the Access Point if it is running.
        if (isAPRunning())
        {
            stopAP();
        }

        // Disable the Wi-Fi radio.
        WiFi.mode(WIFI_OFF);
        logger.info("WiFiManager stopped. Wi-Fi radio disabled.");

        return true;
    }