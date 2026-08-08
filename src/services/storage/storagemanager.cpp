#include "StorageManager.h"

    /**
     * @brief Constructs a new StorageManager.
     *
     * Stores a reference to the application's Logger.
     *
     * @param logger Reference to the application's Logger.
     */
    StorageManager::StorageManager(Logger& logger)
        : logger(logger)
    {
    }

    /**
     * @brief Initializes the storage device.
     *
     * Attempts to mount the SD card and prepares it for file access.
     * using the SD library (SD.h).
     *
     * Workflow
     * --------
     * @code
     * Mount SD card
     *        │
     *        ▼
     * Update mount state
     *        │
     *        ▼
     * Return success
     * @endcode
     *
     * @return true if the SD card was mounted successfully.
     * @return false otherwise.
     */
    bool StorageManager::start()
    {
        logger.info("Storagemanager started.");

        mounted = SD.begin();

        if(mounted)
            logger.info("Storage mounted successfully.");
        else
            logger.error("Storage mount failed.");

        return mounted;
    }

    /**
     * @brief Returns whether the storage device is mounted.
     *
     * @return true if storage is available.
     * @return false otherwise.
     */
    bool StorageManager::isMounted() const
    {
        return mounted;
    }

    /**
     * @brief Checks whether a file exists.
     *
     * Returns false if the storage device is unavailable.
     * using the SD library (SD.h).
     *
     * @param path Absolute file path.
     * @return true if the file exists.
     * @return false otherwise.
     */
    bool StorageManager::exists(const String& path) const
    {
        if (!mounted)
        {
            logger.error("Storage not mounted. Cannot check file existence.");
            return false;
        }

        logger.info("File exists: " + path);
        return SD.exists(path);
    }

    /**
     * @brief Opens a file.
     *
     * Attempts to open the specified file and stores the result in the supplied
     * File object.
     *
     * Workflow
     * --------
     * @code
     * Check storage
     *        │
     *        ▼
     * Open file
     *        │
     *        ▼
     * Return success
     * @endcode
     *
     * @param path Absolute file path.
     * @param file Reference to the File object.
     * @param mode File access mode.
     *
     * @return true if the file was opened successfully.
     * @return false otherwise.
     */
    bool StorageManager::open(const String& path, File& file, const char* mode)
    {
        if (!mounted)
        {
            logger.error("Storage not mounted. Cannot open file.");
            return false;
        }

        file = SD.open(path, mode);
        logger.info("File opened: " + path);

        return file;
    }

    /**
     * @brief Closes an opened file.
     *
     * Safely closes the supplied file if it is currently open. If the File
     * object is invalid or already closed, no action is taken.
     * we do this becasue we do not want other parts of the code to open and close files, 
     * we want to keep track of the files that are open and closed in the storage manager.
     *
     * Workflow
     * --------
     * @code
     * Check file validity
     *         │
     *         ▼
     * Close file
     *         │
     *         ▼
     * Return
     * @endcode
     *
     * Example
     * -------
     * @code
     * File file;
     *
     * if (services.storage.open("/web/index.html", file))
     * {
     *     // Use the file...
     *
     *     services.storage.close(file);
     * }
     * @endcode
     *
     * @param file Reference to the File object to close.
     */
    void StorageManager::close(File& file)
    {
        if (file)
        {
            file.close();
            logger.info("File closed: " + file);
        }
    }

    /**
     * @brief Stops the storage manager and unmounts the storage device.
     *
     * This function should be called when the application is shutting down or
     * when the storage device is no longer needed.
     *
     * Workflow
     * --------
     * @code
     * Check mount state
     *         │
     *         ▼
     * Unmount storage
     *         │
     *         ▼
     * Update mount state
     *         │
     *         ▼
     * Return success
     * @endcode
     *
     * @return true if the storage device was unmounted successfully.
     * @return false otherwise.
     */
    bool StorageManager::stop()
    {
        if (mounted)
        {
            SD.end();
            mounted = false;
            logger.info("Storage unmounted successfully.");
            logger.info("StorageManager stopped.");

            return true;
        }
        else
        {
            logger.warning("Storage was not mounted. Nothing to unmount.");
            logger.info("StorageManager stopped.");

            return false;
        }
    }