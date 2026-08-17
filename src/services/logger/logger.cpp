#include "logger.h"
#include "banner.h"

    /**
    * @brief Starts the serial console for logging.
    *
    * Workflow
    * --------
    * @code
    * Start serial console
    *        │
    *        ▼
    * Logger is ready for use
    * @endcode
    *
    */
    bool Logger::start()
    {
        if (loggerRunning)
            return false;

        //This does not need to be started aghain
        //Serial.begin(115200);

        //Unfortunately, Serial.begin() does not provide a way to check if the serial console started successfully.
        loggerRunning = true;


        info("[INFO] Logger started.");
        return true;
    }

    /**
    * @brief Simple logger class for logging messages to the serial console.
    *
    * Workflow
    * --------
    * @code
    * none yet, later logging to a file on the SD card will be implemented here.
    * @endcode
    *
    * @param message The message to log.
    */
    void Logger::info(const String& message)
    {
        if (!loggerRunning)
            return;

        //Print the message to the serial console for debugging purposes.
        Serial.println(message);
    }


    /**
    * @brief Simple warning logger class for logging messages to the serial console.
    *
    * Workflow
    * --------
    * @code
    * none yet, later logging to a file on the SD card will be implemented here.
    * @endcode
    *
    * @param message The message to log.
    */
    void Logger::warning(const String& message)
    {
        if (!loggerRunning)
            return;

        Serial.print("[WARNING] ");
        //Print the message to the serial console for debugging purposes.
        Serial.println(message);
    }

    /**
    * @brief Simple error logger class for logging messages to the serial console.
    *
    * Workflow
    * --------
    * @code
    * none yet, later logging to a file on the SD card will be implemented here.
    * @endcode
    *
    * @param message The message to log.
    */
    void Logger::error(const String& message)
    {
        if (!loggerRunning)
            return;

        Serial.print("[ERROR] ");
        //Print the message to the serial console for debugging purposes.
        Serial.println(message);
    }

    /**
    * @brief Prints the application banner to the serial console.
    *
    * Example:
    * @code
    * logger.printBanner();
    * @endcode
    */
    void Logger::printBanner()
    {
        if (!loggerRunning)
            return;

        //The c3b0 banner is printed to the serial console for debugging purposes.
        //The banner is defined in the Banner (banner.cpp) namespace and is compiled into the firmware.
        Serial.println(Banner::BANNER);
    }

    /**
    * @brief Checks if the logger is currently running.
    *
    * @return true if the logger is running, false otherwise.
    */
    bool Logger::isRunning() const
    {
        return loggerRunning;
    }

    /**
    * @brief Stops the serial console for logging.
    * 
    * @return true if the logger stopped successfully, false otherwise.
    */
    bool Logger::stop()
    {
        //Fiels can be closed here if logging to a file on the SD card is implemented in the future.
        //Other things can be done here to stop the logger if needed.
        loggerRunning = false;
        return true;
    }