#pragma once

#include <string>
#include <fstream>

/**
 * @class EventLogger
 * @brief A utility class for logging events to a file with timestamps and log levels.
 *
 * The `EventLogger` class provides functionality to log messages with a timestamp,
 * component name, and log level (INFO, WARNING, or ERROR) to a specified log file.
 */
class EventLogger {
    public:
        /**
         * @enum LogLevel
         * @brief Represents the severity level of a log message.
         */
        enum LogLevel { INFO, WARNING, ERROR };
    
        /**
         * @brief Constructs an EventLogger object.
         *
         * Opens the specified log file for appending log messages. If no file path
         * is provided, it defaults to "logs/system.log".
         *
         * @param logFilePath The path to the log file. Defaults to "logs/system.log".
         * @throws std::runtime_error If the log file cannot be opened.
         */
        explicit EventLogger(const std::string& logFilePath = "logs/system.log");

        /**
             * @brief Destroys the EventLogger object.
             *
             * Closes the log file if it is open.
             */
        ~EventLogger();

        /**
         * @brief Logs an event to the log file.
         *
         * Writes a log entry with the current timestamp, component name, log level,
         * and message to the log file.
         *
         * @param component The name of the component generating the log.
         * @param level The severity level of the log message.
         * @param message The log message to be written.
         */
        void logEvent(const std::string& component, LogLevel level, const std::string& message);

    private:
        std::ofstream logFile; /**< The output file stream for the log file. */

        /**
         * @brief Gets the current timestamp as a string.
         *
         * Generates a timestamp in the format "YYYY-MM-DD HH:MM:SS".
         *
         * @return A string representing the current timestamp.
         */
        std::string getCurrentTimestamp() const;

        /**
         * @brief Converts a log level to its string representation.
         *
         * Maps the `LogLevel` enum to a corresponding string (e.g., "INFO", "WARNING", "ERROR").
         *
         * @param level The log level to convert.
         * @return A string representation of the log level.
         */
        std::string logLevelToString(LogLevel level) const;
};
