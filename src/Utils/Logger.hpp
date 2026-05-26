#pragma once
#include <string>
#include <mutex>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Utils {

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

// Singleton thread-safe logger. Zero external deps, writes to stdout/stderr.
class Logger {
public:
    static Logger& instance();

    void setLevel(LogLevel lvl) { min_level_ = lvl; }

    template<typename... Args>
    void log(LogLevel lvl, Args&&... args) {
        if (lvl < min_level_) return;
        std::ostringstream oss;
        (oss << ... << std::forward<Args>(args));
        emit(lvl, oss.str());
    }

    template<typename... Args> void debug(Args&&... a) { log(LogLevel::DEBUG, std::forward<Args>(a)...); }
    template<typename... Args> void info (Args&&... a) { log(LogLevel::INFO,  std::forward<Args>(a)...); }
    template<typename... Args> void warn (Args&&... a) { log(LogLevel::WARN,  std::forward<Args>(a)...); }
    template<typename... Args> void error(Args&&... a) { log(LogLevel::ERROR, std::forward<Args>(a)...); }

private:
    Logger() = default;
    void emit(LogLevel lvl, const std::string& msg);
    static const char* levelStr(LogLevel lvl);

    std::mutex mu_;
    LogLevel   min_level_{LogLevel::INFO};
};

// Convenience macro
#define LOG_INFO(...)  Utils::Logger::instance().info(__VA_ARGS__)
#define LOG_WARN(...)  Utils::Logger::instance().warn(__VA_ARGS__)
#define LOG_ERROR(...) Utils::Logger::instance().error(__VA_ARGS__)
#define LOG_DEBUG(...) Utils::Logger::instance().debug(__VA_ARGS__)

} // namespace Utils
