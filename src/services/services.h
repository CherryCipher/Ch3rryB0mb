#pragma once

#include "logger/Logger.h"
#include "storage/StorageManager.h"
#include "wifi/WiFiManager.h"
#include "webserver/WebServerManager.h"
#include "GUI/uimanager.h"
#include "GUI/screenmanager.h"

/**
 * @class Services
 * @brief Central container for all shared application services.
 *
 * The Services class owns and initializes every shared service used by
 * Ch3rryB0mb.
 *
 * Rather than creating service objects throughout the application,
 * every subsystem receives references to the shared instances stored
 * inside this class.
 *
 * Responsibilities
 * ----------------
 * - Own all shared services.
 * - Construct services in the correct dependency order.
 * - Start all services during application startup.
 * - Stop all services during application shutdown.
 *
 * Design
 * ------
 * Services acts as the application's Composition Root.
 *
 * All service dependencies are injected through constructors instead
 * of being created inside individual classes.
 *
 * Typical lifecycle
 * -----------------
 * @code
 * Services services;
 *
 * void setup()
 * {
 *     services.start();
 * }
 *
 * void loop()
 * {
 *     services.update();
 * }
 * @endcode
 */
class Services
{
public:

    /**
     * @brief Constructs the application's services.
     *
     * Creates every shared service and injects the required dependencies.
     *
     * Construction order
     * ------------------
     * Logger
     *      │
     *      ▼
     * StorageManager
     *      │
     *      ▼
     * WiFiManager
     *      │
     *      ▼
     * WebServerManager
     */
    Services();

    /**
     * @brief Starts every application service.
     *
     * Services are started in dependency order.
     *
     * Startup order
     * -------------
     * @code
     * Logger
     *      │
     *      ▼
     * Storage
     *      │
     *      ▼
     * WiFi
     *      │
     *      ▼
     * Web Server
     * @endcode
     *
     * @return true if all services started successfully.
     * @return false if one or more services failed.
     */
    bool start();

    /**
     * @brief Updates all services.
     *
     * This function should be called continuously from the application's
     * main loop.
     *
     * Example
     * -------
     * @code
     * void loop()
     * {
     *     services.update();
     * }
     * @endcode
     */
    void update();

    /**
     * @brief Stops all running services.
     *
     * Services are stopped in reverse startup order.
     */
    void stop();

    /**
     * @brief Starts AP mode on the Ch3rryB0mb
     *
     * Automatically starts AP mode, and starts the webserver
     * 
     * @return true if all services started successfully.
     * @return false if one or more services failed.
     */
    bool startAPMode();


public:

    /**
     * @brief Shared application logger.
     */
    Logger logger;

    /**
     * @brief Shared storage service.
     */
    StorageManager storage;

    /**
     * @brief Shared WiFi service.
     */
    WiFiManager wifi;

    /**
     * @brief Shared embedded web server.
     */
    WebServerManager web;

    /**
     * @brief Shared embedded UIManager.
     */
    UIManager ui;

    /**
     * @brief Shared embedded ScreenManager.
     */
    ScreenManager screens;
};