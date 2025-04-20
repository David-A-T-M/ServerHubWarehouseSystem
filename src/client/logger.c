#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static FILE* logFile = NULL;

bool initLogger(const char* filename) {
    logFile = fopen(filename, "a");
    return logFile != NULL;
}

void logEvent(const char* component, const char* message) {
    if (logFile) {
        time_t now = time(NULL);
        char* timestamp = ctime(&now);
        timestamp[strcspn(timestamp, "\n")] = '\0'; // Remove newline
        fprintf(logFile, "[%s] [%s] %s\n", timestamp, component, message);
        fflush(logFile); // Ensure immediate write
    }
}

void closeLogger() {
    if (logFile) {
        fclose(logFile);
        logFile = NULL;
    }
}
