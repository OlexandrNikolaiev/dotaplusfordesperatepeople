#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <QString>

#include "../Utils/Utility.h"

#define LOG(message) Logger::logWrapper(message, __FUNCTION__)

class Logger {
public:
    static Logger& getInstance();

    static void log(const std::string& message, const std::string& function);
    static void log(const QString& message, const std::string& function);

    static void logWrapper(const char* message, const std::string& function) {
        log(std::string(message), function);
    }

    template <typename T>
    static void logWrapper(const T& message, const std::string& function) {
        log(message, function);
    }

    static void logWrapper(const QString& message, const std::string& function) {
        log(message, function);
    }

private:
    Logger(const std::string& filename);
    ~Logger();
    void shutdown();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static std::string getCurrentDateTime();
    static std::ofstream _logFile;
};
