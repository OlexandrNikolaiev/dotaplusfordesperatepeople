#include "Logger.h"

#define ENABLE_DEBUG_OUTPUT 1
#define ENABLE_CONSOLE_OUTPUT 0

std::ofstream Logger::_logFile;

Logger& Logger::getInstance() {
    if (ENABLE_DEBUG_OUTPUT) {
        static Logger instance("debug.log");
        return instance;
    }
}

Logger::Logger(const std::string& filename) {
    _logFile.open(filename, std::ios::out);
    if (!_logFile.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
    } else {
        _logFile << getCurrentDateTime() << "[LOG START]" << std::endl;
    }
}

Logger::~Logger() {
    if (_logFile.is_open()) {
        shutdown();
    }
}

void Logger::log(const std::string& message, const std::string& function) {
    if (_logFile.is_open()) {
        std::string logMessage = getCurrentDateTime() + "[" + function + "] " + message;
        if (ENABLE_CONSOLE_OUTPUT) {
            std::cout << logMessage << std::endl;
        }
        _logFile << logMessage << std::endl;
    }
}

void Logger::log(const QString& message, const std::string& function) {
    log(message.toStdString(), function);
}

std::string Logger::getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm tm_now;
    localtime_s(&tm_now, &now);

    std::ostringstream oss;
    oss << "[" << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S") << "] ";
    return oss.str();
}

void Logger::shutdown() {
    if (_logFile.is_open()) {
        _logFile << getCurrentDateTime() << "[LOG END]" << std::endl;
        _logFile.flush();

        if (!_logFile.good()) {
            std::cerr << "Log file was not closed properly" << std::endl;
        }
        _logFile.close();
    }
}
