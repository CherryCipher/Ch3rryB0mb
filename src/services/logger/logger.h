#pragma once

#include <Arduino.h>

//v1
//Simple logger class to log messages to the serial console which will later change to a file on the SD card. 
//For now, it will just log to the serial console.


/**
 * @brief Logger class for logging messages to the serial console.
 *
 * This class provides a simple interface for logging messages to the serial console.
 * In the future, this will be replaced with file-based logging on the SD card.
 *
 */
class Logger
{
public:
    /**
    * @brief Starts the serial console for logging.
    *
    * Example:
    * @code
    * logger.start();
    * @endcode
    */
    bool start();

    /**
    * @brief Logs a message to the serial console.
    *
    * Example:
    * @code
    * logger.info("Hello, world!");
    * @endcode
    */
    void info(const String& message);

    /**
    * @brief Logs a warning message to the serial console.
    *
    * Example:
    * @code
    * logger.warning("Warning: Something might be wrong!");
    * @endcode
    */
    void warning(const String& message);

        /**
    * @brief Logs an error message to the serial console.
    *
    * Example:
    * @code
    * logger.error("Error: Something went wrong!");
    * @endcode
    */
    void error(const String& message);

    /**
    * @brief Prints the application banner to the serial console.
    *
    * The actual banner content is defined in Banner.cpp namespace and can be modified there as needed.
    * 
    * Example:
    * @code
    * logger.printBanner();
    * @endcode
    */
    void printBanner();

    /**
    * @brief Returns whether the logger is currently active.
    *
    * Example:
    * @code
    * if(logger.isRunning())
    * {
    *    //logger is running properly, we can log messages to the serial console.
    * }
    * @endcode
    */
    bool isRunning() const;

    /**
    * @brief Stops the serial console for logging.
    *
    * Example:
    * @code
    * logger.stop();
    * @endcode
    */
    bool stop();

private:
    /**
    * @brief Boolean flag indicating whether the logger is currently running.
    *
    * Used to track the state of the logger and prevent logging when it is not active.
    */
    bool loggerRunning = false;
};