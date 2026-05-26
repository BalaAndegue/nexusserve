#include "ConfigManager.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>

// Minimal JSON parsing — replace with nlohmann/json if the config grows.
namespace Utils {

static std::string jsonValueOf(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return {};
    pos = json.find(':', pos);
    if (pos == std::string::npos) return {};
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '"')) ++pos;
    auto end = json.find_first_of(",}\"\n", pos);
    return json.substr(pos, end - pos);
}

Config ConfigManager::load(const std::string& json_path) {
    Config cfg;

    std::ifstream f(json_path);
    if (f) {
        std::ostringstream ss;
        ss << f.rdbuf();
        std::string json = ss.str();

        if (auto v = jsonValueOf(json, "port"); !v.empty())
            cfg.port = static_cast<uint16_t>(std::stoi(v));
        if (auto v = jsonValueOf(json, "public_root"); !v.empty())
            cfg.public_root = v;
        if (auto v = jsonValueOf(json, "thread_pool_size"); !v.empty())
            cfg.thread_pool_size = std::stoi(v);
        if (auto v = jsonValueOf(json, "log_level"); !v.empty())
            cfg.log_level = v;
    }

    applyEnvOverrides(cfg);
    return cfg;
}

void ConfigManager::applyEnvOverrides(Config& cfg) {
    // Cloud-native: environment variables take precedence over config file.
    if (auto* v = std::getenv("SERVER_PORT"))        cfg.port            = static_cast<uint16_t>(std::stoi(v));
    if (auto* v = std::getenv("SERVER_PUBLIC_ROOT")) cfg.public_root     = v;
    if (auto* v = std::getenv("SERVER_THREADS"))     cfg.thread_pool_size = std::stoi(v);
    if (auto* v = std::getenv("LOG_LEVEL"))          cfg.log_level        = v;
}

} // namespace Utils
