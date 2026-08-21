#pragma once

#include "logger/Logger.h"
#include "spi/spimanager.h"
#include "storage/StorageManager.h"
#include "wifi/WiFiManager.h"
#include "webserver/WebServerManager.h"
#include "UI/uimanager.h"
#include "nrf/nrfmanager.h"
#include "cc1101/cc1101manager.h"

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
     */
    Services();

    /**
     * @brief Starts every application service.
     *
     * Services are started in dependency order.
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
     */
    void update();

    /**
     * @brief Stops all running services.
     *
     * Services are stopped in reverse startup order.
     */
    void stop();


public:

    /**
     * @brief Shared application logger.
     */
    Logger logger;

    /**
     * @brief Shared SPI manager.
     */
    SPIManager spi;

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
     * @brief Shared embedded NRFManager.
     */
    NRFManager nrf;

    /**
     * @brief Shared CC1101 Sub-GHz radio manager.
     */
    CC1101Manager cc1101;

private:

    /**
     * @brief Flag to check for down services
     */
    bool serviceDown = false;

};