#include "Logger.hpp"

namespace Utils {

Logger& Logger::instance() {
    static Logger inst;
    return inst;
}

void Logger::emit(LogLevel lvl, const std::string& msg) {
    auto now  = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::lock_guard lk(mu_);
    auto& out = (lvl >= LogLevel::WARN) ? std::cerr : std::cout;
    out << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
        << " [" << levelStr(lvl) << "] "
        << msg << '\n';
}

const char* Logger::levelStr(LogLevel lvl) {
    switch (lvl) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
    }
    return "?????";
}

} // namespace Utils
