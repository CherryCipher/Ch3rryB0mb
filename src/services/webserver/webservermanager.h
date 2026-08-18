#pragma once

#include <Arduino.h>
#include <WebServer.h>

#include "../storage/StorageManager.h"
#include "../logger/logger.h"
#include "Pages.h"

/**
 * @class WebServerManager
 * @brief Hosts the embedded Ch3rryB0mb web dashboard and REST API.
 *
 * The WebServerManager is responsible for serving the Ch3rryB0mb web
 * interface and processing incoming HTTP requests.
 *
 * Static dashboard files are loaded through the StorageManager, making the
 * web server independent from the underlying storage implementation.
 *
 * Responsibilities
 * ----------------
 * - Start and stop the embedded HTTP server.
 * - Register all HTTP routes.
 * - Serve static dashboard files.
 * - Handle REST API requests.
 * - Process incoming HTTP clients.
 * - Display an embedded fallback page if the dashboard is unavailable.
 *
 * Design
 * ------
 * The WebServerManager never communicates directly with the SD library.
 * All file operations are delegated to the StorageManager service.
 *
 * Typical lifecycle
 * -----------------
 * @code
 * StorageManager storage;
 * storage.start();
 *
 * WebServerManager web(storage);
 *
 * web.start();
 *
 * while (true)
 * {
 *     web.handleClients();
 * }
 * @endcode
 */
class WebServerManager
{
public:

    /**
     * @brief Constructs a new WebServerManager.
     *
     * @param storage Reference to the application's StorageManager.
     * @param logger Reference to the application's Logger.
     */
    explicit WebServerManager(StorageManager& storage, Logger& logger);

    /**
     * @brief Starts the embedded HTTP server.
     *
     * Registers all routes and begins listening for incoming HTTP
     * connections on port 80.
     *
     * @return true if the server started successfully.
     * @return false otherwise.
     */
    bool start();

    /**
     * @brief Stops the embedded HTTP server.
     *
     * Stops accepting incoming HTTP connections.
     */
    void stop();

    /**
     * @brief Processes incoming HTTP requests.
     *
     * This function should be called continuously from the application's
     * main loop.
     *
     */
    void handleClients();

    
    /**
     * @brief Returns true if the webserver is running
     * 
     * @return true if webserver is running.
     * @return false if otherwise.
     */
    bool isRunning() const;

private:

    /**
     * @brief Registers all HTTP routes.
     *
     * Configures all dashboard and API endpoints before the server
     * begins accepting client connections.
     */
    void registerRoutes();

    /**
     * @brief Handles the system status endpoint.
     *
     * Returns a JSON object containing basic information about the
     * running firmware.
     */
    void handleStatus();

    /**
     * @brief Serves static web files.
     *
     * Incoming URLs are translated into files located inside the
     * /web directory.
     *
     * If the requested dashboard cannot be found, an embedded fallback
     * page is served instead.
     *
     * Unknown files result in a standard HTTP 404 response.
     */
    void serveStaticFiles();

    /**
     * @brief Serves the embedded fallback dashboard.
     *
     * Displays a simple diagnostic page whenever the dashboard
     * files cannot be located on the storage device.
     */
    void serveFallback();

    /**
     * @brief Sends a standard HTTP 404 response.
     */
    void handleNotFound();

    /**
     * @brief Determines the MIME type of a file.
     *
     * Returns the appropriate HTTP Content-Type based on the file
     * extension.
     *
     * @param path Absolute file path.
     * @return MIME type string.
     */
    String getMimeType(const String& path);

private:
    /**
     * @brief Variable to check if webserver is running
     *
     * Used to check if the webserver is runnign to handle updating in services
     */
    bool running = false;

    /**
     * @brief Reference to the application's storage service.
     *
     * Used to access all dashboard files and other static web resources.
     */
    StorageManager& storage;

    /**
    * @brief Reference to the application's Logger instance.
    *
    * Used for logging messages related to web server operations.
    */
    Logger& logger;

    /**
     * @brief Embedded HTTP server.
     *
     * Listens for incoming HTTP requests on port 80.
     */
    WebServer server{80};
};