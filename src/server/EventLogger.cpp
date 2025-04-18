#include "server/EventLogger.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

EventLogger::EventLogger(const std::string& logFilePath) {
    logFile.open(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo de log: " + logFilePath);
    }
}

EventLogger::~EventLogger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void EventLogger::logEvent(const std::string& component, LogLevel level, const std::string& message) {
    std::string timestamp = getCurrentTimestamp();
    std::string levelStr = logLevelToString(level);
    logFile << "[" << timestamp << "] "
            << "[" << component << "] "
            << "[" << levelStr << "] "
            << message << std::endl;
}

std::string EventLogger::getCurrentTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::tm* tm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string EventLogger::logLevelToString(LogLevel level) const {
    switch (level) {
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}
