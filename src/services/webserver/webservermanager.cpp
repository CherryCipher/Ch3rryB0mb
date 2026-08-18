#include "WebServerManager.h"

/**
 * @brief Constructs a new WebServerManager.
 *
 * Stores a reference to the application's StorageManager. All static
 * dashboard files will be loaded through this service.
 *
 * @param storage Reference to the application's StorageManager.
 * @param logger Reference to the application's Logger.
 */
WebServerManager::WebServerManager(StorageManager& storage, Logger& logger)
    : storage(storage), logger(logger)
{
}

/**
 * @brief Starts the embedded HTTP server.
 *
 * Registers all HTTP routes and starts listening for incoming client
 * connections.
 *
 * @return true if the server started successfully.
 * @return false otherwise.
 */
bool WebServerManager::start()
{
    registerRoutes();

    server.begin();
    logger.info("WebServerManager started. Web server started on port 80.");

    //set running to true
    running = true;

    return true;
}

/**
 * @brief Stops the embedded HTTP server.
 *
 * Stops accepting incoming HTTP requests.
 */
void WebServerManager::stop()
{
    server.stop();
    logger.info("Web server stopped.");

    //set running to false
    running = false;
}

/**
 * @brief Check to see if webserver is running
 *
 * @return true if the server is running
 * @return false otherwise.
 */
bool WebServerManager::isRunning() const
{
    return running;
}

/**
 * @brief Processes incoming HTTP requests.
 *
 * This function should be called continuously from the application's
 * main loop.
 */
void WebServerManager::handleClients()
{
    server.handleClient();
}

/**
 * @brief Registers all HTTP routes.
 *
 * Configures all REST API endpoints and static content handlers.
 */
void WebServerManager::registerRoutes()
{
    server.on("/api/status", HTTP_GET, [this]()
    {
        handleStatus();
    });

    server.onNotFound([this]()
    {
        serveStaticFiles();
    });
}

/**
 * @brief Handles the system status endpoint.
 *
 * Returns a small JSON object describing the running firmware.
 */
void WebServerManager::handleStatus()
{
    server.send(
        200,
        "application/json",
        R"({
        "status":"online",
        "device":"Ch3rryB0mb"
        })"
    );
}

/**
 * @brief Serves static web files.
 *
 * Requests for "/" are translated to "/web/index.html".
 *
 * All other requests are mapped directly into the /web directory.
 *
 * If the dashboard is unavailable, the embedded fallback page is shown.
 * Unknown files return a standard HTTP 404 response.
 */
void WebServerManager::serveStaticFiles()
{
    // Dashboard request
    // this opens the webserver page in /web/index.html
    if (server.uri() == "/")
    {
        File file;

        if (storage.open("/web/index.html", file))
        {
            server.streamFile(file, "text/html");

            storage.close(file);

            return;
        }

        serveFallback();

        return;
    }

    // Static file request
    String path = "/web" + server.uri();

    File file;

    if (storage.open(path, file))
    {
        server.streamFile(file, getMimeType(path));

        storage.close(file);

        return;
    }

    handleNotFound();
}

/**
 * @brief Serves the embedded fallback dashboard.
 *
 * This page is displayed whenever the dashboard files are missing
 * or the storage device is unavailable.
 * 
 * Page is loaded from the Pages namespace and is compiled into the firmware.
 * 
 */
void WebServerManager::serveFallback()
{
    server.send(
        200,
        "text/html",
        Pages::FALLBACK
    );

    logger.warning("Fallback dashboard served. Storage device may be unavailable.");
}

/**
 * @brief Sends a standard HTTP 404 response.
 *
 * Returned whenever a requested resource does not exist.
 * Page is loaded from the Pages namespace and is compiled into the firmware.
 * 
 */
void WebServerManager::handleNotFound()
{
    server.send(
        404,
        "text/html",
        Pages::ERROR404
    );

    logger.error("404 Not Found: " + server.uri());
}

/**
 * @brief Determines the MIME type of a file.
 *
 * Returns the appropriate HTTP Content-Type based on the supplied
 * file extension.
 *
 * @param path Absolute file path.
 * @return MIME type string.
 */
String WebServerManager::getMimeType(const String& path)
{
    if (path.endsWith(".html"))  return "text/html";
    if (path.endsWith(".css"))   return "text/css";
    if (path.endsWith(".js"))    return "application/javascript";
    if (path.endsWith(".json"))  return "application/json";

    if (path.endsWith(".png"))   return "image/png";
    if (path.endsWith(".jpg"))   return "image/jpeg";
    if (path.endsWith(".jpeg"))  return "image/jpeg";
    if (path.endsWith(".gif"))   return "image/gif";
    if (path.endsWith(".svg"))   return "image/svg+xml";
    if (path.endsWith(".ico"))   return "image/x-icon";

    if (path.endsWith(".woff"))  return "font/woff";
    if (path.endsWith(".woff2")) return "font/woff2";
    if (path.endsWith(".ttf"))   return "font/ttf";

    return "application/octet-stream";
}