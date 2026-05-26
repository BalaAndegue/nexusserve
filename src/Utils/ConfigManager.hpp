#pragma once
#include <cstdint>
#include <string>

namespace Utils {

// Loads config from config.json, then overrides with environment variables.
// Priority: ENV VAR > config.json > built-in default.
struct Config {
    uint16_t port{8080};
    std::string public_root{"./public"};
    int thread_pool_size{0}; // 0 = auto (hardware_concurrency)
    std::string log_level{"INFO"};
};

class ConfigManager {
  public:
    // Loads config.json from disk, then applies env-var overrides.
    static Config load(const std::string& json_path = "config.json");

  private:
    static void applyEnvOverrides(Config& cfg);
};

} // namespace Utils
