#ifndef LOGGER_H
 #define LOGGER_H

 #include <stdbool.h>

 /**
  * @brief Initializes the logger with an output file.
  *
  * @param filename Name of the file where logs will be written.
  * @return true if the file was successfully opened, false otherwise.
  */
 bool initLogger(const char* filename);

 /**
  * @brief Logs an event with a timestamp and component.
  *
  * @param component Component name (e.g., "Client", "Server", "Inventory").
  * @param message Message to log.
  */
 void logEvent(const char* component, const char* message);

 /**
  * @brief Closes the log file.
  */
 void closeLogger();

 #endif //LOGGER_H
