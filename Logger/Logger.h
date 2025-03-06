#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>

#include "../Utils/Utility.h"

#define LOG(message) Logger::log(message, __FUNCTION__)

class Logger { //todo
public:
    static Logger& getInstance();
    static void log(const std::string& message, const std::string& function);

private:
    Logger(const std::string& filename);
    ~Logger();
    void shutdown();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static std::string getCurrentDateTime();
    static std::ofstream _logFile;
};

