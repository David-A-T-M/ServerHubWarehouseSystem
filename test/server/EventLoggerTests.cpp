#include "server/EventLogger.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <cstdio>

class EventLoggerTest : public ::testing::Test {
protected:
    std::string logFile = "test_log.log";

    void SetUp() override {
        std::remove(logFile.c_str());
    }

    void TearDown() override {
        std::remove(logFile.c_str());
    }

    std::string getLastLogEntry() {
        std::ifstream file(logFile);
        std::string line, lastLine;
        while (std::getline(file, line)) {
            lastLine = line;
        }
        return lastLine;
    }
};

TEST_F(EventLoggerTest, ConstructorCreatesLogFile) {
    EventLogger logger(logFile);
    std::ifstream file(logFile);
    ASSERT_TRUE(file.is_open());
}

TEST_F(EventLoggerTest, ConstructorFailsWithInvalidPath) {
    ASSERT_THROW(EventLogger("/invalid/path/to/log.log"), std::runtime_error);
}

TEST_F(EventLoggerTest, LogEventWritesExpectedOutput) {
    EventLogger logger(logFile);
    logger.logEvent("TestComponent", EventLogger::INFO, "Hello world!");
    std::string last = getLastLogEntry();
    EXPECT_NE(last.find("[TestComponent]"), std::string::npos);
    EXPECT_NE(last.find("[INFO]"), std::string::npos);
    EXPECT_NE(last.find("Hello world!"), std::string::npos);
}

TEST_F(EventLoggerTest, LogIncludesFormattedTimestamp) {
    EventLogger logger(logFile);
    logger.logEvent("X", EventLogger::INFO, "msg");
    std::string last = getLastLogEntry();
    ASSERT_GE(last.size(), 20);
    EXPECT_EQ(last[0], '[');
    EXPECT_EQ(last[5], '-');
    EXPECT_EQ(last[8], '-');
    EXPECT_EQ(last[11], ' ');
    EXPECT_EQ(last[14], ':');
    EXPECT_EQ(last[17], ':');
    EXPECT_EQ(last[20], ']');
}

TEST_F(EventLoggerTest, LogsAllLevelsCorrectly) {
    EventLogger logger(logFile);
    logger.logEvent("X", EventLogger::INFO, "info msg");
    logger.logEvent("X", EventLogger::WARNING, "warn msg");
    std::string last = getLastLogEntry();
    EXPECT_NE(last.find("warn msg"), std::string::npos);
    EXPECT_NE(last.find("[WARNING]"), std::string::npos);
    logger.logEvent("X", EventLogger::ERROR, "error msg");
    last = getLastLogEntry();
    EXPECT_NE(last.find("error msg"), std::string::npos);
    EXPECT_NE(last.find("[ERROR]"), std::string::npos);
}

TEST_F(EventLoggerTest, HandlesEmptyComponentAndMessage) {
    EventLogger logger(logFile);
    logger.logEvent("", EventLogger::WARNING, "");
    std::string last = getLastLogEntry();
    EXPECT_NE(last.find("[]"), std::string::npos);
    EXPECT_NE(last.find("[WARNING]"), std::string::npos);
}