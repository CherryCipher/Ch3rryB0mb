#include "storagemanager.h"

/**
 * @brief Constructs a new StorageManager.
 *
 * Stores references to the application's Logger and shared SPIManager.
 *
 * @param logger Reference to the application's Logger.
 * @param spiManager Reference to the shared SPIManager.
 */
StorageManager::StorageManager(Logger& logger, SPIManager& spiManager)
    : logger(logger), spiManager(spiManager)
{
}

/**
 * @brief Initializes the storage device.
 *
 * Attempts to mount the SD card using the shared SPI bus and prepares
 * it for file access.
 *
 * @return true if the SD card was mounted successfully.
 * @return false otherwise.
 */
bool StorageManager::start()
{
    logger.info("StorageManager started.");

    if (!spiManager.isRunning())
    {
        logger.error("SPIManager is not running.");
        return false;
    }

    mounted = SD.begin(SD_CS, spiManager.getHardwareBus());

    if (mounted)
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
 *
 * @param path Absolute file path.
 *
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

    return SD.exists(path);
}

/**
 * @brief Opens a file.
 *
 * Attempts to open the specified file and stores the result in the supplied
 * File object.
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

    if (file)
        logger.info("File opened: " + path);
    else
        logger.error("Failed to open file: " + path);

    return file;
}

/**
 * @brief Closes an opened file.
 *
 * Safely closes the supplied file if it is currently open.
 *
 * @param file Reference to the File object to close.
 */
void StorageManager::close(File& file)
{
    if (!file)
        return;

    file.close();

    logger.info("File closed.");
}

/**
 * @brief Stops the storage manager and unmounts the storage device.
 *
 * @return true if the storage device was unmounted successfully.
 * @return false otherwise.
 */
bool StorageManager::stop()
{
    if (!mounted)
    {
        logger.warning("Storage was not mounted. Nothing to unmount.");
        logger.info("StorageManager stopped.");
        return false;
    }

    SD.end();
    mounted = false;

    logger.info("Storage unmounted successfully.");
    logger.info("StorageManager stopped.");

    return true;
}