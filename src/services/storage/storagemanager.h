#pragma once

#include <Arduino.h>
#include <SD.h>

#include "../logger/logger.h"

/**
 * @class StorageManager
 * @brief Provides a simple interface for persistent file storage.
 *
 * The StorageManager is responsible for initializing and accessing the
 * primary storage medium used by Ch3rryB0mb.
 *
 * The rest of the firmware should never communicate directly with the SD
 * library. All file operations should be performed through this manager.
 *
 * Responsibilities
 * ----------------
 * - Initialize the storage device.
 * - Check whether storage is available.
 * - Open files.
 * - Check whether files exist.
 *
 * Future
 * ------
 * The StorageManager is intentionally hardware independent. Although the
 * current implementation uses an SD card, it can later be replaced with
 * LittleFS, SPIFFS or another storage backend without changing the rest of
 * the firmware.
 */
class StorageManager
{
public:
    /**
     * @brief Constructs a new StorageManager.
     *
     * @param logger Reference to the application's Logger.
     */
    explicit StorageManager(Logger& logger);

    /**
     * @brief Initializes the storage device.
     *
     * Attempts to mount the SD card and prepares it for file access.
     *
     * @return true if the storage device was mounted successfully.
     * @return false otherwise.
     *
     */
    bool start();

    /**
     * @brief Returns whether the storage device is available.
     *
     * @return true if storage is mounted.
     */
    bool isMounted() const;

    /**
     * @brief Checks whether a file exists.
     *
     * @param path Absolute file path.
     * @return true if the file exists.
     */
    bool exists(const String& path) const;

    /**
     * @brief Opens a file.
     *
     * Attempts to open the specified file and stores the result in the supplied
     * File object.
     *
     * @param path Absolute file path.
     * @param file Reference to the File object that will receive the opened file.
     * @param mode File access mode (FILE_READ or FILE_WRITE).
     *
     * @return true if the file was opened successfully.
     * @return false otherwise.
     */
    bool open(const String& path, File& file, const char* mode = FILE_READ);

    /**
     * @brief Closes an opened file.
     *
     * Safely closes the supplied File object if it is open.
     *
     * @param file File to close.
     */
    void close(File& file);

    /**
    * @brief Stops the storage manager and unmounts the storage device.
    *
    * This function should be called when the application is shutting down or
    * when the storage device is no longer needed.
    *
    * @return true if the storage device was unmounted successfully.
    * @return false otherwise.
    */
    bool stop();

private:

    /**
    * Indicates whether the storage device is currently mounted.
    */
    bool mounted = false;

    /**
    * @brief Reference to the application's Logger instance.
    *
    * Used for logging messages related to web server operations.
    */
    Logger& logger;
};